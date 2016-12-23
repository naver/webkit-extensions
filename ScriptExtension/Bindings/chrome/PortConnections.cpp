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
#include "PortConnections.h"

#include "InjectedBundlePageProxy.h"
#include "PortMessages.h"

namespace Extensions {
namespace chrome {

static void generatePortIDPair(uint64_t* port1, uint64_t* port2)
{
    static uint64_t portIDPair;
    *port1 = ++portIDPair;
    *port2 = ++portIDPair;
}

static uint64_t oppositePortID(uint64_t portID)
{
    return portID % 2 ? portID + 1 : portID - 1;
}

const char* PortConnections::supplementName()
{
    return "PortConnections";
}

PortConnections::PortConnections()
{
}

std::pair<uint64_t, uint64_t> PortConnections::connect(InjectedBundlePageProxy* server, InjectedBundlePageProxy* client)
{
    uint64_t serverPortID = 0;
    uint64_t clientPortID = 0;
    generatePortIDPair(&serverPortID, &clientPortID);

    m_pageMap.add(serverPortID, client);
    m_pageMap.add(clientPortID, server);

    return std::make_pair(serverPortID, clientPortID);
}

void PortConnections::postMessage(uint64_t portID, const API::Dictionary& msg)
{
    if (InjectedBundlePageProxy* proxy = m_pageMap.get(portID))
        proxy->connection().send(Messages::Port::DispatchOnMessage(msg), oppositePortID(portID));
}

void PortConnections::disconnect(uint64_t portID)
{
    m_pageMap.remove(portID);
    m_pageMap.remove(oppositePortID(portID));
}

}
}
