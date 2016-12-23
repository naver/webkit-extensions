/*
 * Copyright (C) 2016 Naver Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef API_String_h
#define API_String_h

#include "Object.h"
#include "StringReference.h"
#include <WebKit/WKString.h>
#include <string>
#include <wtf/text/WTFString.h>

namespace IPC {
class ArgumentDecoder;
class ArgumentEncoder;
}

namespace API {

class APIString final : public ObjectImpl<WKStringRef> {
public:
    APIString() = default;
    explicit APIString(WKTypeRef);
    explicit APIString(WKStringRef);
    explicit APIString(WebKit::WKAdoptTag, WKStringRef);
    explicit APIString(const char*);
    explicit APIString(IPC::StringReference);
    explicit APIString(const std::string&);
    explicit APIString(const WTF::String&);
    ~APIString() = default;

    APIString(const APIString& other) { m_impl = other.m_impl; }
    APIString(APIString&& other) { m_impl.swap(other.m_impl); }
    APIString& operator=(const APIString& other) { m_impl = other.m_impl; return *this; }
    APIString& operator=(APIString&& other) { m_impl.swap(other.m_impl); return *this; }

    static std::string stringValue(WKTypeRef, std::string defaultValue = std::string());

    bool isEmpty() const;

    size_t length() const;
    String characters() const;

    size_t utf8MaximumLength() const;
    std::string utf8() const { return utf8(impl()); }
    static std::string utf8(WKTypeRef);

    friend bool operator==(const APIString& a, const APIString& b);

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, APIString&);
};

template<>
struct APITypeTraits<std::string> {
    typedef APIString APIType;
    static WKTypeID typeID() { return WKStringGetTypeID(); }
    static std::string extract(WKTypeRef type) { return APIString::stringValue(type); }
};

inline APIString::APIString(WKTypeRef impl)
    : ObjectImpl(static_cast<WKStringRef>(impl))
{
    ASSERT(WKGetTypeID(impl) == WKStringGetTypeID());
}

inline APIString::APIString(WKStringRef impl)
    : ObjectImpl(impl)
{
}

inline APIString::APIString(WebKit::WKAdoptTag, WKStringRef impl)
    : ObjectImpl(AdoptWK, impl)
{
}

inline APIString::APIString(const char* s)
    : ObjectImpl(AdoptWK, WKStringCreateWithUTF8CString(s))
{
}

inline APIString::APIString(IPC::StringReference s)
    : ObjectImpl(AdoptWK, WKStringCreateWithUTF8CString(s.data()))
{
}

inline APIString::APIString(const std::string& s)
    : ObjectImpl(AdoptWK, WKStringCreateWithUTF8CString(s.c_str()))
{
}

inline APIString::APIString(const WTF::String& s)
    : ObjectImpl(AdoptWK, WKStringCreateWithUTF8CString(s.utf8().data()))
{
}

inline std::string APIString::stringValue(WKTypeRef impl, std::string defaultValue)
{
    return (WKGetTypeID(impl) == WKStringGetTypeID()) ? APIString::utf8(impl) : defaultValue;
}

inline bool APIString::isEmpty() const
{
    return WKStringIsEmpty(typeImpl());
}

inline size_t APIString::length() const
{
    return WKStringGetLength(typeImpl());
}

inline String APIString::characters() const
{
    size_t bufferLength = length();
    Vector<WKChar> buffer(bufferLength);
    WKStringGetCharacters(typeImpl(), buffer.data(), bufferLength);
    return String(buffer.data(), bufferLength);
}

inline size_t APIString::utf8MaximumLength() const
{
    return WKStringGetMaximumUTF8CStringSize(typeImpl());
}

inline std::string APIString::utf8(WKTypeRef impl)
{
    if (WKGetTypeID(impl) != WKStringGetTypeID())
        return std::string();

    WKStringRef str = static_cast<WKStringRef>(impl);
    size_t bufferLength = WKStringGetMaximumUTF8CStringSize(str);
    Vector<char> buffer(bufferLength);
    bufferLength = WKStringGetUTF8CString(str, buffer.data(), bufferLength);
    return std::string(buffer.data(), bufferLength - 1);
}

inline bool operator==(const APIString& a, const APIString& b)
{
    return WKStringIsEqual(a.typeImpl(), b.typeImpl());
}

}

using API::APIString;

#endif // API_String_h
