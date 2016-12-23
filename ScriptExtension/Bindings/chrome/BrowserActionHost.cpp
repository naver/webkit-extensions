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
#include "BrowserActionHost.h"

#include "BrowserActionHostMessages.h"
#include "ChromeHost.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleView.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"

#include "WebKitExtension.h"

namespace Extensions {
namespace chrome {

BrowserActionHost::BrowserActionHost(ChromeHost& chrome)
    : m_chrome(chrome)
{
}

BrowserActionHost::~BrowserActionHost()
{
}

void BrowserActionHost::setTitle(const API::Dictionary& details)
{
    m_chrome.extension().page().client().browserActionSetTitle(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), nullptr);
}

void BrowserActionHost::getTitle(const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().browserActionGetTitle(m_chrome.extension().page().page(),  static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void BrowserActionHost::setIcon(const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().browserActionSetIcon(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void BrowserActionHost::setPopup(const API::Dictionary& details)
{
    m_chrome.extension().page().client().browserActionSetPopup(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), nullptr);
}

void BrowserActionHost::getPopup(const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().browserActionGetPopup(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void BrowserActionHost::setBadgeText(const API::Dictionary& details)
{
    m_chrome.extension().page().client().browserActionSetBadgeText(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), nullptr);
}

void BrowserActionHost::getBadgeText(const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().browserActionGetBadgeText(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void BrowserActionHost::setBadgeBackgroundColor(const API::Dictionary& details)
{
    m_chrome.extension().page().client().browserActionSetBadgeBackgroundColor(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), nullptr);
}

void BrowserActionHost::getBadgeBackgroundColor(const API::Dictionary& details, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().browserActionGetBadgeBackgroundColor(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(details.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void BrowserActionHost::enable(const API::Number& tabId)
{
    m_chrome.extension().page().client().browserActionEnable(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), nullptr);
}

void BrowserActionHost::disable(const API::Number& tabId)
{
    m_chrome.extension().page().client().browserActionDisable(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(tabId.impl()), nullptr);
}

}
}
