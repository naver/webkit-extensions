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
#include "ProcessLocalData.h"

#include "ArgumentCoders.h"
#include "ArgumentDecoder.h"
#include "ArgumentEncoder.h"
#include "GetSet.h"
#include "InjectedBundleProcess.h"
#include "InjectedBundleProcessMessages.h"
#include <wtf/NeverDestroyed.h>

namespace API {

typedef std::pair<ProcessIdentifier, uint64_t> DataIdentifier;

class ProcessLocalDataPrivate {
public:
    typedef ProcessLocalData::FreeDataFunctionWrapper FreeDataFunctionWrapper;
};
typedef ProcessLocalDataPrivate::FreeDataFunctionWrapper FreeDataFunctionWrapper;

static uint64_t generateIdentifier()
{
    static uint64_t uniqueID;
    return ++uniqueID;
}

static HashMap<DataIdentifier, RefPtr<ProcessLocalData>>& handles()
{
    static NeverDestroyed<HashMap<DataIdentifier, RefPtr<ProcessLocalData>>> map;
    return map;
}

static HashMap<DataIdentifier, FreeDataFunctionWrapper*>& proxies()
{
    static NeverDestroyed<HashMap<DataIdentifier, FreeDataFunctionWrapper*>> map;
    return map;
}

ProcessLocalData::FreeDataFunctionContext::~FreeDataFunctionContext()
{
    if (availability == Availability::Proxy) {
        IPC::Connection* connection = Extensions::InjectedBundleProcess::getConnection(processID);
        ProcessLocalData deletedData;
        DataIdentifier dataID = std::make_pair(processID, ID);
        FreeDataFunctionWrapper temporaryFunction = { 0, 0, this };
        deletedData.m_freeDataFunction = &temporaryFunction;
        connection->send(Messages::InjectedBundleProcess::DereferenceData(deletedData), 0);
        proxies().remove(dataID);
        deletedData.m_freeDataFunction.leakRef();
    }
}

void ProcessLocalData::dereference(const ProcessLocalData& processLocalData)
{
    FreeDataFunctionContext& context = *processLocalData.context();
    DataIdentifier dataID = std::make_pair(context.processID, context.ID);
    ASSERT(handles().contains(dataID));
    handles().remove(dataID);
}

ProcessLocalData::~ProcessLocalData()
{
}

IPC::Connection* ProcessLocalData::connection() const
{
    return Extensions::InjectedBundleProcess::getConnection(processID());
}

ProcessLocalData::FreeDataFunctionContext* ProcessLocalData::context() const
{
    return WTF::bitwise_cast<FreeDataFunctionContext*>(m_freeDataFunction->context);
}

const std::shared_ptr<void>& ProcessLocalData::contextData() const
{
    return context()->data;
}

void ProcessLocalData::setContextData(const std::shared_ptr<void>& ptr)
{
    context()->data = ptr;
}

void ProcessLocalData::release(IPC::ArgumentEncoder& encoder, const ProcessLocalData& data)
{
    FreeDataFunctionContext& context = *data.context();
    Availability availability = context.availability;

    if (availability == Availability::Local) {
        if (!context.ID) {
            context.processID = IPC::currentProcess();
            context.ID = generateIdentifier();
        }
    }

    DataIdentifier dataID = std::make_pair(context.processID, context.ID);

    if (availability == Availability::Local)
        handles().add(dataID, API::Object::copy(data));

    ObjectDictionary values;
    Setter setter(values);
    setter.set("?processID", dataID.first.value);
    setter.set("?ID", dataID.second);
    setter.set("?availability", static_cast<uint64_t>(availability));

    if (availability == Availability::Local)
        data.encodeAttachments(values);

    encoder << Object::create<Dictionary>(values);
}

bool ProcessLocalData::acquire(IPC::ArgumentDecoder& decoder, ProcessLocalData& data)
{
    WKDictionaryRef dictionaryRef;
    if (!decoder.decode(dictionaryRef))
        return false;

    Ref<Dictionary> dictionary = Object::create<Dictionary>(dictionaryRef);
    ObjectDictionary values = dictionary->copyKeyValues();
    Getter getter(values);

    uint64_t processID;
    if (!getter.get("?processID", processID))
        return false;

    uint64_t ID;
    if (!getter.get("?ID", ID))
        return false;

    uint64_t availabilityValue;
    if (!getter.get("?availability", availabilityValue))
        return false;

    DataIdentifier dataID(processID, ID);
    Availability availability = availabilityValue ? Availability::Proxy : Availability::Local;

    if (availability == Availability::Local)
        data = *handles().get(dataID);
    else {
        if (!proxies().contains(dataID)) {
            data.initializeData(values, processID, ID);
            proxies().add(std::move(dataID), data.m_freeDataFunction.get());
        } else {
            FreeDataFunctionWrapper* wrapper = proxies().get(dataID);
            data.m_freeDataFunction = wrapper;
            data.m_impl = wrapper->data;
        }
    }

    return true;
}

void ProcessLocalData::encode(IPC::ArgumentEncoder& encoder) const
{
    ProcessLocalData::release(encoder, *this);
}

bool ProcessLocalData::decode(IPC::ArgumentDecoder& decoder, ProcessLocalData& result)
{
    if (!ProcessLocalData::acquire(decoder, result))
        return false;

    return true;
}

}
