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

#ifndef Chrome_ChromeHost_h
#define Chrome_ChromeHost_h

#include "MessageReceiver.h"

namespace API {
class Dictionary;
}

namespace Extensions {

class InjectedBundleView;
class ScriptExtensionHost;

namespace chrome {

class BrowserActionHost;
class ChromeWindowsHost;
class ContextMenusHost;
class EventProxyFactory;
class I18nHost;
class PortHost;
class RuntimeHost;
class StorageAreaHost;
class TabsHost;
class WebRequestHost;

class ChromeHost : public IPC::MessageReceiver {
public:
    ChromeHost(ScriptExtensionHost&);
    ~ChromeHost();

    ScriptExtensionHost& extension() const { return m_extension; }
    EventProxyFactory& eventFactory() const { return *m_eventFactory; }

    BrowserActionHost& browserAction() const { return *m_browserAction; }
    ChromeWindowsHost& windows() const { return *m_windows; }
    ContextMenusHost& contextMenus() const { return *m_contextMenus; }
    I18nHost& i18n() const { return *m_i18n; }
    PortHost& port() const { return *m_port; }
    RuntimeHost& runtime() const { return *m_runtime; }
    StorageAreaHost& storage() const { return *m_storage; }
    TabsHost& tabs() const { return *m_tabs; }
    WebRequestHost& webRequest() const { return *m_webRequest; }

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void scriptMessage(const API::Dictionary& messageBody);

    ScriptExtensionHost& m_extension;
    std::unique_ptr<EventProxyFactory> m_eventFactory;

    std::unique_ptr<BrowserActionHost> m_browserAction;
    std::unique_ptr<ChromeWindowsHost> m_windows;
    std::unique_ptr<ContextMenusHost> m_contextMenus;
    std::unique_ptr<I18nHost> m_i18n;
    std::unique_ptr<PortHost> m_port;
    std::unique_ptr<RuntimeHost> m_runtime;
    std::unique_ptr<StorageAreaHost> m_storage;
    std::unique_ptr<TabsHost> m_tabs;
    std::unique_ptr<WebRequestHost> m_webRequest;
};

}
}

#endif // Chrome_ChromeHost_h
