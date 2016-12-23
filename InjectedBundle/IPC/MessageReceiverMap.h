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

#ifndef MessageReceiverMap_h
#define MessageReceiverMap_h

#include "Connection.h"
#include "MessageReceiver.h"
#include "StringReference.h"
#include <wtf/HashMap.h>

namespace IPC {

class MessageReceiverMap {
public:
    MessageReceiverMap();
    ~MessageReceiverMap();

    void addMessageReceiver(StringReference messageReceiverName, MessageReceiver&);
    void addMessageReceiver(StringReference messageReceiverName, uint64_t destinationID, MessageReceiver&);

    void removeMessageReceiver(StringReference messageReceiverName);
    void removeMessageReceiver(StringReference messageReceiverName, uint64_t destinationID);

    void invalidate();

    bool dispatchMessage(Connection&, MessageDecoder&);
    bool dispatchSyncMessage(Connection&, MessageDecoder&, std::unique_ptr<MessageEncoder>&);

private:
    // Message receivers that don't require a destination ID.
    HashMap<StringReference, MessageReceiver*> m_globalMessageReceivers;

    HashMap<std::pair<StringReference, uint64_t>, MessageReceiver*> m_messageReceivers;
};

inline MessageReceiverMap::MessageReceiverMap()
{
}

inline MessageReceiverMap::~MessageReceiverMap()
{
}

inline void MessageReceiverMap::addMessageReceiver(StringReference messageReceiverName, MessageReceiver& messageReceiver)
{
    ASSERT(!m_globalMessageReceivers.contains(messageReceiverName));

    messageReceiver.willBeAddedToMessageReceiverMap();
    m_globalMessageReceivers.set(messageReceiverName, &messageReceiver);
}

inline void MessageReceiverMap::addMessageReceiver(StringReference messageReceiverName, uint64_t destinationID, MessageReceiver& messageReceiver)
{
    ASSERT(destinationID);
    ASSERT(!m_messageReceivers.contains(std::make_pair(messageReceiverName, destinationID)));
    ASSERT(!m_globalMessageReceivers.contains(messageReceiverName));

    messageReceiver.willBeAddedToMessageReceiverMap();
    m_messageReceivers.set(std::make_pair(messageReceiverName, destinationID), &messageReceiver);
}

inline void MessageReceiverMap::removeMessageReceiver(StringReference messageReceiverName)
{
    ASSERT(m_globalMessageReceivers.contains(messageReceiverName));

    auto it = m_globalMessageReceivers.find(messageReceiverName);
    it->value->willBeRemovedFromMessageReceiverMap();

    m_globalMessageReceivers.remove(it);
}

inline void MessageReceiverMap::removeMessageReceiver(StringReference messageReceiverName, uint64_t destinationID)
{
    ASSERT(m_messageReceivers.contains(std::make_pair(messageReceiverName, destinationID)));

    auto it = m_messageReceivers.find(std::make_pair(messageReceiverName, destinationID));
    it->value->willBeRemovedFromMessageReceiverMap();

    m_messageReceivers.remove(it);
}

inline void MessageReceiverMap::invalidate()
{
    for (auto& messageReceiver : m_globalMessageReceivers.values())
        messageReceiver->willBeRemovedFromMessageReceiverMap();
    m_globalMessageReceivers.clear();


    for (auto& messageReceiver : m_messageReceivers.values())
        messageReceiver->willBeRemovedFromMessageReceiverMap();
    m_messageReceivers.clear();
}

inline bool MessageReceiverMap::dispatchMessage(Connection& connection, MessageDecoder& decoder)
{
    if (MessageReceiver* messageReceiver = m_globalMessageReceivers.get(decoder.messageReceiverName())) {
        ASSERT(!decoder.destinationID());

        messageReceiver->didReceiveMessage(connection, decoder);
        return true;
    }

    if (MessageReceiver* messageReceiver = m_messageReceivers.get(std::make_pair(decoder.messageReceiverName(), decoder.destinationID()))) {
        messageReceiver->didReceiveMessage(connection, decoder);
        return true;
    }

    return false;
}

inline bool MessageReceiverMap::dispatchSyncMessage(Connection& connection, MessageDecoder& decoder, std::unique_ptr<MessageEncoder>& replyEncoder)
{
    if (MessageReceiver* messageReceiver = m_globalMessageReceivers.get(decoder.messageReceiverName())) {
        ASSERT(!decoder.destinationID());

        messageReceiver->didReceiveSyncMessage(connection, decoder, replyEncoder);
        return true;
    }

    if (MessageReceiver* messageReceiver = m_messageReceivers.get(std::make_pair(decoder.messageReceiverName(), decoder.destinationID()))) {
        messageReceiver->didReceiveSyncMessage(connection, decoder, replyEncoder);
        return true;
    }

    return false;
}

};

#endif // MessageReceiverMap_h
