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

#ifndef Chrome_EventProxy_h
#define Chrome_EventProxy_h

#include "EventOptions.h"
#include "Listener.h"

#include <wtf/Vector.h>

namespace API {
class Array;
class Dictionary;
class Function;
}

namespace Extensions {
namespace chrome {

class EventProxyFactory;

class EventProxy {
public:
    EventProxy(EventProxyFactory&, EventOptions);
    virtual ~EventProxy();

    virtual void addListener(const API::Function& callback, const API::Dictionary& filter, const API::Array& extraInfoSpec);
    virtual void triggerEvent(RefPtr<API::Array> arguments);

    void append(std::unique_ptr<Listener> listener);

private:
    EventProxyFactory& m_factory;
    EventOptions m_options;
    Vector<std::unique_ptr<Listener>> m_listeners;
};

}
}

#endif // Chrome_EventProxy_h
