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
#include "JSValue.h"

#include "Array.h"
#include "Dictionary.h"
#include "Function.h"
#include "JSArray.h"
#include "JSDictionary.h"
#include "Number.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {

static std::string stringFromJSString(JSStringRef jsString)
{
    size_t maxBufferSize = JSStringGetMaximumUTF8CStringSize(jsString);
    char* utf8Buffer = new char[maxBufferSize];
    size_t bytesWrittenToUTF8Buffer = JSStringGetUTF8CString(jsString, utf8Buffer, maxBufferSize);
    std::string stdString(utf8Buffer, bytesWrittenToUTF8Buffer - 1); // bytesWrittenToUTF8Buffer includes a trailing \0 which std::string doesn't need.
    delete[] utf8Buffer;
    return stdString;
}

JSValueRef toJSBoolean(JSContextRef context, API::Number* value)
{
    return JSValueMakeBoolean(context, value->booleanValue());
}

JSValueRef toJSNumber(JSContextRef context, API::Number* value)
{
    return JSValueMakeNumber(context, value->doubleValue());
}

static JSValueRef toJSString(JSContextRef context, API::APIString* value)
{
    std::string stringBuffer = value->utf8();
    JSRetainPtr<JSStringRef> string(Adopt, JSStringCreateWithUTF8CString(stringBuffer.data()));
    return JSValueMakeString(context, string.get());
}

static JSValueRef toJSArray(JSContextRef context, API::Array* value)
{
    JSObjectRef object = JSObjectMake(context, 0, 0);
    const size_t count = value->size();
    for (size_t index = 0; index < count; ++index) {
        RefPtr<JSValue> itemAtIndex = JSValue::create(context, value->copyItemAtIndex(index));
        JSObjectSetPropertyAtIndex(context, object, index, toJS(context, itemAtIndex.get()), 0);
    }

    return object;
}

static JSValueRef toJSDictionary(JSContextRef context, API::Dictionary* value)
{
    JSObjectRef object = JSObjectMake(context, 0, 0);
    const size_t count = value->size();
    Vector<Ref<APIString>> keys = value->copyKeys();
    for (size_t index = 0; index < count; ++index) {
        const APIString& key = keys[index];
        JSRetainPtr<JSStringRef> propertyName(Adopt, JSStringCreateWithUTF8CString(key.utf8().data()));
        RefPtr<JSValue> itemAtIndex = JSValue::create(context, value->itemForKey(key));
        JSObjectSetProperty(context, object, propertyName.get(), itemAtIndex->value(), 0, 0);
    }

    return object;
}

static JSValueRef toJSFunction(JSContextRef context, API::Function* value)
{
    return JSValueMakeUndefined(context); // TODO: Not implemented yet.
}

static JSValueRef toJS(JSContextRef context, API::Object* value)
{
    if (!value)
        return JSValueMakeUndefined(context);

    if (value->typeID() == API::APITypeTraits<bool>::typeID())
        return toJSBoolean(context, static_cast<API::Number*>(value));
    else if (value->typeID() == API::APITypeTraits<double>::typeID())
        return toJSNumber(context, static_cast<API::Number*>(value));
    else if (value->typeID() == API::APITypeTraits<uint64_t>::typeID())
        return toJSNumber(context, static_cast<API::Number*>(value));
    else if (value->typeID() == API::APITypeTraits<std::string>::typeID())
        return toJSString(context, static_cast<API::APIString*>(value));
    else if (value->typeID() == API::APITypeTraits<API::ObjectArray>::typeID())
        return toJSArray(context, static_cast<API::Array*>(value));
    else if (value->typeID() == API::APITypeTraits<API::ObjectDictionary>::typeID())
        return toJSDictionary(context, static_cast<API::Dictionary*>(value));

    ASSERT_NOT_REACHED();
    return JSValueMakeUndefined(context);
}

Ref<JSValue> JSValue::create(JSContextRef context, RefPtr<API::Object> value)
{
    return adoptRef(*new JSValue(context, toJS(context, value.get())));
}

bool JSValue::booleanValue() const
{
    return JSValueToBoolean(context(), value());
}

double JSValue::numberValue() const
{
    return JSValueToNumber(context(), value(), 0);
}

std::string JSValue::stringValue() const
{
    JSRetainPtr<JSStringRef> string(Adopt, JSValueToStringCopy(context(), value(), 0));
    return stringFromJSString(string.get());
}

size_t JSValue::length() const
{
    JSRetainPtr<JSStringRef> lengthString(Adopt, JSStringCreateWithUTF8CString("length"));
    return static_cast<size_t>(JSValueToNumber(context(), JSObjectGetProperty(context(), object(), lengthString.get(), 0), 0));
}

RefPtr<JSValue> JSValue::valueAtIndex(size_t index) const
{
    return JSValue::create(context(), JSObjectGetPropertyAtIndex(context(), object(), index, 0));
}

Vector<RefPtr<JSValue>> JSValue::values() const
{
    const size_t count = length();
    if (count == 0)
        return { };

    Vector<RefPtr<JSValue>> data;
    for (size_t index = 0; index < count; ++index)
        data.append(valueAtIndex(index));

    return WTF::move(data);
}

RefPtr<JSDictionary> JSValue::toDictionary(RefPtr<JSArray> values)
{
    if (!values || !isArray())
        return JSDictionary::create(context(), JSValueMakeUndefined(context()));

    JSObjectRef object = JSObjectMake(context(), 0, 0);
    const size_t count = length();
    for (size_t index = 0; index < count; ++index) {
        const std::string key = valueAtIndex(index)->stringValue();
        JSRetainPtr<JSStringRef> propertyName(Adopt, JSStringCreateWithUTF8CString(key.data()));
        JSObjectSetProperty(context(), object, propertyName.get(), values->valueAtIndex(index)->value(), 0, 0);
    }

    return JSDictionary::create(context(), object);
}

void JSValue::set(const std::string& key, RefPtr<JSValue> value) const
{
    set(key, value->value());
}

void JSValue::set(const std::string& key, bool value) const
{
    set(key, JSValueMakeBoolean(context(), value));
}

void JSValue::set(const std::string& key, double value) const
{
    set(key, JSValueMakeNumber(context(), value));
}

void JSValue::set(const std::string& key, uint64_t value) const
{
    set(key, JSValueMakeNumber(context(), static_cast<double>(value)));
}

void JSValue::set(const std::string& key, JSValueRef value) const
{
    JSRetainPtr<JSStringRef> propertyName(Adopt, JSStringCreateWithUTF8CString(key.data()));
    JSObjectSetProperty(context(), object(), propertyName.get(), value, 0, 0);
}

HashMap<std::string, RefPtr<JSValue>> JSValue::keyValues() const
{
    JSObjectRef object = this->object();
    JSPropertyNameArrayRef propertyNameArray = JSObjectCopyPropertyNames(context(), object);
    size_t count = JSPropertyNameArrayGetCount(propertyNameArray);

    HashMap<std::string, RefPtr<JSValue>> data;
    for (size_t index = 0; index < count; ++index) {
        JSStringRef propertyName = JSPropertyNameArrayGetNameAtIndex(propertyNameArray, index);
        data.add(stringFromJSString(propertyName), JSValue::create(context(), JSObjectGetProperty(context(), object, propertyName, 0)));
    }

    JSPropertyNameArrayRelease(propertyNameArray);

    return WTF::move(data);
}

RefPtr<JSValue> JSValue::call(const Vector<RefPtr<JSValue>>& arguments) const
{
    Vector<JSValueRef> argumentValues;
    for (auto& argument : arguments) {
        ASSERT(context() == argument->context());
        argumentValues.append(argument->value());
    }
    return JSValue::create(context(), call(argumentValues));
}

JSValueRef JSValue::call(const Vector<JSValueRef>& arguments) const
{
    return JSObjectCallAsFunction(context(), object(), 0, arguments.size(), arguments.data(), 0);
}

RefPtr<API::Object> JSValue::toAPI() const
{
    if (isBoolean() || isNumber())
        return toAPINumber();
    else if (isString())
        return toAPIString();
    else if (isArray())
        return toAPIArray();
    else if (isFunction())
        return toAPIFunction();
    else if (isObject())
        return toAPIDictionary();
    else if (isUndefined())
        return nullptr;

    ASSERT_NOT_REACHED();
    return nullptr;
}

RefPtr<API::Number> JSValue::toAPINumber() const
{
    return (isBoolean()) ? API::Object::create<API::Number>(booleanValue()) : API::Object::create<API::Number>(numberValue());
}

RefPtr<API::APIString> JSValue::toAPIString() const
{
    return API::Object::create<APIString>(stringValue());
}

RefPtr<API::Array> JSValue::toAPIArray() const
{
    Vector<RefPtr<JSValue>> values = this->values();
    API::ObjectArray data;
    for (size_t index = 0; index < values.size(); ++index)
        data.append(values[index]->toAPI());

    return API::Object::create<API::Array>(data);
}

RefPtr<API::Dictionary> JSValue::toAPIDictionary() const
{
    HashMap<std::string, RefPtr<JSValue>> keyValues = this->keyValues();
    API::ObjectDictionary data;
    for (const auto &keyValue : keyValues)
        data.add(keyValue.key, keyValue.value->toAPI());

    return API::Object::create<API::Dictionary>(data);
}

RefPtr<API::Function> JSValue::toAPIFunction() const
{
    RefPtr<JSValue> protect(const_cast<JSValue*>(this));
    return API::Object::create<API::Function>([=] (const API::Array& argumentsArray) -> RefPtr<API::Array> {
        const size_t argumentCount = argumentsArray.size();
        Vector<JSValueRef> arguments;
        for (size_t index = 0; index < argumentCount; ++index) {
            RefPtr<JSValue> argument = JSValue::create(protect->context(), argumentsArray.copyItemAtIndex(index));
            arguments.append(toJS(protect->context(), argument.get()));
        }

        JSValueRef result = protect->call(arguments);
        bool resultIsNullOrUndefined = JSValueIsNull(protect->context(), result) || JSValueIsUndefined(protect->context(), result);
        RefPtr<JSArray> resultArray = resultIsNullOrUndefined ? JSArray::create(protect->context(), 0, 0) : JSArray::create(protect->context(), &result, 1);
        return resultArray->toAPIArray();
    });
}

}
