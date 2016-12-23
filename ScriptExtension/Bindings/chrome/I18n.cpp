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
#include "I18n.h"

#include "Chrome.h"
#include "InjectedBundlePage.h"
#include "JSI18n.h"
#include "I18nHostMessages.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

I18n::I18n(Chrome& chrome)
    : m_chrome(chrome)
{
}

JSObjectRef I18n::wrap(JSContextRef context)
{
    m_context = context;
    return JSObjectMake(context, JSI18n::i18nClass(), this);
}

void I18n::getAcceptLanguages(RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::I18nHost::GetAcceptLanguages(*toAPI(callback)), 0);
}

RefPtr<JSString> I18n::getMessage(RefPtr<JSString> messageName, RefPtr<JSDictionary> substitutions)
{
    API::APIString result;
    m_chrome.connection().sendSync(Messages::I18nHost::GetMessage(*toAPI(messageName), *toAPI(substitutions)), Messages::I18nHost::GetMessage::Reply(result), 0);
    return JSString::create(m_context, JSValue::create(m_context, WTF::getPtr(const_cast<API::APIString&>(result)))->value());
}

RefPtr<JSString> I18n::getUILanguage(RefPtr<JSString> messageName, RefPtr<JSDictionary> substitutions)
{
    API::APIString result;
    m_chrome.connection().sendSync(Messages::I18nHost::GetUILanguage(*toAPI(messageName), *toAPI(substitutions)), Messages::I18nHost::GetUILanguage::Reply(result), 0);
    return JSString::create(m_context, JSValue::create(m_context, WTF::getPtr(const_cast<API::APIString&>(result)))->value());
}

void I18n::detectLanguage(RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::I18nHost::DetectLanguage(*toAPI(callback)), 0);
}

}
}
