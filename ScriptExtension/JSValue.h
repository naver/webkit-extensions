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

#ifndef JSValue_h
#define JSValue_h

#include "JSRetainValue.h"
#include "Object.h"
#include "StringHash.h"
#include <string>
#include <wtf/HashMap.h>
#include <wtf/Ref.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/Vector.h>

namespace API {
class Array;
class APIString;
class Dictionary;
class Function;
class Number;
}

namespace Extensions {

class JSArray;
class JSDictionary;

class JSValue : public RefCounted<JSValue> {
public:
    static Ref<JSValue> create(JSContextRef, JSValueRef);
    static Ref<JSValue> create(JSContextRef, RefPtr<API::Object>);
    virtual ~JSValue() = default;

    JSValueRef value() const { return m_protect.get(); }
    JSGlobalContextRef context() const { return m_protect.context(); }
    JSObjectRef object() const { return m_protect.object(); }

    bool isNull() const;
    bool isUndefined() const;
    bool isBoolean() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;
    bool isFunction() const;

    bool booleanValue() const;
    double numberValue() const;
    std::string stringValue() const;

    size_t length() const;
    RefPtr<JSValue> valueAtIndex(size_t) const;
    Vector<RefPtr<JSValue>> values() const;

    RefPtr<JSDictionary> toDictionary(RefPtr<JSArray> values);

    void set(const std::string&, RefPtr<JSValue>) const;
    void set(const std::string&, bool) const;
    void set(const std::string&, double) const;
    void set(const std::string&, uint64_t) const;
    void set(const std::string&, JSValueRef) const;

    HashMap<std::string, RefPtr<JSValue>> keyValues() const;

    RefPtr<JSValue> call(const Vector<RefPtr<JSValue>>&) const;
    JSValueRef call(const Vector<JSValueRef>&) const;

    virtual RefPtr<API::Object> toAPI() const;
    RefPtr<API::Number> toAPINumber() const;
    RefPtr<API::APIString> toAPIString() const;
    RefPtr<API::Array> toAPIArray() const;
    RefPtr<API::Dictionary> toAPIDictionary() const;
    RefPtr<API::Function> toAPIFunction() const;

protected:
    JSValue(JSContextRef, JSValueRef);

private:
    JSRetainValue m_protect;
};

inline Ref<JSValue> JSValue::create(JSContextRef context, JSValueRef value)
{
    return adoptRef(*new JSValue(context, value));
}

inline JSValue::JSValue(JSContextRef context, JSValueRef value)
    : m_protect(JSContextGetGlobalContext(context), value)
{
}

inline bool JSValue::isNull() const
{
    return JSValueIsNull(context(), value());
}

inline bool JSValue::isUndefined() const
{
    return JSValueIsUndefined(context(), value());
}

inline bool JSValue::isBoolean() const
{
    return JSValueIsBoolean(context(), value());
}

inline bool JSValue::isNumber() const
{
    return JSValueIsNumber(context(), value());
}

inline bool JSValue::isString() const
{
    return JSValueIsString(context(), value());
}

inline bool JSValue::isArray() const
{
    return JSValueIsArray(context(), value());
}

inline bool JSValue::isObject() const
{
    return JSValueIsObject(context(), value());
}

inline bool JSValue::isFunction() const
{
    return JSObjectIsFunction(context(), object());
}

inline RefPtr<API::Object> toAPI(RefPtr<JSValue> value)
{
    return value->toAPI();
}

inline JSValueRef toJS(JSContextRef context, JSValue* value)
{
    return (value) ? value->value() : JSValueMakeUndefined(context);
}

}

#endif // JSValue_h
