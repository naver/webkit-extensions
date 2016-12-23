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

#include "config.h"
#include "InjectedBundlePageProxy.h"

#include "InjectedBundlePageMessages.h"
#include "InjectedBundleProxy.h"
#include "InjectedBundleView.h"
#include <WebKit/WKPage.h>
#include <WebKit/WKPageInjectedBundleClient.h>

namespace Extensions {

InjectedBundlePageProxy::InjectedBundlePageProxy(InjectedBundleProxy& bundle, WKPageRef page)
    : m_page(page)
    , m_bundle(bundle)
    , m_connection(m_page)
    , m_scriptExtension(*this)
{
    WKPageInjectedBundleClientV0 injectedBundleClient = {
        { 0, this },
        didReceiveMessageFromInjectedBundle,
        didReceiveSynchronousMessageFromInjectedBundle,
    };
    WKPageSetPageInjectedBundleClient(m_page, &injectedBundleClient.base);

    m_scriptExtension.initialize();
}

InjectedBundlePageProxy::~InjectedBundlePageProxy()
{
}

void InjectedBundlePageProxy::setClient(const WebKitExtensionPageClientBase* client)
{
    m_client.initialize(client);
}

void InjectedBundlePageProxy::addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, messageReceiver);
}

void InjectedBundlePageProxy::addMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, destinationID, messageReceiver);
}

void InjectedBundlePageProxy::removeMessageReceiver(IPC::StringReference messageReceiverName)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName);
}

void InjectedBundlePageProxy::removeMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName, destinationID);
}

void InjectedBundlePageProxy::dispatchCallback(const API::Function& callback, const API::Array& arguments)
{
    connection().send(Messages::InjectedBundlePage::DispatchFunction(callback, arguments), 0);
}

void InjectedBundlePageProxy::scriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody)
{
    std::string receiverName(messageReceiverName.utf8());
    std::string name(messageName.utf8());
    WKTypeRef type = messageBody.impl();
    IPC::MessageDecoder decoder(IPC::StringReference(receiverName.c_str(), receiverName.size()), IPC::StringReference(name.c_str(), name.size()), WKArrayCreate(&type, 1));
    dispatchMessage(connection(), decoder);
}

void InjectedBundlePageProxy::dispatchMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder)
{
    if (!m_messageReceiverMap.dispatchMessage(connection, decoder))
        didReceiveMessage(connection, decoder);
}

void InjectedBundlePageProxy::dispatchSyncMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder, std::unique_ptr<IPC::MessageEncoder>& replyEncoder)
{
    if (!m_messageReceiverMap.dispatchSyncMessage(connection, decoder, replyEncoder))
        didReceiveSyncMessage(connection, decoder, replyEncoder);
}

void InjectedBundlePageProxy::dispatchFunction(const API::Function& function, const API::Array& arguments)
{
    function.function(nullptr)(arguments);
}

void InjectedBundlePageProxy::broadcastSyncScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody, PassRefPtr<Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage::DelayedReply> reply)
{
    bundle().broadcastSyncScriptMessage(messageReceiverName, messageName, messageBody, reply);
}

void InjectedBundlePageProxy::didReceiveMessageFromInjectedBundle(WKPageRef, WKStringRef messageName, WKTypeRef messageBody, const void *clientInfo)
{
    static_cast<InjectedBundlePageProxy*>(const_cast<void*>(clientInfo))->didReceiveMessageFromInjectedBundle(messageName, messageBody);
}

void InjectedBundlePageProxy::didReceiveSynchronousMessageFromInjectedBundle(WKPageRef, WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData, const void *clientInfo)
{
    static_cast<InjectedBundlePageProxy*>(const_cast<void*>(clientInfo))->didReceiveSynchronousMessageFromInjectedBundle(messageName, messageBody, returnData);
}

void InjectedBundlePageProxy::didReceiveMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);
    dispatchMessage(m_connection, decoder);
}

void InjectedBundlePageProxy::didReceiveSynchronousMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);

    if (decoder.messageReceiverName() == IPC::StringReference("IPC")) {
        if (decoder.messageName() == IPC::StringReference("WaitForSyncReply")) {
            m_connection.didReceiveWaitMessage();
            *returnData = 0;
        }
        return;
    }

    auto replyEncoder = std::make_unique<IPC::MessageEncoder>(IPC::StringReference("IPC"), IPC::StringReference("SyncMessageReply"), 0);
    dispatchSyncMessage(m_connection, decoder, replyEncoder);
    *returnData = (replyEncoder) ? API::Array(replyEncoder->data()).leakImpl() : 0;
}

} // namespace Extensions
