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

#ifndef API_Number_h
#define API_Number_h

#include "Object.h"
#include <WebKit/WKNumber.h>

namespace IPC {
class ArgumentDecoder;
class ArgumentEncoder;
}

namespace API {

class Number final : public Object {
public:
    Number();
    explicit Number(bool);
    explicit Number(double);
    explicit Number(uint64_t);
    explicit Number(WKTypeRef);
    explicit Number(WKBooleanRef);
    explicit Number(WKDoubleRef);
    explicit Number(WKUInt64Ref);
    explicit Number(WebKit::WKAdoptTag, WKBooleanRef);
    explicit Number(WebKit::WKAdoptTag, WKDoubleRef);
    explicit Number(WebKit::WKAdoptTag, WKUInt64Ref);
    ~Number() = default;

    Number(const Number& other) { m_impl = other.m_impl; }
    Number(Number&& other) { m_impl.swap(other.m_impl); }
    Number& operator=(const Number& other) { m_impl = other.m_impl; return *this; }
    Number& operator=(Number&& other) { m_impl.swap(other.m_impl); return *this; }

    static bool booleanValue(WKTypeRef, bool defaultValue = false);
    static double doubleValue(WKTypeRef, double defaultValue = 0.0);
    static uint64_t uint64Value(WKTypeRef, uint64_t defaultValue = 0);

    bool isBoolean() const { return WKGetTypeID(impl()) == WKBooleanGetTypeID(); }
    bool isDouble() const { return WKGetTypeID(impl()) == WKDoubleGetTypeID(); }
    bool isUInt64() const { return WKGetTypeID(impl()) == WKUInt64GetTypeID(); }

    bool booleanValue() const;
    double doubleValue() const;
    uint64_t uint64Value() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Number&);
};

template<>
struct APITypeTraits<bool> {
    typedef Number APIType;
    static WKTypeID typeID() { return WKBooleanGetTypeID(); }
    static bool extract(WKTypeRef type) { return Number::booleanValue(type); }
};

template<>
struct APITypeTraits<double> {
    typedef Number APIType;
    static WKTypeID typeID() { return WKDoubleGetTypeID(); }
    static double extract(WKTypeRef type) { return Number::doubleValue(type); }
};

template<>
struct APITypeTraits<uint64_t> {
    typedef Number APIType;
    static WKTypeID typeID() { return WKUInt64GetTypeID(); }
    static uint64_t extract(WKTypeRef type) { return Number::uint64Value(type); }
};

inline Number::Number()
{
}

inline Number::Number(bool value)
    : Object(AdoptWK, WKBooleanCreate(value))
{
}

inline Number::Number(double value)
    : Object(AdoptWK, WKDoubleCreate(value))
{
}

inline Number::Number(uint64_t value)
    : Object(AdoptWK, WKUInt64Create(value))
{
}

inline Number::Number(WKTypeRef impl)
    : Object(impl)
{
    ASSERT(WKGetTypeID(impl) == WKBooleanGetTypeID() || WKGetTypeID(impl) == WKDoubleGetTypeID() || WKGetTypeID(impl) == WKUInt64GetTypeID());
}

inline Number::Number(WKBooleanRef impl)
    : Object(impl)
{
}

inline Number::Number(WKDoubleRef impl)
    : Object(impl)
{
}

inline Number::Number(WKUInt64Ref impl)
    : Object(impl)
{
}

inline Number::Number(WebKit::WKAdoptTag, WKBooleanRef impl)
    : Object(AdoptWK, impl)
{
}

inline Number::Number(WebKit::WKAdoptTag, WKDoubleRef impl)
    : Object(AdoptWK, impl)
{
}

inline Number::Number(WebKit::WKAdoptTag, WKUInt64Ref impl)
    : Object(AdoptWK, impl)
{
}

inline bool Number::booleanValue(WKTypeRef item, bool defaultValue)
{
    return (WKGetTypeID(item) == WKBooleanGetTypeID()) ? WKBooleanGetValue(static_cast<WKBooleanRef>(item)) : defaultValue;
}

inline double Number::doubleValue(WKTypeRef item, double defaultValue)
{
    return (WKGetTypeID(item) == WKDoubleGetTypeID()) ? WKDoubleGetValue(static_cast<WKDoubleRef>(item)) : defaultValue;
}

inline uint64_t Number::uint64Value(WKTypeRef item, uint64_t defaultValue)
{
    return (WKGetTypeID(item) == WKUInt64GetTypeID()) ? WKUInt64GetValue(static_cast<WKUInt64Ref>(item)) : defaultValue;
}

inline bool Number::booleanValue() const
{
    if (isBoolean())
        return WKBooleanGetValue(impl<WKBooleanRef>());
    else if (isDouble())
        return WKDoubleGetValue(impl<WKDoubleRef>()) != 0.0;
    else if (isUInt64())
        return WKUInt64GetValue(impl<WKUInt64Ref>()) != 0;

    ASSERT_NOT_REACHED();
    return false;
}

inline double Number::doubleValue() const
{
    if (isBoolean())
        return (WKBooleanGetValue(impl<WKBooleanRef>())) ? 1.0 : 0.0;
    else if (isDouble())
        return WKDoubleGetValue(impl<WKDoubleRef>());
    else if (isUInt64())
        return static_cast<double>(WKUInt64GetValue(impl<WKUInt64Ref>()));

    ASSERT_NOT_REACHED();
    return false;
}

inline uint64_t Number::uint64Value() const
{
    if (isBoolean())
        return (WKBooleanGetValue(impl<WKBooleanRef>())) ? 1 : 0;
    else if (isDouble())
        return static_cast<uint64_t>(WKDoubleGetValue(impl<WKDoubleRef>()));
    else if (isUInt64())
        return WKUInt64GetValue(impl<WKUInt64Ref>());

    ASSERT_NOT_REACHED();
    return false;
}

}

#endif // API_Number_h
