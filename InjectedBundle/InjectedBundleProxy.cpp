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
#include "InjectedBundleProxy.h"

#include "APIString.h"
#include "MessageDecoder.h"
#include "InjectedBundleProcessMessages.h"
#include "InjectedBundlePageProxy.h"
#include <WebKit/WKContext.h>
#include <WebKit/WKPageConfigurationRef.h>
#include <WebKit/WKPageGroup.h>

namespace Extensions {

static const char* viewPageGroupIdentifier = "__InjectedBundleProxyViewPageGroup__";

static const WKPageGroupRef viewPageGroup()
{
    static WKPageGroupRef pageGroup;
    if (!pageGroup) {
        WKRetainPtr<WKStringRef> pageGroupIdentifier(AdoptWK, WKStringCreateWithUTF8CString(viewPageGroupIdentifier));
        pageGroup = WKPageGroupCreateWithIdentifier(pageGroupIdentifier.get());
    };

    return pageGroup;
}

typedef HashMap<WKContextRef, std::unique_ptr<InjectedBundleProxy>> ContextBundleMap;

static ContextBundleMap& bundles()
{
    static ContextBundleMap& shared = *new ContextBundleMap;
    return shared;
}

InjectedBundleProxy* InjectedBundleProxy::create(WKContextRef context, WKTypeRef* result)
{
    ASSERT(!bundles().contains(context));

    InjectedBundleProxy* bundle = new InjectedBundleProxy;
    WKTypeRef resultData = bundle->initialize(context);
    if (result)
        *result = resultData;
    bundles().add(context, std::unique_ptr<InjectedBundleProxy>(bundle));
    return bundle;
}

InjectedBundleProxy* InjectedBundleProxy::get(WKContextRef context)
{
    return bundles().get(context);
}

InjectedBundleProxy::InjectedBundleProxy()
    : InjectedBundleProcess(IPC::currentProcess())
    , m_context(0)
    , m_pageConfiguration(AdoptWK, WKPageConfigurationCreate())
{
}

InjectedBundleProxy::~InjectedBundleProxy()
{
}

struct InjectedBundleProxyDidCreateBundleCallback {
    WebKitExtensionDidCreateBundleCallback callback;
    const void* clientInfo;
};

static InjectedBundleProxyDidCreateBundleCallback didCreateBundleCallback;

void InjectedBundleProxy::setDidCreateBundleCallback(WebKitExtensionDidCreateBundleCallback callback, const void* clientInfo)
{
    didCreateBundleCallback = { callback, clientInfo };
}

void InjectedBundleProxy::setClient(const WebKitExtensionClientBase* client)
{
    m_client.initialize(client);
}

InjectedBundlePageProxy* InjectedBundleProxy::page(const struct OpaqueWKPage& pageRef) const
{
    WKPageRef pagePtr = &pageRef;
    auto page = std::find_if(m_pages.begin(), m_pages.end(), [=] (const std::unique_ptr<InjectedBundlePageProxy>& a) {
        return a->page() == pagePtr;
    });
    return page != m_pages.end() ? (*page).get() : nullptr;
}

WKTypeRef InjectedBundleProxy::initialize(WKContextRef context)
{
    m_context = context;

    WKPageConfigurationSetContext(m_pageConfiguration.get(), m_context);
    WKPageConfigurationSetPageGroup(m_pageConfiguration.get(), viewPageGroup());

    setConnection(std::make_unique<IPC::ConnectionToBundle>(m_context));

    WKContextInjectedBundleClientV1 injectedBundleClient = {
        { 1, this },
        didReceiveMessageFromInjectedBundle,
        didReceiveSynchronousMessageFromInjectedBundle,
        0 //getInjectedBundleInitializationUserData
    };
    WKContextSetInjectedBundleClient(m_context, &injectedBundleClient.base);

    if (didCreateBundleCallback.callback)
        didCreateBundleCallback.callback(reinterpret_cast<WebKitExtensionRef>(this), m_context, didCreateBundleCallback.clientInfo);

    return 0;
}

void InjectedBundleProxy::didReceiveMessageFromInjectedBundle(WKContextRef, WKStringRef messageName, WKTypeRef messageBody, const void *clientInfo)
{
    static_cast<InjectedBundleProxy*>(const_cast<void*>(clientInfo))->didReceiveMessageFromInjectedBundle(messageName, messageBody);
}

void InjectedBundleProxy::didReceiveSynchronousMessageFromInjectedBundle(WKContextRef, WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData, const void *clientInfo)
{
    static_cast<InjectedBundleProxy*>(const_cast<void*>(clientInfo))->didReceiveSynchronousMessageFromInjectedBundle(messageName, messageBody, returnData);
}

void InjectedBundleProxy::didReceiveMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);
    dispatchMessage(connection(), decoder);
}

void InjectedBundleProxy::didReceiveSynchronousMessageFromInjectedBundle(WKStringRef messageName, WKTypeRef messageBody, WKTypeRef* returnData)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);

    if (decoder.messageReceiverName() == IPC::StringReference("IPC")) {
        if (decoder.messageName() == IPC::StringReference("WaitForSyncReply")) {
            connection().didReceiveWaitMessage();
            *returnData = 0;
        }
        return;
    }

    auto replyEncoder = std::make_unique<IPC::MessageEncoder>(IPC::StringReference("IPC"), IPC::StringReference("SyncMessageReply"), 0);
    dispatchSyncMessage(connection(), decoder, replyEncoder);
    *returnData = (replyEncoder) ? API::Array(replyEncoder->data()).leakImpl() : 0;
}

void InjectedBundleProxy::didConnect(const ProcessIdentifier& processID)
{
    addConnection(processID);

    connection().send(Messages::InjectedBundleProcess::DidConnect(this->processID()), 0);
}

void InjectedBundleProxy::didCreatePage(const API::PageHandle& page)
{
    m_pages.append(std::make_unique<InjectedBundlePageProxy>(*this, page.page()));

    if (client().didCreatePage)
        client().didCreatePage(reinterpret_cast<WebKitExtensionRef>(this), page.page(), client().base.clientInfo);
}

void InjectedBundleProxy::willDestroyPage(const API::PageHandle& page)
{
    if (client().willDestroyPage)
        client().willDestroyPage(reinterpret_cast<WebKitExtensionRef>(this), page.page(), client().base.clientInfo);
}

void InjectedBundleProxy::broadcastScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody)
{
    for (auto& page : m_pages)
        page->scriptMessage(messageReceiverName, messageName, messageBody);
}

void InjectedBundleProxy::broadcastSyncScriptMessage(const APIString& messageReceiverName, const APIString& messageName, const API::Dictionary& messageBody, PassRefPtr<Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage::DelayedReply> reply)
{
    // FIXME
    page(0)->scriptMessage(messageReceiverName, messageName, messageBody);

    reply->send(API::Dictionary::create(API::Object::create<APIString>("cancel") ^ API::Object::create<API::Number>(false)));
}

} // namespace Extensions
