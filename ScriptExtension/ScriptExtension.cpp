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
#include "ScriptExtension.h"

#include "InjectedBundlePage.h"
#include "ScriptPrototype.h"
#include "ScriptExtensionMessages.h"

#include <chrome/ChromeMain.h>
#include <test/TestExtensionMain.h>

namespace Extensions {

ScriptExtension::ScriptExtension(InjectedBundlePage& page)
    : m_page(page)
{
    m_page.addMessageReceiver(Messages::ScriptExtension::messageReceiverName(), *this);
}

ScriptExtension::~ScriptExtension()
{
    m_page.removeMessageReceiver(Messages::ScriptExtension::messageReceiverName());
}

void ScriptExtension::initialize()
{
    Extensions::chrome::initializeExtension(*this);
    Extensions::test::initializeExtension(*this);
}

void ScriptExtension::addExtension(Ref<JSWrappable> wrappable, std::string propertyName)
{
    m_extensions.set(std::move(propertyName), WTF::move(wrappable));
}

void ScriptExtension::addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    m_page.addMessageReceiver(messageReceiverName, messageReceiver);
}

void ScriptExtension::addMessageReceiver(IPC::StringReference messageReceiverName, std::unique_ptr<IPC::MessageReceiver> messageReceiver)
{
    IPC::MessageReceiver& receiver = *messageReceiver;
    m_messageReceivers.append(std::move(messageReceiver));
    addMessageReceiver(messageReceiverName, receiver);
}

void ScriptExtension::wrapPrototype(Ref<API::Prototype> object, std::string propertyName)
{
    m_extensions.set(std::move(propertyName), adoptRef(*new ScriptPrototype(WTF::move(object), *this)));
}

void ScriptExtension::injectExtensions(JSGlobalContextRef context)
{
    JSObjectRef window = JSContextGetGlobalObject(context);

    for (auto& extension : m_extensions)
        extension.value->inject(context, window, extension.key);
}

void ScriptExtension::addUserScriptPrototype(const API::Prototype& scriptPrototype, const API::APIString& name)
{
    Ref<API::Prototype> prototype = API::Object::create<API::Prototype>(scriptPrototype);
    wrapPrototype(WTF::move(prototype), name.utf8());
}

} // namespace Extensions
