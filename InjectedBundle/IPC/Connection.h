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

#ifndef Connection_h
#define Connection_h

#include "APIString.h"
#include "Array.h"
#include "MessageDecoder.h"
#include "MessageEncoder.h"
#include "StringReference.h"
#include <WebKit/WKBundle.h>
#include <WebKit/WKBundlePage.h>
#include <WebKit/WKContext.h>
#include <WebKit/WKPage.h>

namespace IPC {

class WaitForSyncReply;

class Connection {
    friend class WaitForSyncReply;
public:
    virtual ~Connection() = default;

    bool canSendSyncMessages() const { return m_canSendSyncMessages; }

    template<typename T> void send(T&& message, uint64_t destinationID);
    template<typename T> bool sendSync(T&& message, typename T::Reply&& reply, uint64_t destinationID);

    bool sendSyncReply(std::unique_ptr<MessageEncoder>);
    void dispatchSyncReply(std::unique_ptr<MessageDecoder>);

    bool shouldWaitForSyncReply() const;
    void waitForSyncReply();

    void ref();
    void deref();

    void didReceiveWaitMessage();

protected:
    Connection() = default;

    void setCanSendSyncMessages(bool canSendSyncMessages) { m_canSendSyncMessages = canSendSyncMessages; }

private:
    virtual void sendMessage(const APIString& message, const API::Array& arguments) = 0;
    virtual void sendSyncMessage(const APIString& message, const API::Array& arguments, WKTypeRef& returnData) = 0;
    bool sendSyncMessageThenWait(const APIString& message, const API::Array& arguments, WaitForSyncReply&);

    bool m_canSendSyncMessages { false };
    std::atomic<bool> m_waitForSyncReply;
    std::unique_ptr<MessageDecoder> m_syncMessageReply;
    std::unique_ptr<MessageEncoder> m_outgoingSyncMessageReply;
};

class WaitForSyncReply {
public:
    WaitForSyncReply(Connection& connection)
        : m_connection(connection)
        , m_waitMessage(StringReference("IPC"), StringReference("WaitForSyncReply"), 0)
    {
        m_connection.waitForSyncReply();
    }
    ~WaitForSyncReply()
    {
        ASSERT(!m_connection.shouldWaitForSyncReply());
    }

    virtual bool conditionalWait(WKTypeRef&) = 0;

protected:
    void sendWaitMessage()
    {
        WKTypeRef returnData = 0;
        m_connection.sendSyncMessage(APIString(m_waitMessage.message()), API::Array(m_waitMessage.data()), returnData);
        ASSERT(!returnData);
    }

    Connection& m_connection;
    MessageEncoder m_waitMessage;
};

template<class T, class Enable = void>
class WaitForSyncMessageReply : public WaitForSyncReply {
public:
    WaitForSyncMessageReply(Connection& connection)
        : WaitForSyncReply(connection)
    {
    }

    bool conditionalWait(WKTypeRef& returnData) override
    {
        if (!returnData)
            return false;

        ASSERT(WKGetTypeID(returnData) == WKArrayGetTypeID());
        auto decoder = std::make_unique<MessageDecoder>(StringReference("IPC"), StringReference("SyncMessageReply"), static_cast<WKArrayRef>(returnData));
        m_connection.dispatchSyncReply(std::move(decoder));
        return true;
    }
};
 
template<class T>
class WaitForSyncMessageReply<T, typename std::enable_if<std::is_class<typename T::DelayedReply>::value>::type> : public WaitForSyncReply {
public:
    WaitForSyncMessageReply(Connection& connection)
        : WaitForSyncReply(connection)
    {
    }

    bool conditionalWait(WKTypeRef& returnData) override
    {
        ASSERT(!returnData);

        static const std::chrono::milliseconds mininumInterval = std::chrono::milliseconds(10);

        if (m_connection.shouldWaitForSyncReply())
            sendWaitMessage();

        while (m_connection.shouldWaitForSyncReply()) {
            std::this_thread::sleep_for(mininumInterval);
            sendWaitMessage();
        }
        return true;
    }
};

template<typename T> void Connection::send(T&& message, uint64_t destinationID)
{
    COMPILE_ASSERT(!T::isSync, AsyncMessageExpected);

    auto encoder = std::make_unique<MessageEncoder>(T::receiverName(), T::name(), destinationID);
    encoder->encode(message.arguments());
    
    sendMessage(APIString(encoder->message()), API::Array(encoder->data()));
}

template<typename T> bool Connection::sendSync(T&& message, typename T::Reply&& reply, uint64_t destinationID)
{
    COMPILE_ASSERT(T::isSync, SyncMessageExpected);

    auto encoder = std::make_unique<MessageEncoder>(T::receiverName(), T::name(), destinationID);
    encoder->encode(message.arguments());

    WaitForSyncMessageReply<T> syncMessageReply(*this);
    sendSyncMessageThenWait(APIString(encoder->message()), API::Array(encoder->data()), syncMessageReply);

    ASSERT(!m_waitForSyncReply.load());
    std::unique_ptr<MessageDecoder> replyDecoder = std::move(m_syncMessageReply);
    return replyDecoder->decode(reply);
}

inline bool Connection::sendSyncReply(std::unique_ptr<MessageEncoder> encoder)
{
    ASSERT(!m_outgoingSyncMessageReply);

    m_outgoingSyncMessageReply = std::move(encoder);
    return true;
}

inline void Connection::dispatchSyncReply(std::unique_ptr<MessageDecoder> decoder)
{
    ASSERT(m_waitForSyncReply.load());

    m_syncMessageReply = std::move(decoder);
    m_waitForSyncReply = false;
}

inline bool Connection::shouldWaitForSyncReply() const
{
    return m_waitForSyncReply.load();
}

inline void Connection::waitForSyncReply()
{
    m_waitForSyncReply = true;
}

inline void Connection::ref()
{
}

inline void Connection::deref()
{
}

inline void Connection::didReceiveWaitMessage()
{
    if (!m_outgoingSyncMessageReply)
        return;

    std::unique_ptr<MessageEncoder> outgoingSyncMessageReply = std::move(m_outgoingSyncMessageReply);
    sendMessage(APIString(outgoingSyncMessageReply->message()), API::Array(outgoingSyncMessageReply->data()));
}

inline bool Connection::sendSyncMessageThenWait(const APIString& message, const API::Array& arguments, WaitForSyncReply& syncMessageReply)
{
    WKTypeRef returnData = nullptr;
    sendSyncMessage(message, arguments, returnData);
    if (!syncMessageReply.conditionalWait(returnData))
        return false;
    return true;
}

class ConnectionToBundle final : public Connection {
public:
    ConnectionToBundle(WKContextRef context) : m_context(context) { }

private:
    void sendMessage(const APIString& message, const API::Array& arguments) override { WKContextPostMessageToInjectedBundle(m_context, message.typeImpl(), arguments.typeImpl()); }
    void sendSyncMessage(const APIString&, const API::Array&, WKTypeRef& returnData) override { ASSERT_NOT_REACHED(); returnData = nullptr; }

    WKContextRef m_context;
};

class ConnectionToContext final : public Connection {
public:
    ConnectionToContext(WKBundleRef bundle) : m_bundle(bundle) { setCanSendSyncMessages(true); }

private:
    void sendMessage(const APIString& message, const API::Array& arguments) override { WKBundlePostMessage(m_bundle, message.typeImpl(), arguments.typeImpl()); }
    void sendSyncMessage(const APIString& message, const API::Array& arguments, WKTypeRef& returnData) override { WKBundlePostSynchronousMessage(m_bundle, message.typeImpl(), arguments.typeImpl(), &returnData); }

    WKBundleRef m_bundle;
};

class ConnectionToBundlePage final : public Connection {
public:
    ConnectionToBundlePage(WKPageRef page) : m_page(page) { }

private:
    void sendMessage(const APIString& message, const API::Array& arguments) override { WKPagePostMessageToInjectedBundle(m_page, message.typeImpl(), arguments.typeImpl()); }
    void sendSyncMessage(const APIString&, const API::Array&, WKTypeRef& returnData) override { ASSERT_NOT_REACHED(); returnData = nullptr; }

    WKPageRef m_page;
};

class ConnectionToPage final : public Connection {
public:
    ConnectionToPage(WKBundlePageRef page) : m_page(page) { setCanSendSyncMessages(true); }

private:
    void sendMessage(const APIString& message, const API::Array& arguments) override { WKBundlePagePostMessage(m_page, message.typeImpl(), arguments.typeImpl()); }
    void sendSyncMessage(const APIString& message, const API::Array& arguments, WKTypeRef& returnData) override { WKBundlePagePostSynchronousMessageForTesting(m_page, message.typeImpl(), arguments.typeImpl(), &returnData); }

    WKBundlePageRef m_page;
};

}

#endif // Connection_h
