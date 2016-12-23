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

#ifndef ArgumentEncoder_h
#define ArgumentEncoder_h

#include "ArgumentCoder.h"
#include "Number.h"
#include <wtf/Vector.h>

namespace IPC {

class ArgumentEncoder {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ArgumentEncoder() = default;
    virtual ~ArgumentEncoder() = default;

    template<typename T> void encodeEnum(T t)
    {
        COMPILE_ASSERT(sizeof(T) <= sizeof(uint64_t), enum_type_must_not_be_larger_than_64_bits);

        encode(static_cast<uint64_t>(t));
    }

    template<typename T> void encode(T&& t)
    {
        ArgumentCoder<typename std::remove_const<typename std::remove_reference<T>::type>::type>::encode(*this, std::forward<T>(t));
    }

    template<typename T> ArgumentEncoder& operator<<(T&& t)
    {
        encode(std::forward<T>(t));
        return *this;
    }

    const API::ObjectArray& data() const { return m_data; }
    size_t dataSize() const { return m_data.size(); }

private:
    void encode(bool v) { m_data.append(API::Object::create<API::Number>(v)); }
    void encode(uint8_t v) { m_data.append(API::Object::create<API::Number>(static_cast<uint64_t>(v))); }
    void encode(uint16_t v) { m_data.append(API::Object::create<API::Number>(static_cast<uint64_t>(v))); }
    void encode(uint32_t v) { m_data.append(API::Object::create<API::Number>(static_cast<uint64_t>(v))); }
    void encode(uint64_t v) { m_data.append(API::Object::create<API::Number>(v)); }
    void encode(int32_t v) { m_data.append(API::Object::create<API::Number>(WTF::bitwise_cast<uint64_t>(int64_t(v)))); }
    void encode(int64_t v) { m_data.append(API::Object::create<API::Number>(WTF::bitwise_cast<uint64_t>(v))); }
    void encode(float v) { m_data.append(API::Object::create<API::Number>(static_cast<double>(v))); }
    void encode(double v) { m_data.append(API::Object::create<API::Number>(v)); }
    void encode(Ref<API::Object> o) { m_data.append(WTF::move(o)); }
    void encode(RefPtr<API::Object> o) { m_data.append(WTF::move(o)); }

    API::ObjectArray m_data;
};

} // namespace IPC

#endif // ArgumentEncoder_h
