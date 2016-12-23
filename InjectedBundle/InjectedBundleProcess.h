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

#ifndef InjectedBundleProcess_h
#define InjectedBundleProcess_h

#include "Connection.h"
#include "Function.h"
#include "InjectedBundleProcessSupplement.h"
#include "ProcessLocalData.h"
#include "MessageReceiverMap.h"
#include <wtf/Forward.h>
#include <wtf/HashMap.h>

namespace Extensions {

class InjectedBundleProcess : public IPC::MessageReceiver {
public:
    static IPC::Connection* getConnection(ProcessIdentifier);

    template <typename T>
    T* supplement()
    {
        return static_cast<T*>(supplement(T::supplementName()));
    }

    template <typename T>
    void addSupplement()
    {
        addSupplement(T::supplementName(), std::make_unique<T>());
    }

    ProcessIdentifier processID() const { return m_processID; }
    IPC::Connection& connection() const { return *m_connection; }

    void addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver&);
    void removeMessageReceiver(IPC::StringReference messageReceiverName);

    void dispatchCallback(const API::Function&, const API::Array&);

protected:
    InjectedBundleProcess(ProcessIdentifier);
    virtual ~InjectedBundleProcess();

    void setConnection(std::unique_ptr<IPC::Connection>);
    void addConnection(ProcessIdentifier);

    void dispatchMessage(IPC::Connection&, IPC::MessageDecoder&);
    void dispatchSyncMessage(IPC::Connection&, IPC::MessageDecoder&, std::unique_ptr<IPC::MessageEncoder>&);

    // IPC::MessageReceiver
    virtual void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override;
    virtual void didReceiveSyncMessage(IPC::Connection&, IPC::MessageDecoder&, std::unique_ptr<IPC::MessageEncoder>&) override;

    virtual void didConnect(const ProcessIdentifier&) = 0;

private:
    void dereferenceData(const API::ProcessLocalData&);
    void dispatchFunction(const API::Function&, const API::Array&);
    void callFunction(const API::Function&, const API::Array&, RefPtr<API::Array>& result);
    void callFunctionDelayReturn(const API::Function&, const API::Array&);
    void delayedFunctionReturn(const API::Function&, const API::Array&);

    InjectedBundleProcessSupplement* supplement(const char*);
    void addSupplement(const char*, std::unique_ptr<InjectedBundleProcessSupplement>);

    ProcessIdentifier m_processID;
    std::unique_ptr<IPC::Connection> m_connection;
    IPC::MessageReceiverMap m_messageReceiverMap;

    typedef HashMap<const char*, std::unique_ptr<InjectedBundleProcessSupplement>, PtrHash<const char*>> InjectedBundleProcessSupplementMap;
    InjectedBundleProcessSupplementMap m_supplements;
};

} // namespace Extensions

#endif // InjectedBundleProcess_h
