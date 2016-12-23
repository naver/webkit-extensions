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

#ifndef JSWrappable_h
#define JSWrappable_h

#include "JSArray.h"
#include "JSDictionary.h"
#include "JSFunction.h"
#include "JSString.h"
#include <wtf/RefCounted.h>

namespace Extensions {

class JSWrappable : public RefCounted<JSWrappable> {
public:
    virtual ~JSWrappable() = default;
    virtual JSObjectRef wrap(JSContextRef) = 0;
    void inject(JSContextRef, JSObjectRef, const std::string&);

    static void setStaticProperty(JSContextRef, JSObjectRef, JSObjectRef property, const std::string& propertyName);

    static JSWrappable* unwrap(JSObjectRef);
    static JSWrappable* unwrap(JSContextRef, JSValueRef);
    static void initialize(JSContextRef, JSObjectRef);
    static void finalize(JSObjectRef);
};

inline JSValueRef toJS(JSContextRef context, JSWrappable* impl)
{
    return impl ? impl->wrap(context) : JSValueMakeUndefined(context);
}

} // namespace Extensions

#endif // JSWrappable_h
