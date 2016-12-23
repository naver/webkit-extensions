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

#ifndef API_Data_h
#define API_Data_h

#include "Object.h"
#include <WebKit/WKData.h>
#include <wtf/Vector.h>

namespace IPC {
class ArgumentDecoder;
class ArgumentEncoder;
}

namespace API {

class Data : public ObjectImpl<WKDataRef> {
public:
    typedef void (*FreeDataFunction)(uint8_t*, const void* context);

    Data() = default;
    explicit Data(WKTypeRef);
    explicit Data(WKDataRef);
    explicit Data(WebKit::WKAdoptTag, WKDataRef);
    explicit Data(const uint8_t*, size_t, FreeDataFunction = nullptr, const void* = 0);
    template<typename T> explicit Data(T, FreeDataFunction = nullptr, const void* = 0);
    virtual ~Data() = default;

    Data(const Data& other) { m_impl = other.m_impl; m_freeDataFunction = other.m_freeDataFunction; }
    Data(Data&& other) { m_impl.swap(other.m_impl); m_freeDataFunction.swap(other.m_freeDataFunction); }
    Data& operator=(const Data& other) { m_impl = other.m_impl; m_freeDataFunction = other.m_freeDataFunction; return *this; }
    Data& operator=(Data&& other) { m_impl.swap(other.m_impl); m_freeDataFunction.swap(other.m_freeDataFunction); return *this; }

    template<typename T> void initialize(FreeDataFunction = nullptr, const void* = 0);
    template<typename T> void initialize(T, FreeDataFunction = nullptr, const void* = 0);
    template<typename T> static void finalize(const uint8_t*);

    const uint8_t* bytes() const;
    size_t size() const;
    template<typename T> const T& data() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Data&);

protected:
    struct FreeDataFunctionWrapper : ThreadSafeRefCounted<FreeDataFunctionWrapper> {
        WKDataRef data;
        FreeDataFunction function;
        const void* context;
        FreeDataFunctionWrapper(WKDataRef, FreeDataFunction, const void*);
        ~FreeDataFunctionWrapper();
    };

    RefPtr<FreeDataFunctionWrapper> m_freeDataFunction;
};

inline Data::FreeDataFunctionWrapper::FreeDataFunctionWrapper(WKDataRef data, FreeDataFunction function, const void* context)
    : data(data)
    , function(function)
    , context(context)
{
}

inline Data::FreeDataFunctionWrapper::~FreeDataFunctionWrapper()
{
    if (function)
        (*function)(const_cast<uint8_t*>(WKDataGetBytes(data)), context);
}

template<size_t size> inline WKDataRef createEmptyWKData()
{
    uint8_t bytes[size] = { 0, };
    return WKDataCreate(bytes, size);
}

inline Data::Data(WKTypeRef impl)
    : ObjectImpl(static_cast<WKDataRef>(impl))
{
    ASSERT(WKGetTypeID(impl) == WKDataGetTypeID());
}

inline Data::Data(WKDataRef impl)
    : ObjectImpl(impl)
{
}

inline Data::Data(WebKit::WKAdoptTag, WKDataRef impl)
    : ObjectImpl(AdoptWK, impl)
{
}

inline Data::Data(const uint8_t* bytes, size_t size, FreeDataFunction freeDataFunction, const void* context)
    : ObjectImpl(AdoptWK, WKDataCreate(bytes, size))
    , m_freeDataFunction(adoptRef(new FreeDataFunctionWrapper(typeImpl(), freeDataFunction, context)))
{
}

template<typename T> inline Data::Data(T data, FreeDataFunction freeDataFunction, const void* context)
{
    initialize(WTF::move(data), freeDataFunction, context);
}

template<typename T> inline void Data::initialize(FreeDataFunction freeDataFunction, const void* context)
{
    m_impl.adopt(createEmptyWKData<sizeof(T)>());
    new (reinterpret_cast<T*>(const_cast<uint8_t*>(bytes()))) T();
    m_freeDataFunction = adoptRef(new FreeDataFunctionWrapper(typeImpl(), freeDataFunction, context));
}

template<typename T> inline void Data::initialize(T data, FreeDataFunction freeDataFunction, const void* context)
{
    m_impl.adopt(createEmptyWKData<sizeof(T)>());
    new (reinterpret_cast<T*>(const_cast<uint8_t*>(bytes()))) T(WTF::move(data));
    m_freeDataFunction = adoptRef(new FreeDataFunctionWrapper(typeImpl(), freeDataFunction, context));
}

template<typename T> inline void Data::finalize(const uint8_t* bytes)
{
    T* ptr = reinterpret_cast<T*>(const_cast<uint8_t*>(bytes));
    ptr->~T();
}

inline const uint8_t* Data::bytes() const
{
    return WKDataGetBytes(typeImpl());
}

inline size_t Data::size() const
{
    return WKDataGetSize(typeImpl());
}

template<typename T> const T& Data::data() const
{
    return *reinterpret_cast<const T*>(bytes());
}

}

#endif // API_Data_h
