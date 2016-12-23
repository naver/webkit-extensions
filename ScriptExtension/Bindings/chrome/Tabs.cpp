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
#include "Tabs.h"

#include "Chrome.h"
#include "InjectedBundlePage.h"
#include "JSTabs.h"
#include "TabsHostMessages.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

Tabs::Tabs(Chrome& chrome)
    : m_chrome(chrome)
{
}

JSObjectRef Tabs::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSTabs::tabsClass(), this);
}

void Tabs::get(double tabId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Get(API::Object::create<API::Number>(tabId), *toAPI(callback)), 0);
}

void Tabs::getCurrent(RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::GetCurrent(*toAPI(callback)), 0);
}

void Tabs::connect(double tabId, RefPtr<JSDictionary> connectInfo)
{
    m_chrome.connection().send(Messages::TabsHost::Connect(API::Object::create<API::Number>(tabId), *toAPI(connectInfo)), 0);
}

void Tabs::sendRequest(double tabId, RefPtr<JSDictionary> request, RefPtr<JSFunction> responseCallback)
{
    // TODO : m_runtime.sendMessage(tabId, request, responseCallback);
}

void Tabs::sendMessage(double tabId, RefPtr<JSDictionary> message, RefPtr<JSDictionary> options, RefPtr<JSFunction> responseCallback)
{
    // TODO : m_runtime.sendMessage(tabId, request, responseCallback);
}

void Tabs::getSelected(double windowId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::GetSelected(API::Object::create<API::Number>(windowId), *toAPI(callback)), 0);
}

void Tabs::getAllInWindow(double windowId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::GetAllInWindow(API::Object::create<API::Number>(windowId), *toAPI(callback)), 0);
}

void Tabs::create(RefPtr<JSDictionary> createProperties, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Create(*toAPI(createProperties), *toAPI(callback)), 0);
}

void Tabs::duplicate(double tabId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Duplicate(API::Object::create<API::Number>(tabId), *toAPI(callback)), 0);
}

void Tabs::query(RefPtr<JSDictionary> queryInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Query(*toAPI(queryInfo), *toAPI(callback)), 0);
}

void Tabs::highlight(RefPtr<JSDictionary> highlightInfo, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Highlight(*toAPI(highlightInfo), *toAPI(callback)), 0);
}

void Tabs::update(double tabId, RefPtr<JSDictionary> updateProperties, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Update(API::Object::create<API::Number>(tabId), *toAPI(updateProperties), *toAPI(callback)), 0);
}

void Tabs::move(RefPtr<JSArray> tabIds, RefPtr<JSDictionary> moveProperties, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Move(*toAPI(tabIds), *toAPI(moveProperties), *toAPI(callback)), 0);
}

void Tabs::reload(double tabId, RefPtr<JSDictionary> reloadProperties, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Reload(API::Object::create<API::Number>(tabId), *toAPI(reloadProperties), *toAPI(callback)), 0);
}

void Tabs::remove(RefPtr<JSArray> tabIds, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::Remove(*toAPI(tabIds), *toAPI(callback)), 0);
}

void Tabs::detectLanguage(RefPtr<JSArray> tabIds, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::DetectLanguage(*toAPI(tabIds), *toAPI(callback)), 0);
}

void Tabs::captureVisibleTab(double windowId, RefPtr<JSDictionary> options, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::CaptureVisibleTab(API::Object::create<API::Number>(windowId), *toAPI(options), *toAPI(callback)), 0);
}

void Tabs::executeScript(double tabId, RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::ExecuteScript(API::Object::create<API::Number>(tabId), *toAPI(details), *toAPI(callback)), 0);
}

void Tabs::insertCSS(double tabId, RefPtr<JSDictionary> details, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::InsertCSS(API::Object::create<API::Number>(tabId), *toAPI(details), *toAPI(callback)), 0);
}

void Tabs::setZoom(double tabId, double zoomFactor, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::SetZoom(API::Object::create<API::Number>(tabId), API::Object::create<API::Number>(zoomFactor), *toAPI(callback)), 0);
}

void Tabs::getZoom(double tabId, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::TabsHost::GetZoom(API::Object::create<API::Number>(tabId), *toAPI(callback)), 0);
}

}
}
