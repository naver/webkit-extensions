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
#include "ScriptExtensionHost.h"

#include "InjectedBundlePageProxy.h"
#include "ScriptExtensionMessages.h"
#include "ScriptExtensionHostMessages.h"

#include <chrome/ChromeMain.h>
#include <test/TestExtensionMain.h>

namespace Extensions {

ScriptExtensionHost::ScriptExtensionHost(InjectedBundlePageProxy& page)
    : m_page(page)
{
    m_page.addMessageReceiver(Messages::ScriptExtensionHost::messageReceiverName(), *this);
}

ScriptExtensionHost::~ScriptExtensionHost()
{
    m_page.removeMessageReceiver(Messages::ScriptExtensionHost::messageReceiverName());
}

void ScriptExtensionHost::initialize()
{
    Extensions::chrome::initializeExtension(*this);
    Extensions::test::initializeExtension(*this);
}

void ScriptExtensionHost::addPrototype(Ref<API::Prototype> object, std::string propertyName)
{
    m_page.connection().send(Messages::ScriptExtension::AddUserScriptPrototype(WTF::move(object), API::Object::create<APIString>(propertyName)), 0);
}

void ScriptExtensionHost::addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    m_page.addMessageReceiver(messageReceiverName, messageReceiver);
}

void ScriptExtensionHost::addMessageReceiver(IPC::StringReference messageReceiverName, std::unique_ptr<IPC::MessageReceiver> messageReceiver)
{
    IPC::MessageReceiver& receiver = *messageReceiver;
    m_messageReceivers.append(std::move(messageReceiver));
    addMessageReceiver(messageReceiverName, receiver);
}

} // namespace Extensions
