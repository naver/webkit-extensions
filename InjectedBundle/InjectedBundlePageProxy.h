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

#ifndef InjectedBundlePageProxy_h
#define InjectedBundlePageProxy_h

#include "ClientInterface.h"
#include "Connection.h"
#include "Function.h"
#include "InjectedBundlePageProxyMessages.h"
#include "MessageReceiverMap.h"
#include "ScriptExtensionHost.h"
#include "WebKitExtension.h"
#include <WebKit/WKPage.h>
#include <WebKit/WKBundleScriptWorld.h>
#include <WebKit/WKRetainPtr.h>
#include <wtf/text/WTFString.h>

namespace Extensions {

class InjectedBundleProxy;
class InjectedBundleView;

typedef Client<WebKitExtensionPageClientBase, WebKitExtensionPageClientV0> WebKitExtensionPageClient;

class InjectedBundlePageProxy : public IPC::MessageReceiver {
public:
    InjectedBundlePageProxy(InjectedBundleProxy&, WKPageRef);
    ~InjectedBundlePageProxy();

    void initialize();

    WKPageRef page() const { return m_page; }
    InjectedBundleProxy& bundle() const { return m_bundle; }

    const WebKitExtensionPageClient::LatestClientInterface& client() const { return m_client.client(); }
    void setClient(const WebKitExtensionPageClientBase*);

    IPC::Connection& connection() { return m_connection; }
    void addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver&);
    void addMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID, IPC::MessageReceiver&);
    void removeMessageReceiver(IPC::StringReference messageReceiverName);
    void removeMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID);

    void dispatchCallback(const API::Function&, const API::Array&);

    void scriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody);

private:
    void dispatchMessage(IPC::Connection&, IPC::MessageDecoder&);
    void dispatchSyncMessage(IPC::Connection&, IPC::MessageDecoder&, std::unique_ptr<IPC::MessageEncoder>&);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override;
    void didReceiveSyncMessage(IPC::Connection&, IPC::MessageDecoder&, std::unique_ptr<IPC::MessageEncoder>&) override;

    void dispatchFunction(const API::Function&, const API::Array&);
    void broadcastSyncScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody, PassRefPtr<Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage::DelayedReply> reply);

    // WKPageInjectedBundleClient
    static void didReceiveMessageFromInjectedBundle(WKPageRef, WKStringRef messageName, WKTypeRef messageBody, const void *clientInfo);
    static void didReceiveSynchronousMessageFromInjectedBundle(WKPageRef, WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData, const void *clientInfo);

    void didReceiveMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody);
    void didReceiveSynchronousMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData);

    WKPageRef m_page;
    InjectedBundleProxy& m_bundle;
    WebKitExtensionPageClient m_client;

    IPC::ConnectionToBundlePage m_connection;
    IPC::MessageReceiverMap m_messageReceiverMap;

    ScriptExtensionHost m_scriptExtension;
};

} // namespace Extensions

#endif // InjectedBundlePageProxy_h
