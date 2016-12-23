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
#include "BrowserAction.h"

#include "BrowserActionHostMessages.h"
#include "Chrome.h"
#include "InjectedBundlePage.h"
#include "JSBrowserAction.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

BrowserAction::BrowserAction(Chrome& chrome)
    : m_chrome(chrome)
{
}

JSObjectRef BrowserAction::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSBrowserAction::browserActionClass(), this);
}

void BrowserAction::setTitle(RefPtr<JSDictionary> details)
{
    m_chrome.connection().send(Messages::BrowserActionHost::SetTitle(*toAPI(details)), 0);
}

void BrowserAction::getTitle(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::BrowserActionHost::GetTitle(*toAPI(details), *toAPI(callback)), 0);
}

void BrowserAction::setIcon(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::BrowserActionHost::SetIcon(*toAPI(details), *toAPI(callback)), 0);
}

void BrowserAction::setPopup(RefPtr<JSDictionary> details)
{
    m_chrome.connection().send(Messages::BrowserActionHost::SetPopup(*toAPI(details)), 0);
}

void BrowserAction::getPopup(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::BrowserActionHost::GetPopup(*toAPI(details), *toAPI(callback)), 0);
}

void BrowserAction::setBadgeText(RefPtr<JSDictionary> details)
{
    m_chrome.connection().send(Messages::BrowserActionHost::SetBadgeText(*toAPI(details)), 0);
}

void BrowserAction::getBadgeText(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::BrowserActionHost::GetBadgeText(*toAPI(details), *toAPI(callback)), 0);
}

void BrowserAction::setBadgeBackgroundColor(RefPtr<JSDictionary> details)
{
    m_chrome.connection().send(Messages::BrowserActionHost::SetBadgeBackgroundColor(*toAPI(details)), 0);
}

void BrowserAction::getBadgeBackgroundColor(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::BrowserActionHost::GetBadgeBackgroundColor(*toAPI(details), *toAPI(callback)), 0);
}

void BrowserAction::enable(double tabId)
{
    m_chrome.connection().send(Messages::BrowserActionHost::Enable(API::Object::create<API::Number>(tabId)), 0);
}

void BrowserAction::disable(double tabId)
{
    m_chrome.connection().send(Messages::BrowserActionHost::Disable(API::Object::create<API::Number>(tabId)), 0);
}

}
}
