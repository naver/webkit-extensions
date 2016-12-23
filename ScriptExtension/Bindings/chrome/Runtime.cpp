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
#include "Runtime.h"

#include "Chrome.h"
#include "Event.h"
#include "EventFactory.h"
#include "InjectedBundlePage.h"
#include "JSRuntime.h"
#include "Port.h"
#include "RuntimeHostMessages.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

Runtime::Runtime(Chrome& chrome)
    : m_chrome(chrome)
    , m_id(nullptr)
    , m_lastError(nullptr)
    , m_onConnect(m_chrome.eventFactory().createEvent("RuntimeOnConnect"))
    , m_onMessage(m_chrome.eventFactory().createEvent("RuntimeOnMessage"))
{
}

JSObjectRef Runtime::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSRuntime::runtimeClass(), this);
}

RefPtr<Port> Runtime::connect(RefPtr<JSString> extensionID, RefPtr<JSDictionary> connectInfo)
{
    ASSERT(extensionID->isNull() || extensionID->isString());

    uint64_t portID = 0;
    m_chrome.connection().sendSync(Messages::RuntimeHost::Connect(*toAPI(extensionID), *toAPI(connectInfo)),
        Messages::RuntimeHost::Connect::Reply(portID), 0);
    return adoptRef(*new Port(m_chrome, portID, connectInfo->context(), *toAPI(connectInfo)));
}

RefPtr<JSDictionary> Runtime::getManifest()
{
    return nullptr;
}

RefPtr<JSString> Runtime::getURL(RefPtr<JSString> path)
{
    return nullptr;
}

void Runtime::reload()
{
}

void Runtime::sendMessage(RefPtr<JSString> extensionID, RefPtr<JSDictionary> message, RefPtr<JSDictionary> options, RefPtr<JSFunction> responseCallback)
{
}

void Runtime::dispatchOnConnect(uint64_t portID, const APIString& extensionID, const API::Dictionary& connectInfo)
{
}

}
}
