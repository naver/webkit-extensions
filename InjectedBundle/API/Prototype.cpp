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
#include "Prototype.h"

#include "ArgumentCoders.h"
#include "ArgumentDecoder.h"
#include "ArgumentEncoder.h"
#include "GetSet.h"
#include "Number.h"
#include <reflect/Class.h>
#include <reflect/Field.h>
#include <reflect/Method.h>
#include <vector>
#include <wtf/PassRefPtr.h>

namespace API {

Prototype::Prototype()
{
}

Prototype::Prototype(RefCountedAny* ptr)
    : ProcessLocalDataImpl(std::make_tuple(ptr))
{
    ASSERT(reflect::ClassUnknown::getClass(ptr));
    ptr->ref();
}

Prototype::Prototype(WebKit::WKAdoptTag, RefCountedAny* ptr)
    : ProcessLocalDataImpl(std::make_tuple(adoptRef(ptr).leakRef()))
{
    ASSERT(reflect::ClassUnknown::getClass(ptr));
}

Prototype::~Prototype()
{
    if (ptr())
        ptr()->deref();
}

RefCountedAny* Prototype::ptr() const
{
    return (availability() == Availability::Local) ? std::get<0>(data<APIPrototypeLocalType>()) : nullptr;
}

std::string Prototype::name() const
{
    return (availability() == Availability::Local) ? reflect::ClassUnknown::getClass(ptr())->getName() : std::get<0>(data<APIPrototypeProxyType>());
}

Vector<std::string> Prototype::staticFunctions() const
{
    if (availability() == Availability::Proxy)
        return std::get<1>(data<APIPrototypeProxyType>());

    reflect::ClassUnknown* unknown = reflect::ClassUnknown::getClass(ptr());
    auto declaredMethods = unknown->getDeclaredMethods();
    Vector<std::string> functions;
    for (auto& declaredMethod : declaredMethods)
        functions.append(declaredMethod->getName());
    return functions;
}

Vector<std::string> Prototype::staticValues() const
{
    if (availability() == Availability::Proxy)
        return std::get<2>(data<APIPrototypeProxyType>());

    reflect::ClassUnknown* unknown = reflect::ClassUnknown::getClass(ptr());
    auto declaredFields = unknown->getDeclaredFields();
    Vector<std::string> values;
    for (auto& declaredField : declaredFields)
        values.append(declaredField->getName());
    return values;
}

void Prototype::encode(IPC::ArgumentEncoder& encoder) const
{
    ProcessLocalData::encode(encoder);
}

bool Prototype::decode(IPC::ArgumentDecoder& decoder, Prototype& result)
{
    return ProcessLocalData::decode(decoder, result);
}

void Prototype::encodeAttachments(ObjectDictionary& values) const
{
    Setter setter(values);
    setter.set("?name", name());
    setter.set("?staticFunctions", staticFunctions());
    setter.set("?staticValues", staticValues());
}

APIPrototypeProxyType Prototype::decodeAttachments(ObjectDictionary& values)
{
    Getter getter(values);

    std::string name;
    if (!getter.get("?name", name))
        return APIPrototypeProxyType();

    Vector<std::string> staticFunctions;
    if (!getter.get("?staticFunctions", staticFunctions))
        return APIPrototypeProxyType();

    Vector<std::string> staticValues;
    if (!getter.get("?staticValues", staticValues))
        return APIPrototypeProxyType();

    return std::make_tuple(name, std::move(staticFunctions), std::move(staticValues));
}

}
