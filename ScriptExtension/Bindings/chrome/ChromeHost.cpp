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
#include "ChromeHost.h"

#include "BrowserActionHost.h"
#include "BrowserActionHostMessages.h"
#include "ChromeStrings.h"
#include "ChromeWindowsHost.h"
#include "ChromeWindowsHostMessages.h"
#include "ContextMenusHost.h"
#include "ContextMenusHostMessages.h"
#include "Dictionary.h"
#include "EventProxyFactory.h"
#include "InjectedBundlePage.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleProxy.h"
#include "InjectedBundleView.h"
#include "I18nHost.h"
#include "I18nHostMessages.h"
#include "PortConnections.h"
#include "PortHost.h"
#include "PortHostMessages.h"
#include "RuntimeHost.h"
#include "RuntimeHostMessages.h"
#include "StorageAreaHost.h"
#include "StorageAreaHostMessages.h"
#include "TabsHost.h"
#include "TabsHostMessages.h"
#include "WebRequestHost.h"

namespace Extensions {
namespace chrome {

ChromeHost::ChromeHost(ScriptExtensionHost& extension)
    : m_extension(extension)
    , m_eventFactory(std::make_unique<EventProxyFactory>(*this))
    , m_browserAction(std::make_unique<BrowserActionHost>(*this))
    , m_windows(std::make_unique<ChromeWindowsHost>(*this))
    , m_contextMenus(std::make_unique<ContextMenusHost>(*this))
    , m_i18n(std::make_unique<I18nHost>(*this))
    , m_port(std::make_unique<PortHost>(*this))
    , m_runtime(std::make_unique<RuntimeHost>(*this))
    , m_storage(std::make_unique<StorageAreaHost>(*this))
    , m_tabs(std::make_unique<TabsHost>(*this))
    , m_webRequest(std::make_unique<WebRequestHost>(*this))
{
    m_extension.page().bundle().addSupplement<PortConnections>();
    m_extension.page().addMessageReceiver(Messages::BrowserActionHost::messageReceiverName(), *m_browserAction);
    m_extension.page().addMessageReceiver(Messages::ChromeWindowsHost::messageReceiverName(), *m_windows);
    m_extension.page().addMessageReceiver(Messages::ContextMenusHost::messageReceiverName(), *m_contextMenus);
    m_extension.page().addMessageReceiver(Messages::I18nHost::messageReceiverName(), *m_i18n);
    m_extension.page().addMessageReceiver(Messages::PortHost::messageReceiverName(), *m_port);
    m_extension.page().addMessageReceiver(Messages::RuntimeHost::messageReceiverName(), *m_runtime);
    m_extension.page().addMessageReceiver(Messages::StorageAreaHost::messageReceiverName(), *m_storage);
    m_extension.page().addMessageReceiver(Messages::TabsHost::messageReceiverName(), *m_tabs);
}

ChromeHost::~ChromeHost()
{
}

void ChromeHost::scriptMessage(const API::Dictionary& messageBody)
{
    RefPtr<APIString> eventName(static_pointer_cast<APIString>(messageBody.itemForKey(*ChromeStrings::chromeEvent())));
    RefPtr<API::Array> arguments(static_pointer_cast<API::Array>(messageBody.itemForKey(*ChromeStrings::arguments())));
    m_eventFactory->scriptMessage(WTF::move(eventName), WTF::move(arguments));
}

}
}
