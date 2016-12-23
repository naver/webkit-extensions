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

#ifndef InjectedBundleProxy_h
#define InjectedBundleProxy_h

#include "ClientInterface.h"
#include "InjectedBundlePageProxyMessages.h"
#include "InjectedBundleProcess.h"
#include "PageHandle.h"
#include "WebKitExtension.h"
#include <WebKit/WKBase.h>
#include <WebKit/WKRetainPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace Extensions {

class InjectedBundlePageProxy;

typedef Client<WebKitExtensionClientBase, WebKitExtensionClientV0> WebKitExtensionClient;

class InjectedBundleProxy : public InjectedBundleProcess {
public:
    static void setDidCreateBundleCallback(WebKitExtensionDidCreateBundleCallback, const void* clientInfo);

    static InjectedBundleProxy* create(WKContextRef, WKTypeRef*);
    static InjectedBundleProxy* get(WKContextRef);
    ~InjectedBundleProxy();

    const WebKitExtensionClient::LatestClientInterface& client() const { return m_client.client(); }
    void setClient(const WebKitExtensionClientBase*);

    size_t pageCount() const { return m_pages.size(); }
    InjectedBundlePageProxy* page(unsigned index) const { return index < m_pages.size() ? m_pages[index].get() : nullptr; }
    InjectedBundlePageProxy* page(const struct OpaqueWKPage&) const;

    void broadcastSyncScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody, PassRefPtr<Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage::DelayedReply> reply);

private:
    InjectedBundleProxy();

    WKTypeRef initialize(WKContextRef);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override;
    void didReceiveSyncMessage(IPC::Connection&, IPC::MessageDecoder&, std::unique_ptr<IPC::MessageEncoder>&) override { }

    static void didReceiveMessageFromInjectedBundle(WKContextRef, WKStringRef messageName, WKTypeRef messageBody, const void *clientInfo);
    static void didReceiveSynchronousMessageFromInjectedBundle(WKContextRef, WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData, const void *clientInfo);

    void didReceiveMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody);
    void didReceiveSynchronousMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData);

    void didConnect(const ProcessIdentifier&) override;
    void didCreatePage(const API::PageHandle& page);
    void willDestroyPage(const API::PageHandle& page);
    void broadcastScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody);

    WKContextRef m_context;
    WebKitExtensionClient m_client;
    WKRetainPtr<WKPageConfigurationRef> m_pageConfiguration;
    Vector<std::unique_ptr<InjectedBundlePageProxy>> m_pages;
};

} // namespace Extensions

#endif // InjectedBundleProxy_h
