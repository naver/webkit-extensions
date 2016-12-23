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

#ifndef MessageDecoder_h
#define MessageDecoder_h

#include "APIString.h"
#include "ArgumentCoders.h"
#include "ArgumentDecoder.h"
#include "StringReference.h"

namespace IPC {

class MessageDecoder : public ArgumentDecoder {
public:
    MessageDecoder(const APIString&, WKTypeRef);
    MessageDecoder(const APIString&, WKArrayRef);
    MessageDecoder(StringReference, StringReference, WKArrayRef);
    virtual ~MessageDecoder() = default;

    StringReference messageReceiverName() const { return m_messageReceiverName; }
    StringReference messageName() const { return m_messageName; }
    uint64_t destinationID() const { return m_destinationID; }

private:
    void parseMessage();

    std::string m_message;
    StringReference m_messageReceiverName;
    StringReference m_messageName;
    uint64_t m_destinationID;
};

inline MessageDecoder::MessageDecoder(const APIString& message, WKTypeRef arguments)
    : ArgumentDecoder(arguments)
    , m_message(message.utf8())
    , m_destinationID(0)
{
    parseMessage();
}

inline MessageDecoder::MessageDecoder(const APIString& message, WKArrayRef arguments)
    : ArgumentDecoder(arguments)
    , m_message(message.utf8())
    , m_destinationID(0)
{
    parseMessage();
}

inline MessageDecoder::MessageDecoder(StringReference messageReceiverName, StringReference messageName, WKArrayRef arguments)
    : ArgumentDecoder(arguments)
    , m_messageReceiverName(messageReceiverName)
    , m_messageName(messageName)
    , m_destinationID(0)
{
}

inline void MessageDecoder::parseMessage()
{
    const char* str = m_message.c_str();
    size_t positions[] = { m_message.find_first_of(':'), m_message.find_last_of(':') };
    size_t lengths[] = { positions[0], positions[1] - positions[0] - 1 };

    m_messageReceiverName = StringReference(str, lengths[0]);
    m_messageName = StringReference(str + positions[0] + 1, lengths[1]);
    m_message[positions[0]] = 0;
    m_message[positions[1]] = 0;

    m_destinationID = std::atoll(str + positions[1] + 1);
}

} // namespace IPC

#endif // MessageDecoder_h
