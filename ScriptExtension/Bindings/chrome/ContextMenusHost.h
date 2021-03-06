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

#ifndef Chrome_ContextMenusHost_h
#define Chrome_ContextMenusHost_h

#include "Dictionary.h"
#include "Function.h"
#include "Number.h"
#include "MessageReceiver.h"

namespace Extensions {
namespace chrome {

class ChromeHost;

class ContextMenusHost : public IPC::MessageReceiver {
public:
    ContextMenusHost(ChromeHost&);
    ~ContextMenusHost();

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;
    void didReceiveSyncMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder, std::unique_ptr<IPC::MessageEncoder>& encoder) override;

    void create(const API::Dictionary& createProperties, const API::Function& callback, RefPtr<API::Number>& result);
    void update(const API::Number& id, const API::Dictionary& updateProperties, const API::Function& callback);
    void remove(const API::Number& menuItemId, const API::Function& callback);
    void removeAll(const API::Function& callback);

    ChromeHost& m_chrome;
};

}
}

#endif // Chrome_ContextMenusHost_h
