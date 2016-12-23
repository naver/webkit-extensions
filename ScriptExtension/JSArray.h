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

#ifndef JSArray_h
#define JSArray_h

#include "Array.h"
#include "JSValue.h"

namespace Extensions {

class JSArray final : public JSValue {
public:
    static Ref<JSArray> create(JSContextRef, JSValueRef);
    static Ref<JSArray> create(JSContextRef, const JSValueRef[], size_t);
    ~JSArray() = default;

private:
    JSArray(JSContextRef, JSValueRef);
};

inline Ref<JSArray> JSArray::create(JSContextRef context, JSValueRef value)
{
    return adoptRef(*new JSArray(context, value));
}

inline Ref<JSArray> JSArray::create(JSContextRef context, const JSValueRef values[], size_t valuesCount)
{
    return adoptRef(*new JSArray(context, JSObjectMakeArray(context, valuesCount, values, NULL)));
}

inline JSArray::JSArray(JSContextRef context, JSValueRef value)
    : JSValue(context, value)
{
}

inline RefPtr<API::Array> toAPI(RefPtr<JSArray> value)
{
    return value->toAPIArray();
}

}

#endif // JSArray_h
