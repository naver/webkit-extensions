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
#include "Port.h"

#include "Chrome.h"
#include "Event.h"
#include "EventFactory.h"
#include "InjectedBundlePage.h"
#include "JSPort.h"
#include "PortMessages.h"
#include "PortHostMessages.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

Port::Port(Chrome& chrome, uint64_t portID, JSContextRef context, const API::Dictionary& connectInfo)
    : m_chrome(chrome)
    , m_portID(portID)
    , m_name(nullptr)
    , m_sender(nullptr)
    , m_onDisconnect(m_chrome.eventFactory().createEvent("Port.onDisconnect"))
    , m_onMessage(m_chrome.eventFactory().createEvent("Port.onMessage"))
{
    RefPtr<APIString> portNameKey = API::Object::create<APIString>("name");
    RefPtr<APIString> portNameValue;
    if (connectInfo.containsKey(*portNameKey))
        portNameValue = API::Object::create<APIString>(connectInfo.itemForKey(*portNameKey)->impl());
    else
        portNameValue = API::Object::create<APIString>("");
    setName(JSString::create(context, JSValue::create(context, portNameValue)->value()));

    m_chrome.extension().page().addMessageReceiver(Messages::Port::messageReceiverName(), m_portID, *this);
}

Port::~Port()
{
    m_chrome.extension().page().removeMessageReceiver(Messages::Port::messageReceiverName(), m_portID);
}

JSObjectRef Port::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSPort::portClass(), this);
}

void Port::disconnect()
{
}

void Port::postMessage(RefPtr<JSDictionary> msg)
{
    m_chrome.connection().send(Messages::PortHost::PostMessage(m_portID, *toAPI(msg)), 0);
}

void Port::dispatchOnMessage(const API::Dictionary& msg)
{
}

}
}

