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
#include "WebRequestEventProxy.h"

#include "EventProxyFactory.h"
#include "EventOptions.h"
#include "InjectedBundlePageProxy.h"
#include "RequestFilter.h"

namespace Extensions {
namespace chrome {

WebRequestEventProxy::WebRequestEventProxy(EventProxyFactory& factory, EventOptions options)
    : EventProxy(factory, options)
{
}

WebRequestEventProxy::~WebRequestEventProxy()
{
}

void WebRequestEventProxy::addListener(const API::Function& callback, const API::Dictionary& filter, const API::Array& extraInfoSpec)
{
    append(std::make_unique<Listener>(API::Object::create<API::Function>(callback), std::make_unique<RequestFilter>(filter)));
    m_extraInfoSpec = API::Object::create<API::Array>(extraInfoSpec.typeImpl());
}

void WebRequestEventProxy::triggerEvent(RefPtr<API::Array> arguments)
{
    // FIXME
}

}
}
