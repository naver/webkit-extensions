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

#ifndef Chrome_Chrome_h
#define Chrome_Chrome_h

#include "InjectedBundlePageResourceLoaderClient.h"
#include "JSWrappable.h"

namespace IPC {
class Connection;
}

namespace Extensions {

class ScriptExtension;

namespace chrome {

class BrowserAction;
class ContextMenus;
class ChromeWindows;
class EventFactory;
class I18n;
class Runtime;
class Storage;
class Tabs;
class WebRequest;

class Chrome : public JSWrappable, public InjectedBundlePageResourceLoaderClient {
public:
    Chrome(ScriptExtension&);

    JSObjectRef wrap(JSContextRef) override;

    ScriptExtension& extension() const { return m_extension; }
    EventFactory& eventFactory() { return *m_eventFactory; }
    IPC::Connection& connection() const;

    BrowserAction& browserAction() const { return const_cast<BrowserAction&>(m_browserAction.get()); }
    ChromeWindows& windows() const { return const_cast<ChromeWindows&>(m_windows.get()); }
    ContextMenus& contextMenus() const { return const_cast<ContextMenus&>(m_contextMenus.get()); }
    I18n& i18n() const { return const_cast<I18n&>(m_i18n.get()); }
    Runtime& runtime() const { return const_cast<Runtime&>(m_runtime.get()); }
    Storage& storage() const { return const_cast<Storage&>(m_storage.get()); }
    Tabs& tabs() const { return const_cast<Tabs&>(m_tabs.get()); }
    WebRequest& webRequest() const { return const_cast<WebRequest&>(m_webRequest.get()); }

private:
    // InjectedBundlePageResourceLoaderClient
    void didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, bool pageIsProvisionallyLoading) override;
    WKURLRequestRef willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, WKURLResponseRef redirectResponse) override;
    void didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLResponseRef) override;
    void didReceiveContentLengthForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, uint64_t contentLength) override;
    void didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;
    void didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKErrorRef) override;
    bool shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;
    bool shouldUseCredentialStorage(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;

    ScriptExtension& m_extension;
    std::unique_ptr<EventFactory> m_eventFactory;

    Ref<BrowserAction> m_browserAction;
    Ref<ChromeWindows> m_windows;
    Ref<ContextMenus> m_contextMenus;
    Ref<I18n> m_i18n;
    Ref<Runtime> m_runtime;
    Ref<Storage> m_storage;
    Ref<Tabs> m_tabs;
    Ref<WebRequest> m_webRequest;
};

}
}

#endif // Chrome_Chrome_h
