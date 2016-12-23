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
#include "ScriptPrototype.h"

#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {

ScriptPrototype::ScriptPrototype(Ref<API::Prototype> impl, ScriptExtension& extension)
    : m_impl(WTF::move(impl))
    , m_extension(extension)
{
    std::string className = m_impl->name();
    Vector<std::string> staticValues = m_impl->staticValues();
    Vector<JSStaticValue> staticValuesData;

    for (auto& staticValue : staticValues)
        staticValuesData.append({ staticValue.data(), getPropertyCallback, setPropertyCallback, kJSPropertyAttributeDontDelete });
    staticValuesData.append({ 0, 0, 0, 0 });

    JSClassDefinition definition = {
        0,
        kJSClassAttributeNone,
    
        className.c_str(),
        NULL,
    
        staticValuesData.data(),
        NULL,
    
        initializeCallback,
        finalizeCallback,
        hasPropertyCallback,
        getPropertyCallback,
        setPropertyCallback,
        deletePropertyCallback,
        getPropertyNamesCallback,
        callInstanceAsFunctionCallback,
        callAsConstructorCallback,
        hasInstanceCallback,
        convertToTypeCallback,
    };

    m_definition = JSClassCreate(&definition);
}

ScriptPrototype::~ScriptPrototype()
{
    JSClassRelease(m_definition);
}

JSObjectRef ScriptPrototype::wrap(JSContextRef context)
{
    JSObjectRef wrapperObject = JSObjectMake(context, m_definition, this);
    JSObjectRef prototypeObject = JSValueToObject(context, JSObjectGetPrototype(context, wrapperObject), NULL);

    Vector<std::string> staticFunctions = m_impl->staticFunctions();
    for (auto& staticFunction : staticFunctions) {
        JSObjectRef functionObject = JSObjectMakeFunctionWithCallback(context, NULL, callPropertyAsFunctionCallback);
        setStaticProperty(context, prototypeObject, functionObject, staticFunction);
        m_functions.add(functionObject, staticFunction);
    }

    return wrapperObject;
}

ScriptPrototype* ScriptPrototype::wrapper(JSObjectRef object)
{
    ScriptPrototype* thisObject = static_cast<ScriptPrototype*>(unwrap(object));
    ASSERT(thisObject);
    return thisObject;
}

void ScriptPrototype::initializeCallback(JSContextRef context, JSObjectRef object)
{
}

void ScriptPrototype::finalizeCallback(JSObjectRef object)
{
    // TODO: Send finalize message.
}

bool ScriptPrototype::hasPropertyCallback(JSContextRef context, JSObjectRef object, JSStringRef propertyName)
{
    return false;
}

JSValueRef ScriptPrototype::getPropertyCallback(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    return JSValueMakeUndefined(context);
}

bool ScriptPrototype::setPropertyCallback(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
{
    return false;
}

bool ScriptPrototype::deletePropertyCallback(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    return false;
}

void ScriptPrototype::getPropertyNamesCallback(JSContextRef context, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames)
{
}

JSValueRef ScriptPrototype::callInstanceAsFunctionCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    return JSValueMakeUndefined(context);
}

JSValueRef ScriptPrototype::callPropertyAsFunctionCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    return wrapper(thisObject)->callPropertyAsFunction(context, function, thisObject, argumentCount, arguments, exception);
}

JSObjectRef ScriptPrototype::callAsConstructorCallback(JSContextRef context, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    return JSObjectMakeError(context, 0, NULL, NULL);
}

bool ScriptPrototype::hasInstanceCallback(JSContextRef context, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception)
{
    return false;
}

JSValueRef ScriptPrototype::convertToTypeCallback(JSContextRef context, JSObjectRef object, ::JSType type, JSValueRef* exception)
{
    return JSValueMakeUndefined(context);
}

JSValueRef ScriptPrototype::callPropertyAsFunction(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    return JSValueMakeUndefined(context);
}

} // namespace Extensions
