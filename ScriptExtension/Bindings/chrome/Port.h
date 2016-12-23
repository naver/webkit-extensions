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

#ifndef Chrome_Port_h
#define Chrome_Port_h

#include "JSWrappable.h"
#include "MessageSender.h"

namespace Extensions {
namespace chrome {

class Chrome;
class Event;

class Port : public JSWrappable, public IPC::MessageReceiver {
public:
    Port(Chrome& chrome, uint64_t portID, JSContextRef context, const API::Dictionary& connectInfo);
    virtual ~Port();

    JSObjectRef wrap(JSContextRef) override;

    RefPtr<JSString> name() { return m_name; }
    void setName(RefPtr<JSString> name) { m_name = name; }
    RefPtr<MessageSender> sender() { return m_sender; }

    Event& onDisconnect() const { return const_cast<Event&>(m_onDisconnect.get()); }
    Event& onMessage() const { return const_cast<Event&>(m_onMessage.get()); }

    void disconnect();
    void postMessage(RefPtr<JSDictionary> msg);

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void dispatchOnMessage(const API::Dictionary& msg);

    Chrome& m_chrome;
    uint64_t m_portID;
    RefPtr<JSString> m_name;
    RefPtr<MessageSender> m_sender;
    Ref<Event> m_onDisconnect;
    Ref<Event> m_onMessage;
};

}
}

#endif // Chrome_Port_h
