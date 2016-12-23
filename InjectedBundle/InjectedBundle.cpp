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
#include "InjectedBundle.h"

#include "InjectedBundlePage.h"
#include "InjectedBundleProcessMessages.h"
#include "InjectedBundleProxyMessages.h"
#include "PageHandle.h"
#include <WebKit/WKBundle.h>
#include <WebKit/WKBundlePage.h>
#include <WebKit/WKBundlePagePrivate.h>
#include <WebKit/WKBundlePrivate.h>
#include <WebKit/WebKit2_C.h>
#include <wtf/text/CString.h>
#include <wtf/text/StringBuilder.h>

namespace Extensions {

InjectedBundle& InjectedBundle::singleton()
{
    static InjectedBundle& shared = *new InjectedBundle;
    return shared;
}

InjectedBundle::InjectedBundle()
    : InjectedBundleProcess(IPC::currentProcess())
    , m_bundle(0)
{
}

InjectedBundle::~InjectedBundle()
{
}

void InjectedBundle::initialize(WKBundleRef bundle, WKTypeRef initializationUserData)
{
    m_bundle = bundle;

    setConnection(std::make_unique<IPC::ConnectionToContext>(m_bundle));

    WKBundleClientV1 client = {
        { 1, this },
        didCreatePage,
        willDestroyPage,
        didInitializePageGroup,
        didReceiveMessage,
        didReceiveMessageToPage
    };
    WKBundleSetClient(m_bundle, &client.base);

    connection().send(Messages::InjectedBundleProcess::DidConnect(processID()), 0);
}

void InjectedBundle::didCreatePage(WKBundleRef bundle, WKBundlePageRef page, const void* clientInfo)
{
    static_cast<InjectedBundle*>(const_cast<void*>(clientInfo))->didCreatePage(page);
}

void InjectedBundle::willDestroyPage(WKBundleRef bundle, WKBundlePageRef page, const void* clientInfo)
{
    static_cast<InjectedBundle*>(const_cast<void*>(clientInfo))->willDestroyPage(page);
}

void InjectedBundle::didInitializePageGroup(WKBundleRef bundle, WKBundlePageGroupRef pageGroup, const void* clientInfo)
{
    static_cast<InjectedBundle*>(const_cast<void*>(clientInfo))->didInitializePageGroup(pageGroup);
}

void InjectedBundle::didReceiveMessage(WKBundleRef bundle, WKStringRef messageName, WKTypeRef messageBody, const void* clientInfo)
{
    static_cast<InjectedBundle*>(const_cast<void*>(clientInfo))->didReceiveMessage(messageName, messageBody);
}

void InjectedBundle::didReceiveMessageToPage(WKBundleRef bundle, WKBundlePageRef page, WKStringRef messageName, WKTypeRef messageBody, const void* clientInfo)
{
    static_cast<InjectedBundle*>(const_cast<void*>(clientInfo))->didReceiveMessageToPage(page, messageName, messageBody);
}

void InjectedBundle::didCreatePage(WKBundlePageRef page)
{
    auto webPage = std::make_unique<InjectedBundlePage>(page);

    connection().send(Messages::InjectedBundleProxy::DidCreatePage(API::PageHandle(page)), 0);

    webPage->initialize();
    m_pages.add(page, std::move(webPage));
}

void InjectedBundle::willDestroyPage(WKBundlePageRef page)
{
    connection().send(Messages::InjectedBundleProxy::WillDestroyPage(API::PageHandle(page)), 0);
    m_pages.remove(page);
}

void InjectedBundle::didInitializePageGroup(WKBundlePageGroupRef pageGroup)
{
    m_pageGroup = pageGroup;
}

void InjectedBundle::didReceiveMessage(WKStringRef messageName, WKTypeRef messageBody)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);

    if (decoder.messageReceiverName() == IPC::StringReference("IPC")) {
        if (decoder.messageName() == IPC::StringReference("SyncMessageReply"))
            connection().dispatchSyncReply(std::make_unique<IPC::MessageDecoder>(WTF::move(decoder)));
        return;
    }

    dispatchMessage(connection(), decoder);
}

void InjectedBundle::didReceiveMessageToPage(WKBundlePageRef page, WKStringRef messageName, WKTypeRef messageBody)
{
    this->page(page)->didReceiveMessage(messageName, messageBody);
}

void InjectedBundle::didConnect(const ProcessIdentifier& processID)
{
    if (getConnection(processID))
        return;

    addConnection(processID);
}

} // namespace Extensions
