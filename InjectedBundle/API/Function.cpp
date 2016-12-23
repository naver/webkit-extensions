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
#include "Function.h"

#include "ArgumentCoders.h"
#include "ArgumentDecoder.h"
#include "ArgumentEncoder.h"
#include "InjectedBundleProcess.h"
#include "InjectedBundleProcessMessages.h"

namespace API {

Function::Function()
{
}

Function::Function(std::function<RefPtr<Array> (const Array&)> function)
    : ProcessLocalDataImpl(std::make_tuple(std::move(function)))
{
    setContextData(std::make_shared<FunctionContextData>());
}

Function::~Function()
{
}

std::function<RefPtr<Array> (const Array&)> Function::function() const
{
    RefPtr<Function> protect(API::Object::copy(*this));

    return (availability() == Availability::Local) ? std::get<0>(data<APIFunctionLocalType>()) : [=] (const Array& arguments) {
        ASSERT(connection() && connection()->canSendSyncMessages());

        RefPtr<Array> result = API::Object::create<API::Array>();
        connection()->sendSync(Messages::InjectedBundleProcess::CallFunction(*protect, arguments),
            Messages::InjectedBundleProcess::CallFunction::Reply(*result), 0);
        return WTF::move(result);
    };
}

std::function<RefPtr<Array> (const Array&)> Function::function(std::nullptr_t) const
{
    RefPtr<Function> protect(API::Object::copy(*this));

    return (availability() == Availability::Local) ? std::get<0>(data<APIFunctionLocalType>()) : [=] (const Array& arguments) {
        connection()->send(Messages::InjectedBundleProcess::DispatchFunction(*protect, arguments), 0);
        return nullptr;
    };
}

std::function<RefPtr<Array> (const Array&)> Function::function(APIFunctionDelayedReturnCallback&& callback) const
{
    ASSERT(availability() == Availability::Proxy && !hasPendingDelayedReturn());
    ASSERT(connection() && !connection()->canSendSyncMessages());

    FunctionContextData& functionData = this->functionData();
    functionData.protect = API::Object::copy(*this);
    functionData.delayedReturnCallback = std::make_unique<APIFunctionDelayedReturnCallback>(callback);

    RefPtr<Function> protect(functionData.protect);

    return [=] (const Array& arguments) {
        connection()->send(Messages::InjectedBundleProcess::CallFunctionDelayReturn(*protect, arguments), 0);
        return nullptr;
    };
}

bool Function::hasPendingDelayedReturn() const
{
    return !!functionData().delayedReturnCallback;
}

void Function::delayedReturn(const Array& results) const
{
    ASSERT(availability() == Availability::Proxy && hasPendingDelayedReturn());

    FunctionContextData& functionData = this->functionData();
    RefPtr<Function> protect = WTF::move(functionData.protect);
    std::unique_ptr<APIFunctionDelayedReturnCallback> callback = std::move(functionData.delayedReturnCallback);
    (*callback)(results);
}

Function::FunctionContextData& Function::functionData() const
{
    return *(static_cast<FunctionContextData*>(contextData().get()));
}

void Function::encode(IPC::ArgumentEncoder& encoder) const
{
    ProcessLocalData::encode(encoder);
}

bool Function::decode(IPC::ArgumentDecoder& decoder, Function& result)
{
    return ProcessLocalData::decode(decoder, result);
}

void Function::initializeData(ObjectDictionary& values, uint64_t processID, uint64_t ID)
{
    ProcessLocalDataImpl::initializeData(values, processID, ID);
    setContextData(std::make_shared<FunctionContextData>());
}

}