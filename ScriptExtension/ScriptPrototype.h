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

#ifndef ScriptPrototype_h
#define ScriptPrototype_h

#include "Prototype.h"
#include "JSWrappable.h"
#include <string>
#include <wtf/HashMap.h>

namespace Extensions {

class ScriptExtension;

class ScriptPrototype final : public JSWrappable {
public:
    ScriptPrototype(Ref<API::Prototype>, ScriptExtension&);
    ~ScriptPrototype();

    JSObjectRef wrap(JSContextRef) override;

private:
    static ScriptPrototype* wrapper(JSObjectRef object);

    static void initializeCallback(JSContextRef, JSObjectRef);
    static void finalizeCallback(JSObjectRef);
    static bool hasPropertyCallback(JSContextRef, JSObjectRef, JSStringRef propertyName);
    static JSValueRef getPropertyCallback(JSContextRef, JSObjectRef, JSStringRef propertyName, JSValueRef* exception);
    static bool setPropertyCallback(JSContextRef, JSObjectRef, JSStringRef propertyName, JSValueRef value, JSValueRef* exception);
    static bool deletePropertyCallback(JSContextRef, JSObjectRef, JSStringRef propertyName, JSValueRef* exception);
    static void getPropertyNamesCallback(JSContextRef, JSObjectRef, JSPropertyNameAccumulatorRef propertyNames);
    static JSValueRef callInstanceAsFunctionCallback(JSContextRef, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef callPropertyAsFunctionCallback(JSContextRef, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSObjectRef callAsConstructorCallback(JSContextRef, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static bool hasInstanceCallback(JSContextRef, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception);
    static JSValueRef convertToTypeCallback(JSContextRef, JSObjectRef, ::JSType type, JSValueRef* exception);

    JSValueRef callPropertyAsFunction(JSContextRef, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    RefPtr<API::Prototype> m_impl;
    JSClassRef m_definition { nullptr };
    HashMap<JSObjectRef, std::string> m_functions;
    ScriptExtension& m_extension;
};

} // namespace Extensions

#endif
