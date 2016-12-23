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

#ifndef API_ProcessLocalData_h
#define API_ProcessLocalData_h

#include "Data.h"
#include "Dictionary.h"
#include "MessageReceiver.h"
#include "ProcessIdentifier.h"
#include "StringHash.h"
#include <wtf/HashMap.h>

namespace API {

class ProcessLocalData : public Data {
    friend class ProcessLocalDataPrivate;
public:
    enum class Availability {
        Proxy,
        Local,
    };

    ProcessLocalData() = default;
    virtual ~ProcessLocalData();

    static void release(IPC::ArgumentEncoder&, const ProcessLocalData&);
    static bool acquire(IPC::ArgumentDecoder&, ProcessLocalData&);

    ProcessIdentifier processID() const;
    Availability availability() const;

    static void dereference(const ProcessLocalData&);

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, ProcessLocalData&);

protected:
    struct FreeDataFunctionContext {
        ProcessIdentifier processID;
        uint64_t ID;
        Availability availability;
        std::shared_ptr<void> data;
        ~FreeDataFunctionContext();
    };

    template<typename T> ProcessLocalData(T, FreeDataFunction, FreeDataFunctionContext*);

    IPC::Connection* connection() const;

    FreeDataFunctionContext* context() const;
    const std::shared_ptr<void>& contextData() const;
    void setContextData(const std::shared_ptr<void>&);

    virtual void initializeData(ObjectDictionary&, uint64_t, uint64_t) { }
    virtual void encodeAttachments(ObjectDictionary&) const { }
};

template<typename T> inline ProcessLocalData::ProcessLocalData(T data, FreeDataFunction freeDataFunction, FreeDataFunctionContext* context)
    : Data(WTF::move(data), freeDataFunction, WTF::bitwise_cast<const void*>(context))
{
}

inline ProcessIdentifier ProcessLocalData::processID() const
{
    return WTF::bitwise_cast<FreeDataFunctionContext*>(m_freeDataFunction->context)->processID;
}

inline ProcessLocalData::Availability ProcessLocalData::availability() const
{
    return WTF::bitwise_cast<FreeDataFunctionContext*>(m_freeDataFunction->context)->availability;
}

template<typename LocalType, typename ProxyType>
class ProcessLocalDataImpl : public ProcessLocalData {
public:
    virtual ~ProcessLocalDataImpl() = default;

protected:
    ProcessLocalDataImpl() = default;
    ProcessLocalDataImpl(LocalType);

    static void freeData(uint8_t*, const void*);

    virtual void initializeData(ObjectDictionary&, uint64_t, uint64_t) override;
    virtual ProxyType decodeAttachments(ObjectDictionary&) = 0;
};

template<typename LocalType, typename ProxyType>
inline ProcessLocalDataImpl<LocalType, ProxyType>::ProcessLocalDataImpl(LocalType impl)
    : ProcessLocalData(WTF::move(impl), freeData, new FreeDataFunctionContext { 0, 0, Availability::Local })
{
}

template<typename LocalType, typename ProxyType>
inline void ProcessLocalDataImpl<LocalType, ProxyType>::freeData(uint8_t* bytes, const void* context)
{
    std::unique_ptr<FreeDataFunctionContext> ptr(WTF::bitwise_cast<FreeDataFunctionContext*>(context));
    if (ptr->availability == Availability::Local)
        Data::finalize<LocalType>(bytes);
    else
        Data::finalize<ProxyType>(bytes);
}

template<typename LocalType, typename ProxyType>
inline void ProcessLocalDataImpl<LocalType, ProxyType>::initializeData(ObjectDictionary& values, uint64_t processID, uint64_t ID)
{
    Data::initialize(decodeAttachments(values), freeData, new FreeDataFunctionContext { processID, ID, Availability::Proxy });
}

}

#endif // API_ProcessLocalData_h
