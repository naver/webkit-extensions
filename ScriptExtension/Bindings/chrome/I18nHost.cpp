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
#include "I18nHost.h"

#include "ChromeHost.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleView.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"
#include "I18nHostMessages.h"

#include "WebKitExtension.h"

namespace Extensions {
namespace chrome {

I18nHost::I18nHost(ChromeHost& chrome)
    : m_chrome(chrome)
{
}

I18nHost::~I18nHost()
{
}

void I18nHost::getAcceptLanguages(const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().i18nGetAcceptLanguages(m_chrome.extension().page().page(), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}
void I18nHost::getMessage(const API::APIString& messageName, const API::Dictionary& substitutions, RefPtr<API::APIString>& result)
{
    result = API::Object::create<API::APIString>(m_chrome.extension().page().client().i18nGetMessage(m_chrome.extension().page().page(), static_cast<WKStringRef>(messageName.impl()), static_cast<WKDictionaryRef>(substitutions.impl()), nullptr));
}

void I18nHost::getUILanguage(const API::APIString& messageName, const API::Dictionary& substitutions, RefPtr<API::APIString>& result)
{
    result= API::Object::create<API::APIString>(m_chrome.extension().page().client().i18nGetUILanguage(m_chrome.extension().page().page(), static_cast<WKStringRef>(messageName.impl()), static_cast<WKDictionaryRef>(substitutions.impl()), nullptr));
}

void I18nHost::detectLanguage(const API::Function& callback)
{
    WKArrayRef arguments = WKArrayCreate(0, 0);
    m_chrome.extension().page().client().i18nDetectLanguage(m_chrome.extension().page().page(), &arguments, nullptr);
    m_chrome.extension().page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

}
}
