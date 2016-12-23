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
#include "TabsHost.h"

#include "ChromeHost.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleView.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"
#include "TabsHostMessages.h"

#include "WebKitExtension.h"

namespace Extensions {
namespace chrome {

TabsHost::TabsHost(ChromeHost& chrome)
    : m_chrome(chrome)
{
}

TabsHost::~TabsHost()
{
}

void TabsHost::get(const API::Number& tabId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsGet(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::getCurrent(const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsGetCurrent(m_chrome.extension().page().page(), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::connect(const API::Number& tabId, const API::Dictionary& connectInfo)
{
}

void TabsHost::sendRequest(const API::Number& tabId, const API::Dictionary& request, const API::Function& responseCallback)
{
}

void TabsHost::sendMessage(const API::Number& tabId, const API::Dictionary& message, const API::Dictionary& options, const API::Function& responseCallback)
{
}

void TabsHost::getSelected(const API::Number& windowId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsGetSelected(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::getAllInWindow(const API::Number& windowId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsGetAllInWindow(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::create(const API::Dictionary& createProperties, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsCreate(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(createProperties.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::duplicate(const API::Number& tabId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsDuplicate(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::query(const API::Dictionary& queryInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsQuery(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(queryInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::highlight(const API::Dictionary& highlightInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsHighlight(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(highlightInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::update(const API::Number& tabId, const API::Dictionary& updateProperties, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsUpdate(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), static_cast<WKDictionaryRef>(updateProperties.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::move(const API::Array& tabIds, const API::Dictionary& moveProperties, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsMove(m_chrome.extension().page().page(), static_cast<WKArrayRef>(tabIds.impl()), static_cast<WKDictionaryRef>(moveProperties.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::reload(const API::Number& tabId, const API::Dictionary& reloadProperties, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsReload(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), static_cast<WKDictionaryRef>(reloadProperties.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::remove(const API::Array& tabIds, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsRemove(m_chrome.extension().page().page(), static_cast<WKArrayRef>(tabIds.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::detectLanguage(const API::Array& tabIds, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsDetectLanguage(m_chrome.extension().page().page(), static_cast<WKArrayRef>(tabIds.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::captureVisibleTab(const API::Number& windowId, const API::Dictionary& options, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsCaptureVisibleTab(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), static_cast<WKDictionaryRef>(options.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::executeScript(const API::Number& tabId, const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsExecuteScript(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::insertCSS(const API::Number& tabId, const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsInsertCSS(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::setZoom(const API::Number& tabId, const API::Number& zoomFactor, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsSetZoom(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), static_cast<WKDoubleRef>(zoomFactor.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void TabsHost::getZoom(const API::Number& tabId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().tabsGetZoom(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

}
}
