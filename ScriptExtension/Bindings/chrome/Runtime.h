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

#ifndef Chrome_Runtime_h
#define Chrome_Runtime_h

#include "JSWrappable.h"
#include "MessageReceiver.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {
namespace chrome {

class Chrome;
class Event;
class Port;

class Runtime : public JSWrappable, public IPC::MessageReceiver {
public:
    Runtime(Chrome&);

    JSObjectRef wrap(JSContextRef) override;

    RefPtr<JSString> id() { return m_id; }
    RefPtr<JSString> lastError() { return m_lastError; }

    Event& onConnect() const { return const_cast<Event&>(m_onConnect.get()); }
    Event& onMessage() const { return const_cast<Event&>(m_onMessage.get()); }

    RefPtr<Port> connect(RefPtr<JSString> extensionID, RefPtr<JSDictionary> connectInfo);
    RefPtr<JSDictionary> getManifest();
    RefPtr<JSString> getURL(RefPtr<JSString> path);
    void reload();
    void sendMessage(RefPtr<JSString> extensionID, RefPtr<JSDictionary> message, RefPtr<JSDictionary> options, RefPtr<JSFunction> responseCallback);

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void dispatchOnConnect(uint64_t portID, const APIString& extensionID, const API::Dictionary& connectInfo);

    Chrome& m_chrome;
    RefPtr<JSString> m_id;
    RefPtr<JSString> m_lastError;
    Ref<Event> m_onConnect;
    Ref<Event> m_onMessage;
};

}
}

#endif // Chrome_Runtime_h
