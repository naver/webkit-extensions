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

#ifndef API_Array_h
#define API_Array_h

#include "APIString.h"
#include "Number.h"
#include "Dictionary.h"
#include <WebKit/WKArray.h>
#include <functional>
#include <wtf/Vector.h>

namespace API {

typedef Vector<RefPtr<Object>> ObjectArray;

class Array final : public ObjectImpl<WKArrayRef> {
public:
    template<typename... Values> static Ref<Array> create(Values&&...);

    Array();
    explicit Array(WKTypeRef);
    explicit Array(WKArrayRef);
    explicit Array(WebKit::WKAdoptTag, WKArrayRef);
    explicit Array(const ObjectArray&);
    template<typename T> explicit Array(const Vector<T>&);
    ~Array() = default;

    Array(const Array& other) { m_impl = other.m_impl; }
    Array(Array&& other) { m_impl.swap(other.m_impl); }
    Array& operator=(const Array& other) { m_impl = other.m_impl; return *this; }
    Array& operator=(Array&& other) { m_impl.swap(other.m_impl); return *this; }

    template<typename T> static std::pair<Vector<T>, bool> values(WKTypeRef, std::function<std::pair<T, bool> (WKTypeRef)>);
    template<typename T> static std::pair<Vector<T>, bool> values(WKTypeRef);
    template<typename T> std::pair<Vector<T>, bool> values() const { return values<T>(impl()); }

    WKTypeRef itemAtIndex(size_t) const;
    RefPtr<Object> copyItemAtIndex(size_t) const;
    size_t size() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Array&);
};

template<typename T>
struct APITypeTraits<Vector<T>> {
    typedef Array APIType;
    static WKTypeID typeID() { return WKArrayGetTypeID(); }
    static Vector<T> extract(WKTypeRef type) { return Array::values<T>(type).first; }
};

template<>
struct APITypeTraits<ObjectArray> {
    typedef Array APIType;
    static WKTypeID typeID() { return WKArrayGetTypeID(); }
    static ObjectArray extract(WKTypeRef type) { return Array::values<ObjectRef>(type).first; }
};

inline WKArrayRef createArrayFromVector(const ObjectArray& values)
{
    size_t numberOfValues = values.size();
    Vector<WKTypeRef> items(numberOfValues);
    for (size_t i = 0; i < numberOfValues; ++i)
        items[i] = values[i] ? values[i]->impl() : nullptr;

    return WKArrayCreate(items.data(), numberOfValues);
}

template<typename T> inline WKArrayRef createArrayFromVector(const Vector<T>& values)
{
    ObjectArray items;
    for (const auto& value : values)
        items.append(adoptRef(new typename APITypeTraits<T>::APIType(value)));

    return createArrayFromVector(items);
}

inline Array::Array()
    : ObjectImpl(AdoptWK, WKArrayCreate(0, 0))
{
}

inline Array::Array(WKTypeRef impl)
    : ObjectImpl(static_cast<WKArrayRef>(impl))
{
    ASSERT(WKGetTypeID(impl) == WKArrayGetTypeID());
}

inline Array::Array(WKArrayRef impl)
    : ObjectImpl(impl)
{
}

inline Array::Array(WebKit::WKAdoptTag, WKArrayRef impl)
    : ObjectImpl(AdoptWK, impl)
{
}

inline Array::Array(const ObjectArray& values)
    : ObjectImpl(AdoptWK, createArrayFromVector(values))
{
}

template<typename T> inline Array::Array(const Vector<T>& values)
    : ObjectImpl(AdoptWK, createArrayFromVector(values))
{
}

template<typename T> inline std::pair<Vector<T>, bool> Array::values(WKTypeRef impl, std::function<std::pair<T, bool> (WKTypeRef)> cast)
{
    std::pair<Vector<T>, bool> result = { Vector<T>(), false };

    if (WKGetTypeID(impl) != WKArrayGetTypeID())
        return WTF::move(result);

    WKArrayRef array = static_cast<WKArrayRef>(impl);
    size_t size = WKArrayGetSize(array);
    for (size_t index = 0; index < size; ++index) {
        auto castResult = cast(WKArrayGetItemAtIndex(array, index));
        if (!castResult.second)
            return result;
        result.first.append(castResult.first);
    }

    result.second = true;
    return result;
}

template<typename T> inline std::pair<Vector<T>, bool> Array::values(WKTypeRef impl)
{
    return values<T>(impl, [](WKTypeRef item) -> std::pair<T, bool> {
            return std::make_pair(APITypeTraits<T>::extract(item), WKGetTypeID(item) == APITypeTraits<T>::typeID());
        });
}

inline WKTypeRef Array::itemAtIndex(size_t index) const
{
    return WKArrayGetItemAtIndex(typeImpl(), index);
}

inline RefPtr<Object> Array::copyItemAtIndex(size_t index) const
{
    return Object::create(itemAtIndex(index));
}

inline size_t Array::size() const
{
    return WKArrayGetSize(typeImpl());
}

class ArrayBuilder {
    friend class Array;
private:
    bool add(bool v);
    bool add(uint8_t v);
    bool add(uint16_t v);
    bool add(uint32_t v);
    bool add(uint64_t v);
    bool add(int32_t v);
    bool add(int64_t v);
    bool add(float v);
    bool add(double v);
    bool add(const char* v);
    bool add(const std::string& v);
    bool add(Ref<API::Object> o);
    bool add(RefPtr<API::Object> o);
    bool add(Ref<API::Dictionary> o);
    template<typename T> bool add(const T& o);
    Ref<Array> build();

    ObjectArray m_array;
};

inline bool ArrayBuilder::add(bool v)
{
    m_array.append(API::Object::create<API::Number>(v));
    return true;
}

inline bool ArrayBuilder::add(uint8_t v)
{
    m_array.append(API::Object::create<API::Number>(static_cast<uint64_t>(v)));
    return true;
}

inline bool ArrayBuilder::add(uint16_t v)
{
    m_array.append(API::Object::create<API::Number>(static_cast<uint64_t>(v)));
    return true;
}

inline bool ArrayBuilder::add(uint32_t v)
{
    m_array.append(API::Object::create<API::Number>(static_cast<uint64_t>(v)));
    return true;
}

inline bool ArrayBuilder::add(uint64_t v)
{
    m_array.append(API::Object::create<API::Number>(v));
    return true;
}

inline bool ArrayBuilder::add(int32_t v)
{
    m_array.append(API::Object::create<API::Number>(WTF::bitwise_cast<uint64_t>(int64_t(v))));
    return true;
}

inline bool ArrayBuilder::add(int64_t v)
{
    m_array.append(API::Object::create<API::Number>(WTF::bitwise_cast<uint64_t>(v)));
    return true;
}

inline bool ArrayBuilder::add(float v)
{
    m_array.append(API::Object::create<API::Number>(static_cast<double>(v)));
    return true;
}

inline bool ArrayBuilder::add(double v)
{
    m_array.append(API::Object::create<API::Number>(v));
    return true;
}

inline bool ArrayBuilder::add(const char* v)
{
    m_array.append(API::Object::create<APIString>(v));
    return true;
}

inline bool ArrayBuilder::add(const std::string& v)
{
    m_array.append(API::Object::create<APIString>(v));
    return true;
}

inline bool ArrayBuilder::add(Ref<API::Object> o)
{
    m_array.append(WTF::move(o));
    return true;
}

inline bool ArrayBuilder::add(RefPtr<API::Object> o)
{
    m_array.append(WTF::move(o));
    return true;
}

inline bool ArrayBuilder::add(Ref<API::Dictionary> o)
{
    m_array.append(WTF::move(o));
    return true;
}

template<typename T> inline bool ArrayBuilder::add(const T& o)
{
    m_array.append(API::Object::copy(o));
    return true;
}

inline Ref<Array> ArrayBuilder::build()
{
    return API::Object::create<API::Array>(m_array);
}

template<typename... Values> inline Ref<Array> Array::create(Values&&... values)
{
    ArrayBuilder builder;
    auto results = { builder.add(std::forward<Values>(values))... };
    return builder.build();
}

}

#endif // API_Array_h
