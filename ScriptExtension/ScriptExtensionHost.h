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

#ifndef ScriptExtensionHost_h
#define ScriptExtensionHost_h

#include "MessageReceiver.h"
#include "Prototype.h"
#include <WebKit/WKBase.h>
#include <reflect/Class.h>
#include <wtf/Forward.h>
#include <wtf/Vector.h>

namespace Extensions {

class InjectedBundlePageProxy;

class ScriptExtensionHost final : public IPC::MessageReceiver {
public:
    ScriptExtensionHost(InjectedBundlePageProxy&);
    ~ScriptExtensionHost();

    void initialize();

    void addPrototype(Ref<API::Prototype>, std::string);
    template<typename T, typename... Args> void addPrototype(std::string propertyName, Args&&... args)
    {
        addPrototype(adoptRef(*new API::Prototype(AdoptWK, reflect::getClass<T>().newInstance(reflect::LeakPtr, std::forward<Args>(args)...))), std::move(propertyName));
    }

    void addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver&);
    void addMessageReceiver(IPC::StringReference messageReceiverName, std::unique_ptr<IPC::MessageReceiver>);
    template<typename T, typename... Args> void addMessageReceiver(IPC::StringReference messageReceiverName, Args&&... args)
    {
        addMessageReceiver(messageReceiverName, std::make_unique<T>(std::forward<Args>(args)...));
    }

    InjectedBundlePageProxy& page() { return m_page; }

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override { }

    InjectedBundlePageProxy& m_page;
    Vector<std::unique_ptr<IPC::MessageReceiver>> m_messageReceivers;
};

} // namespace Extensions

#endif // ScriptExtensionHost_h
