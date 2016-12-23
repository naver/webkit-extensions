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
#include "Event.h"

#include "EventFactory.h"
#include "InjectedBundlePage.h"
#include "JSEvent.h"
#include "ScriptExtension.h"

#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

Event::Event(EventFactory& factory, IPC::StringReference name, EventOptions options)
    : m_factory(factory)
    , m_name(name)
    , m_options(options)
{
}

JSObjectRef Event::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSEvent::eventClass(), this);
}

void Event::addListener(RefPtr<JSFunction> callback, RefPtr<JSDictionary> filter, RefPtr<JSArray> extraInfoSpec)
{
    if (!m_options.supportsListeners)
        ASSERT(0); // FIXME : throw new Error("This event does not support listeners.");

    if (!m_options.maxListeners)
        ASSERT(0); // FIXME : throw new Error("Too many listeners for " + this.eventName);

    if (filter->length()) {
        if (!m_options.supportsFilters)
            ASSERT(0); // FIXME : throw new Error("This event does not support filters.");
    }

    m_factory.addListener(m_name, callback, filter, extraInfoSpec);
}

void Event::removeListener(RefPtr<JSFunction> callback)
{
    if (callback != nullptr)
        callback->call(Vector<RefPtr<JSValue>>());
}

}
}
