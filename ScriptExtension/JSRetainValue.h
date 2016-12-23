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

#ifndef JSRetainValue_h
#define JSRetainValue_h

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSValueRef.h>
#include <algorithm>

class JSRetainValue {
public:
    JSRetainValue() : m_value(0) { }
    JSRetainValue(JSGlobalContextRef context, JSValueRef value);
    JSRetainValue(const JSRetainValue&);
    ~JSRetainValue();
    
    JSValueRef get() const { return m_value; }

    JSGlobalContextRef context() const { return m_context; }
    JSObjectRef object() const;
    
    void clear();

    bool operator!() const { return !m_value; }
    explicit operator bool() const { return m_value; }

    JSRetainValue& operator=(const JSRetainValue&);

    void swap(JSRetainValue&);

private:
    JSGlobalContextRef m_context;
    JSValueRef m_value;
};

inline void JSRetain(JSGlobalContextRef ctx, JSValueRef value) { JSGlobalContextRetain(ctx); JSValueProtect(ctx, value); }
inline void JSRelease(JSGlobalContextRef ctx, JSValueRef value) { JSGlobalContextRelease(ctx); JSValueUnprotect(ctx, value); }

inline JSRetainValue::JSRetainValue(JSGlobalContextRef context, JSValueRef value)
    : m_context(context)
    , m_value(value)
{
    if (m_value)
        JSRetain(m_context, m_value);
}

inline JSRetainValue::JSRetainValue(const JSRetainValue& o)
    : m_context(o.m_context)
    , m_value(o.m_value)
{
    if (m_value)
        JSRetain(m_context, m_value);
}

inline JSRetainValue::~JSRetainValue()
{
    if (m_value)
        JSRelease(m_context, m_value);
}

inline JSObjectRef JSRetainValue::object() const
{
    if (!JSValueIsObject(m_context, m_value))
        return JSObjectMakeError(m_context, 0, 0, 0);

    return JSValueToObject(m_context, m_value, 0);
}

inline void JSRetainValue::clear()
{
    if (JSValueRef value = m_value) {
        JSGlobalContextRef ctx = m_context;
        m_context = 0;
        m_value = 0;
        JSRelease(ctx, m_value);
    }
}

inline JSRetainValue& JSRetainValue::operator=(const JSRetainValue& o)
{
    JSGlobalContextRef octx = o.context();
    JSValueRef optr = o.get();
    if (optr)
        JSRetain(octx, optr);
    JSGlobalContextRef ctx = m_context;
    JSValueRef value = m_value;
    m_context = octx;
    m_value = optr;
    if (value)
        JSRelease(ctx, value);
    return *this;
}

inline void JSRetainValue::swap(JSRetainValue& o)
{
    std::swap(m_value, o.m_value);
}

inline void swap(JSRetainValue& a, JSRetainValue& b)
{
    a.swap(b);
}

template<typename JSValueRef, typename U> inline bool operator==(const JSRetainValue& a, const JSRetainValue& b)
{ 
    return a.context() == b.context() && JSValueIsStrictEqual(a.context(), a.get(), b.get()); 
}

#endif // JSRetainValue_h
