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
#include "ChromeWindowsHost.h"
#include "ChromeWindowsHostMessages.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleView.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"

#include "WebKitExtension.h"

namespace Extensions {
namespace chrome {

ChromeWindowsHost::ChromeWindowsHost(ChromeHost& chrome)
    : m_chrome(chrome)
{
}

ChromeWindowsHost::~ChromeWindowsHost()
{
}

void ChromeWindowsHost::get(const API::Number& windowId, const API::Dictionary& getInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsGet(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), static_cast<WKDictionaryRef>(getInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::getCurrent(const API::Dictionary& getInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsGetCurrent(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(getInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::getLastFocused(const API::Dictionary& getInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsGetLastFocused(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(getInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::getAll(const API::Dictionary& getInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsGetAll(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(getInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::create(const API::Dictionary& createData, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsCreate(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(createData.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::update(const API::Number& windowId, const API::Dictionary& updateInfo, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsUpdate(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), static_cast<WKDictionaryRef>(updateInfo.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void ChromeWindowsHost::remove(const API::Number& windowId, const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().windowsRemove(m_chrome.extension().page().page(), static_cast<WKDoubleRef>(windowId.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

}
}
