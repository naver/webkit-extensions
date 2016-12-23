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
#include "ChromeWindows.h"

#include "Chrome.h"
#include "ChromeWindowsHostMessages.h"
#include "InjectedBundlePage.h"
#include "JSChromeWindows.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

ChromeWindows::ChromeWindows(Chrome& chrome)
    : m_chrome(chrome)
{
}

JSObjectRef ChromeWindows::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSChromeWindows::chromeWindowsClass(), this);
}

void ChromeWindows::get(double windowId, RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::Get(API::Object::create<API::Number>(windowId), *toAPI(getInfo), *toAPI(callback)), 0);
}

void ChromeWindows::getCurrent(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::GetCurrent(*toAPI(getInfo), *toAPI(callback)), 0);
}

void ChromeWindows::getLastFocused(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::GetLastFocused(*toAPI(getInfo), *toAPI(callback)), 0);
}

void ChromeWindows::getAll(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::GetAll(*toAPI(getInfo), *toAPI(callback)), 0);
}

void ChromeWindows::create(RefPtr<JSDictionary> createData, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::Create(*toAPI(createData), *toAPI(callback)), 0);
}

void ChromeWindows::update(double windowId, RefPtr<JSDictionary> updateInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::Update(API::Object::create<API::Number>(windowId), *toAPI(updateInfo), *toAPI(callback)), 0);
}

void ChromeWindows::remove(double windowId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::ChromeWindowsHost::Remove(API::Object::create<API::Number>(windowId), *toAPI(callback)), 0);
}

}
}
