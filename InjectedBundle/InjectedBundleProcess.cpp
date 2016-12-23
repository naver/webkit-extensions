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
#include "InjectedBundleProcess.h"

#include "InjectedBundleProcessMessages.h"
#include "MessageDecoder.h"
#include "ProcessLocalData.h"
#include <wtf/NeverDestroyed.h>
#include <wtf/HashMap.h>

namespace Extensions {

static HashMap<ProcessIdentifier, IPC::Connection*>& connections()
{
    static NeverDestroyed<HashMap<ProcessIdentifier, IPC::Connection*>> map;
    return map;
}

InjectedBundleProcess::InjectedBundleProcess(ProcessIdentifier processID)
    : m_processID(processID)
{
}

InjectedBundleProcess::~InjectedBundleProcess()
{
    connections().remove(m_processID);
}

IPC::Connection* InjectedBundleProcess::getConnection(ProcessIdentifier processID)
{
    return connections().get(processID);
}

void InjectedBundleProcess::setConnection(std::unique_ptr<IPC::Connection> connection)
{
    ASSERT(!m_connection);
    m_connection = std::move(connection);

    connections().add(ProcessIdentifier(0), m_connection.get());
}

void InjectedBundleProcess::addConnection(ProcessIdentifier processID)
{
    ASSERT(m_connection);
    connections().add(processID, m_connection.get());
}

void InjectedBundleProcess::addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, messageReceiver);
}

void InjectedBundleProcess::removeMessageReceiver(IPC::StringReference messageReceiverName)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName);
}

void InjectedBundleProcess::dispatchCallback(const API::Function& callback, const API::Array& arguments)
{
    getConnection(processID())->send(Messages::InjectedBundleProcess::DispatchFunction(callback, arguments), 0);
}

void InjectedBundleProcess::dispatchMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder)
{
    if (!m_messageReceiverMap.dispatchMessage(connection, decoder)) {
        if (decoder.messageReceiverName() == Messages::InjectedBundleProcess::messageReceiverName()) {
            InjectedBundleProcess::didReceiveMessage(connection, decoder);
            return;
        }
        didReceiveMessage(connection, decoder);
    }
}

void InjectedBundleProcess::dispatchSyncMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder, std::unique_ptr<IPC::MessageEncoder>& replyEncoder)
{
    if (!m_messageReceiverMap.dispatchSyncMessage(connection, decoder, replyEncoder)) {
        if (decoder.messageReceiverName() == Messages::InjectedBundleProcess::messageReceiverName()) {
            InjectedBundleProcess::didReceiveSyncMessage(connection, decoder, replyEncoder);
            return;
        }
        didReceiveSyncMessage(connection, decoder, replyEncoder);
    }
}

void InjectedBundleProcess::dereferenceData(const API::ProcessLocalData& processLocalData)
{
    API::ProcessLocalData::dereference(processLocalData);
}

void InjectedBundleProcess::dispatchFunction(const API::Function& function, const API::Array& arguments)
{
    function.function(nullptr)(arguments);
}

void InjectedBundleProcess::callFunction(const API::Function& function, const API::Array& arguments, RefPtr<API::Array>& result)
{
    result = function.function()(arguments);
}

void InjectedBundleProcess::callFunctionDelayReturn(const API::Function& function, const API::Array& arguments)
{
    connection().send(Messages::InjectedBundleProcess::DelayedFunctionReturn(function, *function.function()(arguments)), 0);
}

void InjectedBundleProcess::delayedFunctionReturn(const API::Function& function, const API::Array& results)
{
    function.delayedReturn(results);
}

InjectedBundleProcessSupplement* InjectedBundleProcess::supplement(const char* supplementName)
{
    return m_supplements.get(supplementName);
}

void InjectedBundleProcess::addSupplement(const char* supplementName, std::unique_ptr<InjectedBundleProcessSupplement> supplement)
{
    InjectedBundleProcessSupplement& entry = *supplement;
    if (m_supplements.add(supplementName, std::move(supplement)))
        entry.initialize(*this);
}

} // namespace Extensions
