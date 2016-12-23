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

#ifndef API_GetSet_h
#define API_GetSet_h

#include "Object.h"
#include <wtf/HashMap.h>

namespace API {

class Getter {
public:
    Getter(const ObjectDictionary& values) : m_values(values) { }
    template<typename T> bool get(const std::string& key, T& value);

private:
    const ObjectDictionary& m_values;
};

template<typename T>
inline bool Getter::get(const std::string& key, T& value)
{
    typedef typename std::decay<T>::type ValueType;

    if (!m_values.contains(key))
        return false;

    RefPtr<Object> item = m_values.get(key);
    if (!item)
        return false;

    if (item->typeID() != APITypeTraits<ValueType>::typeID())
        return false;

    value = APITypeTraits<ValueType>::extract(item->impl());
    return true;
}

class Setter {
public:
    Setter(ObjectDictionary& values) : m_values(values) { }
    template<typename T> void set(std::string&& key, T&& value);

private:
    ObjectDictionary& m_values;
};

template<typename T>
inline void Setter::set(std::string&& key, T&& value)
{
    m_values.set(std::forward<std::string>(key), Object::create<typename APITypeTraits<typename std::decay<T>::type>::APIType>(value));
}

}

#endif // API_GetSet_h
