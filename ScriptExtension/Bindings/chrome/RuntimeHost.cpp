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
#include "RuntimeHost.h"

#include "ChromeHost.h"
#include "ChromeHostMessages.h"
#include "ChromeStrings.h"
#include "EventProxyFactory.h"
#include "InjectedBundlePageProxy.h"
#include "InjectedBundleProxy.h"
#include "Number.h"
#include "PortConnections.h"
#include "RuntimeHostMessages.h"
#include "RuntimeMessages.h"
#include "ScriptExtensionHost.h"
#include "ScriptExtensionMessages.h"

namespace Extensions {
namespace chrome {

RuntimeHost::RuntimeHost(ChromeHost& chrome)
    : m_chrome(chrome)
    , m_connections(*m_chrome.extension().page().bundle().supplement<PortConnections>())
    , m_onConnect(m_chrome.eventFactory().createEvent("RuntimeOnConnect"))
    , m_onMessage(m_chrome.eventFactory().createEvent("RuntimeOnMessage"))
{
}

RuntimeHost::~RuntimeHost()
{
}

void RuntimeHost::connect(const APIString& extensionID, const API::Dictionary& connectInfo, uint64_t& portID)
{
    InjectedBundlePageProxy* server = &m_chrome.extension().page();
    InjectedBundlePageProxy* client = m_chrome.extension().page().bundle().page(0); // FIXME: Remove.
    std::pair<uint64_t, uint64_t> portIDs = m_connections.connect(server, client);

    portID = portIDs.first;

    client->scriptMessage(
        *ChromeStrings::chromeHost(),
        *ChromeStrings::scriptMessage(),
        API::Dictionary::create(
            ChromeStrings::chromeEvent() ^ ChromeStrings::runtimeOnConnect(),
            ChromeStrings::arguments() ^ API::Array::create(portIDs.second, connectInfo)
            )
        );
}

}
}
