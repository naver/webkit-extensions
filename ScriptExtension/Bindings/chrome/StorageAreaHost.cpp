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
#include "StorageAreaHost.h"

#include "ChromeHost.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleView.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"
#include "StorageAreaHostMessages.h"

#include "WebKitExtension.h"

namespace Extensions {
namespace chrome {

StorageAreaHost::StorageAreaHost(ChromeHost& chrome)
    : m_chrome(chrome)
{
}

StorageAreaHost::~StorageAreaHost()
{
}

void StorageAreaHost::get(const API::Dictionary& keys, const API::Function& callback)
{
    WKArrayRef arguments;
    m_chrome.extension().page().client().storageGet(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(keys.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void StorageAreaHost::getBytesInUse(const API::Dictionary& keys, const API::Function& callback)
{
    WKArrayRef arguments;
    m_chrome.extension().page().client().storageGetBytesInUse(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(keys.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void StorageAreaHost::set(const API::Dictionary& items, const API::Function& callback)
{
    WKArrayRef arguments;
    m_chrome.extension().page().client().storageSet(m_chrome.extension().page().page(), static_cast<WKDictionaryRef>(items.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void StorageAreaHost::remove(const API::Array& keys, const API::Function& callback)
{
    WKArrayRef arguments;
    m_chrome.extension().page().client().storageRemove(m_chrome.extension().page().page(), static_cast<WKArrayRef>(keys.impl()), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

void StorageAreaHost::clear(const API::Function& callback)
{
    WKArrayRef arguments;
    m_chrome.extension().page().client().storageClear(m_chrome.extension().page().page(), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

}
}
