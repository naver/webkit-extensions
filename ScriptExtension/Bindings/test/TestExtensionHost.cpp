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
#include "TestExtensionHost.h"

#include "InjectedBundlePageProxy.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHost.h"
#include "TestExtensionHostMessages.h"

namespace Extensions {
namespace test {

TestExtensionHost::TestExtensionHost(ScriptExtensionHost& extension)
    : m_extension(extension)
{
}

TestExtensionHost::~TestExtensionHost()
{
}

void TestExtensionHost::testCallback(const API::Dictionary& dict, const API::Function& callback)
{
    double highestValue = 0.0;
    std::string highestValueKey;

    auto keyValues = dict.copyKeyValues();
    for (auto& keyValue : keyValues) {
        Ref<API::Number> value = API::Object::create<API::Number>(keyValue.value->impl());
        double doubleValue = value->doubleValue();
        if (doubleValue > highestValue) {
            highestValue = doubleValue;
            highestValueKey = keyValue.key;
        }
    }

    API::ObjectArray arguments;
    arguments.append(API::Object::create<APIString>(highestValueKey));
    m_extension.page().dispatchCallback(callback, API::Object::create<API::Array>(arguments).get());
}

}
}
