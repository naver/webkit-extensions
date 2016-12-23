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
#include "EventProxyFactory.h"

#include "APIString.h"
#include "ChromeHost.h"
#include "ChromeStrings.h"
#include "EventProxyFactoryMessages.h"
#include "InjectedBundlePageProxy.h"
#include "ScriptExtensionHost.h"
#include "WebRequestEventProxy.h"

namespace Extensions {
namespace chrome {

static bool isWebRequestEvent(IPC::StringReference eventname)
{
    bool result = false;

    if (std::strncmp(eventname.data(), "WebRequest", std::strlen("WebRequest")) == 0)
        result = true;
    return result;
}

EventProxyFactory::EventProxyFactory(ChromeHost& chrome)
    : m_chrome(chrome)
{
    m_chrome.extension().page().addMessageReceiver(Messages::EventProxyFactory::messageReceiverName(), *this);
}

EventProxyFactory::~EventProxyFactory()
{
    m_chrome.extension().page().removeMessageReceiver(Messages::EventProxyFactory::messageReceiverName());
}

InjectedBundlePageProxy& EventProxyFactory::page() const
{
    return m_chrome.extension().page();
}

EventProxy& EventProxyFactory::createEvent(IPC::StringReference eventName, EventOptions options)
{
    std::unique_ptr<EventProxy> event;

    if (isWebRequestEvent(eventName))
        event = std::make_unique<WebRequestEventProxy>(*this, options);
    else
        event = std::make_unique<EventProxy>(*this, options);

    EventProxy& entry = *event;
    m_events.add(eventName, std::move(event));
    return entry;
}

void EventProxyFactory::scriptMessage(RefPtr<APIString> eventName, RefPtr<API::Array> arguments)
{
    if (!eventName)
        return;

    const std::string utf8EventName = eventName->utf8();
    if (EventProxy* event = m_events.get(IPC::StringReference(utf8EventName.c_str(), utf8EventName.size())))
        event->triggerEvent(WTF::move(arguments));
}

void EventProxyFactory::addListener(const APIString& eventName, const API::Function& callback, const API::Dictionary& filter, const API::Array& extraInfoSpec)
{
    const std::string utf8EventName = eventName.utf8();
    if (EventProxy* event = m_events.get(IPC::StringReference(utf8EventName.c_str(), utf8EventName.size())))
        event->addListener(callback, filter, extraInfoSpec);
}

}
}
