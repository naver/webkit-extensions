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

#ifndef ArgumentDecoder_h
#define ArgumentDecoder_h

#include "ArgumentCoder.h"
#include "Array.h"
#include <WebKit/WKArray.h>
#include <WebKit/WKNumber.h>
#include <WebKit/WKRetainPtr.h>

namespace IPC {

class ArgumentDecoder {
    WTF_MAKE_FAST_ALLOCATED;
public:
    ArgumentDecoder(WKTypeRef);
    ArgumentDecoder(WKArrayRef);
    virtual ~ArgumentDecoder() = default;

    size_t length() const { return m_data.size(); }

    bool isInvalid() const { return false; }

    bool decode(bool&);
    bool decode(uint8_t&);
    bool decode(uint16_t&);
    bool decode(uint32_t&);
    bool decode(uint64_t& result) { WKUInt64Ref number; if (!decode(number, WKUInt64GetTypeID())) return false; result = WKUInt64GetValue(number); return true; }
    bool decode(int32_t&);
    bool decode(int64_t&);
    bool decode(float&);
    bool decode(double&);

    template<typename T> bool decodeEnum(T& result)
    {
        static_assert(sizeof(T) <= 8, "Enum type T must not be larger than 64 bits!");

        uint64_t value;
        if (!decode(value))
            return false;
        
        result = static_cast<T>(value);
        return true;
    }

    // Generic type decode function.
    template<typename T> bool decode(T& t)
    {
        return ArgumentCoder<T>::decode(*this, t);
    }

    template<typename T> bool decode(T& t, WKTypeID typeID)
    {
        WKTypeRef i = m_data.itemAtIndex(m_position);
        if (WKGetTypeID(i) != typeID)
            return false;

        t = static_cast<T>(i);
        ++m_position;
        return true;
    }

private:
    API::Array m_data;
    size_t m_position;
};

inline ArgumentDecoder::ArgumentDecoder(WKTypeRef data)
    : m_data(data)
    , m_position(0)
{
}

inline ArgumentDecoder::ArgumentDecoder(WKArrayRef data)
    : m_data(data)
    , m_position(0)
{
}

} // namespace IPC

#endif // ArgumentDecoder_h
