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

#ifndef ScriptExtension_h
#define ScriptExtension_h

#include "APIString.h"
#include "Function.h"
#include "MessageReceiver.h"
#include "Prototype.h"
#include "StringHash.h"
#include <JavaScriptCore/JSContextRef.h>
#include <reflect/Class.h>
#include <wtf/HashMap.h>

namespace Extensions {

class InjectedBundlePage;
class JSWrappable;

class ScriptExtension final : public IPC::MessageReceiver {
public:
    ScriptExtension(InjectedBundlePage&);
    ~ScriptExtension();

    void initialize();

    void addExtension(Ref<JSWrappable>, std::string);
    template<typename T, typename... Args> void addExtension(std::string propertyName, Args&&... args)
    {
        addExtension(adoptRef(*new T(std::forward<Args>(args)..., *this)), std::move(propertyName));
    }

    void addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver&);
    void addMessageReceiver(IPC::StringReference messageReceiverName, std::unique_ptr<IPC::MessageReceiver>);
    template<typename T, typename... Args> void addMessageReceiver(IPC::StringReference messageReceiverName, Args&&... args)
    {
        addMessageReceiver(messageReceiverName, std::make_unique<T>(std::forward<Args>(args)...));
    }

    void wrapPrototype(Ref<API::Prototype>, std::string);
    template<typename T, typename... Args> void wrapPrototype(std::string propertyName, Args&&... args)
    {
        wrapPrototype(adoptRef(*new API::Prototype(AdoptWK, reflect::getClass<T>().newInstance(reflect::LeakPtr, std::forward<Args>(args)...))), std::move(propertyName));
    }

    void injectExtensions(JSGlobalContextRef);

    InjectedBundlePage& page() { return m_page; }

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override;

    // Message handlers
    void addUserScriptPrototype(const API::Prototype&, const API::APIString&);

    InjectedBundlePage& m_page;
    HashMap<std::string, RefPtr<JSWrappable>> m_extensions;
    Vector<std::unique_ptr<IPC::MessageReceiver>> m_messageReceivers;
};

} // namespace Extensions

#endif // ScriptExtension_h
