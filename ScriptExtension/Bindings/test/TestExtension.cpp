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
#include "TestExtension.h"

#include "InjectedBundlePage.h"
#include "JSTestExtension.h"
#include "ScriptExtension.h"
#include "TestExtensionHostMessages.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace test {

TestExtension::TestExtension(ScriptExtension& extension)
    : m_extension(extension)
    , m_brix(0.0)
{
}

JSObjectRef TestExtension::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSTestExtension::testExtensionClass(), this);
}

void TestExtension::favoriteFruit(double number, RefPtr<JSString> string)
{
    setBrix(number);
    setName(string);
}

RefPtr<JSDictionary> TestExtension::otherFruits(RefPtr<JSArray> arr1, RefPtr<JSArray> arr2)
{
    RefPtr<JSDictionary> result = arr1->toDictionary(arr2);
    result->set(m_name->stringValue(), m_brix);
    return WTF::move(result);
}

void TestExtension::chooseSweetest(RefPtr<JSDictionary> dict, RefPtr<JSFunction> callback)
{
    m_extension.page().connection().send(Messages::TestExtensionHost::TestCallback(*toAPI(dict), *toAPI(callback)), 0);
}

double TestExtension::brix()
{
    return m_brix;
}

void TestExtension::setBrix(double number)
{
    m_brix = number;
}

RefPtr<JSString> TestExtension::name()
{
    return m_name;
}

void TestExtension::setName(RefPtr<JSString> string)
{
    m_name = string;
}

}
}
