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
#include "JSWrappable.h"

#include <JavaScriptCore/JSContextRefPrivate.h>
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {

void JSWrappable::inject(JSContextRef context, JSObjectRef window, const std::string& propertyName)
{
    setStaticProperty(context, window, wrap(context), propertyName);
}

void JSWrappable::setStaticProperty(JSContextRef context, JSObjectRef object, JSObjectRef property, const std::string& propertyName)
{
    JSRetainPtr<JSStringRef> propertyNameString(Adopt, JSStringCreateWithUTF8CString(propertyName.c_str()));
    JSObjectSetProperty(context, object, propertyNameString.get(), property, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, NULL);
}

JSWrappable* JSWrappable::unwrap(JSContextRef context, JSValueRef value)
{
    ASSERT_ARG(context, context);
    ASSERT_ARG(value, value);
    if (!context || !value)
        return 0;
    return unwrap(JSValueToObject(context, value, 0));
}

JSWrappable* JSWrappable::unwrap(JSObjectRef object)
{
    JSWrappable* wrappable = static_cast<JSWrappable*>(JSObjectGetPrivate(object));
    ASSERT(wrappable);
    return wrappable;
}

void JSWrappable::initialize(JSContextRef ctx, JSObjectRef object)
{
    JSWrappable* wrappable = unwrap(object);
    if (!wrappable)
        return;
    wrappable->ref();
}

void JSWrappable::finalize(JSObjectRef object)
{
    JSWrappable* wrappable = unwrap(object);
    if (!wrappable)
        return;
    wrappable->deref();
}

} // namespace Extensions
