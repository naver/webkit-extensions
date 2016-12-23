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

#ifndef Chrome_EventProxyFactory_h
#define Chrome_EventProxyFactory_h

#include "Dictionary.h"
#include "EventProxy.h"
#include "Function.h"
#include "MessageReceiver.h"
#include "StringReference.h"

#include <wtf/HashMap.h>

namespace Extensions {

class InjectedBundlePageProxy;

namespace chrome {

class ChromeHost;

class EventProxyFactory : public IPC::MessageReceiver {
public:
    EventProxyFactory(ChromeHost&);
    ~EventProxyFactory();

    InjectedBundlePageProxy& page() const;

    EventProxy& createEvent(IPC::StringReference eventName, EventOptions options = {true, false, std::numeric_limits<uint64_t>::max()});
    void scriptMessage(RefPtr<APIString> eventName, RefPtr<API::Array> arguments);

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void addListener(const APIString& eventName, const API::Function& callback, const API::Dictionary& filter, const API::Array& extraInfoSpec);

    ChromeHost& m_chrome;

    typedef HashMap<IPC::StringReference, std::unique_ptr<EventProxy>> EventMap;
    EventMap m_events;
};

}
}

#endif // Chrome_EventProxyFactory_h
