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

#ifndef API_Dictionary_h
#define API_Dictionary_h

#include "APIString.h"
#include "StringHash.h"
#include <WebKit/WKArray.h>
#include <WebKit/WKDictionary.h>
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace API {

typedef HashMap<std::string, RefPtr<Object>> ObjectDictionary;

class Dictionary final : public ObjectImpl<WKDictionaryRef> {
public:
    struct KeyValue {
        RefPtr<APIString> key;
        RefPtr<API::Object> value;
    };

    template<typename... KeyValues> static Ref<Dictionary> create(KeyValues&&...);

    Dictionary() = default;
    explicit Dictionary(WKTypeRef);
    explicit Dictionary(WKDictionaryRef);
    explicit Dictionary(WebKit::WKAdoptTag, WKDictionaryRef);
    explicit Dictionary(const ObjectDictionary&);
    ~Dictionary() = default;

    Dictionary(const Dictionary& other) { m_impl = other.m_impl; }
    Dictionary(Dictionary&& other) { m_impl.swap(other.m_impl); }
    Dictionary& operator=(const Dictionary& other) { m_impl = other.m_impl; return *this; }
    Dictionary& operator=(Dictionary&& other) { m_impl.swap(other.m_impl); return *this; }

    RefPtr<Object> itemForKey(const APIString&) const;
    size_t size() const;

    Vector<Ref<APIString>> copyKeys() const;

    bool containsKey(const APIString&) const;
    static ObjectDictionary copyKeyValues(WKTypeRef);
    ObjectDictionary copyKeyValues() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Dictionary&);
};

template<>
struct APITypeTraits<ObjectDictionary> {
    typedef Dictionary APIType;
    static WKTypeID typeID() { return WKDictionaryGetTypeID(); }
    static ObjectDictionary extract(WKTypeRef type) { return Dictionary::copyKeyValues(type); }
};

inline WKDictionaryRef createDictionaryFromMap(const ObjectDictionary& map)
{
    size_t numberOfValues = map.size();

    auto keysIterator = map.keys().begin();
    Vector<WKStringRef> keys(numberOfValues);
    Vector<APIString> strings(numberOfValues);
    for (size_t i = 0; i < numberOfValues; ++i, ++keysIterator) {
        strings[i] = APIString(*keysIterator);
        keys[i] = strings[i].typeImpl();
    }

    auto valuesIterator = map.values().begin();
    Vector<WKTypeRef> values(numberOfValues);
    for (size_t i = 0; i < numberOfValues; ++i, ++valuesIterator)
        values[i] = valuesIterator->get()->impl();

    return WKDictionaryCreate(keys.data(), values.data(), numberOfValues);
}

inline Dictionary::Dictionary(WKTypeRef impl)
    : ObjectImpl(static_cast<WKDictionaryRef>(impl))
{
    ASSERT(WKGetTypeID(impl) == WKDictionaryGetTypeID());
}

inline Dictionary::Dictionary(WKDictionaryRef impl)
    : ObjectImpl(impl)
{
}

inline Dictionary::Dictionary(WebKit::WKAdoptTag, WKDictionaryRef impl)
    : ObjectImpl(AdoptWK, impl)
{
}

inline Dictionary::Dictionary(const ObjectDictionary& map)
    : ObjectImpl(createDictionaryFromMap(map))
{
}

inline RefPtr<Object> Dictionary::itemForKey(const APIString& key) const
{
    return Object::create(WKDictionaryGetItemForKey(typeImpl(), key.typeImpl()));
}

inline size_t Dictionary::size() const
{
    return WKDictionaryGetSize(typeImpl());
}

inline Vector<Ref<APIString>> Dictionary::copyKeys() const
{
    WKRetainPtr<WKArrayRef> keys = adoptWK(WKDictionaryCopyKeys(typeImpl()));
    size_t numberOfKeys = WKArrayGetSize(keys.get());

    Vector<Ref<APIString>> result;
    for (size_t i = 0; i < numberOfKeys; ++i) {
        WKTypeRef key = WKArrayGetItemAtIndex(keys.get(), i);
        ASSERT(WKGetTypeID(key) == WKStringGetTypeID());
        result.append(API::Object::create<API::APIString>(key));
    }

    return WTF::move(result);
}

inline bool Dictionary::containsKey(const APIString& key) const
{
    return !!WKDictionaryGetItemForKey(typeImpl(), key.typeImpl());
}

inline ObjectDictionary Dictionary::copyKeyValues(WKTypeRef impl)
{
    Ref<Dictionary> dictionary = adoptRef(*new Dictionary(impl));
    return dictionary->copyKeyValues();
}

inline ObjectDictionary Dictionary::copyKeyValues() const
{
    Vector<Ref<APIString>> keys = copyKeys();
    ObjectDictionary result;
    for (auto& key : keys) {
        Ref<API::APIString> str = API::Object::create<API::APIString>(key);
        result.add(key->utf8(), itemForKey(str.get()));
    }

    return WTF::move(result);
}

class DictionaryBuilder {
    friend class Dictionary;
private:
    bool set(Dictionary::KeyValue&& keyValue);
    Ref<Dictionary> build();

    ObjectDictionary m_dictionary;
};

inline bool DictionaryBuilder::set(Dictionary::KeyValue&& keyValue)
{
    m_dictionary.set(keyValue.key->utf8(), keyValue.value);
    return true;
}

inline Ref<Dictionary> DictionaryBuilder::build()
{
    return API::Object::create<API::Dictionary>(m_dictionary);
}

template<typename... KeyValues> inline Ref<Dictionary> Dictionary::create(KeyValues&&... keyValues)
{
    DictionaryBuilder builder;
    auto results = { builder.set(std::forward<KeyValues>(keyValues))... };
    return builder.build();
}

inline Dictionary::KeyValue operator^(RefPtr<APIString> key, RefPtr<API::Object> value)
{
    return { key, value };
}

inline Dictionary::KeyValue operator^(Ref<APIString> key, Ref<API::Object> value)
{
    return { key.ptr(), value.ptr() };
}

}

#endif // API_Dictionary_h
