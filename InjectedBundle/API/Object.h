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

#ifndef API_Object_h
#define API_Object_h

#include <WebKit/WKRetainPtr.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace API {

class Object : public ThreadSafeRefCounted<Object> {
public:
    static Ref<Object> create(WKTypeRef);
    template<typename T, typename... Args> static Ref<T> create(Args&&...);

    virtual ~Object()
    {
    }

    virtual WKTypeID typeID() const { return WKGetTypeID(m_impl.get()); }

    WKTypeRef impl() const
    {
        return m_impl.get();
    }

    template<typename T> T impl() const
    {
        return static_cast<T>(m_impl.get());
    }

    WKTypeRef leakImpl()
    {
        return m_impl.leakRef();
    }

    template<typename T> static Ref<T> copy(const T& o);

protected:
    Object()
    {
    }

    Object(WebKit::WKAdoptTag)
    {
    }

    Object(WKTypeRef type)
        : m_impl(type)
    {
    }

    Object(WebKit::WKAdoptTag, WKTypeRef type)
        : m_impl(AdoptWK, type)
    {
    }

    WKRetainPtr<WKTypeRef> m_impl;
};

template <typename WKType>
class ObjectImpl : public Object {
public:
    virtual ~ObjectImpl()
    {
    }

    WKType typeImpl() const
    {
        return impl<WKType>();
    }

protected:
    ObjectImpl()
    {
    }

    ObjectImpl(WKType type)
        : Object(type)
    {
    }

    ObjectImpl(WebKit::WKAdoptTag, WKType type)
        : Object(AdoptWK, type)
    {
    }
};

template<typename T, typename... Args> Ref<T> Object::create(Args&&... args)
{
    return adoptRef(*new T(std::forward<Args>(args)...));
}

template<typename T> Ref<T> Object::copy(const T& o)
{
    return adoptRef(*new T(o));
}

template<typename T>
struct APITypeTraits;

typedef RefPtr<Object> ObjectRef;

template<>
struct APITypeTraits<ObjectRef> {
    typedef Object APIType;
    static WKTypeID typeID() { return 0;}
    static ObjectRef extract(WKTypeRef type) { return Object::create(type); }
};

}

#endif // API_Object_h
