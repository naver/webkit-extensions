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

#ifndef Chrome_TabsHost_h
#define Chrome_TabsHost_h

#include "Dictionary.h"
#include "Function.h"
#include "Number.h"
#include "MessageReceiver.h"

namespace Extensions {
namespace chrome {

class ChromeHost;

class TabsHost : public IPC::MessageReceiver {
public:
    TabsHost(ChromeHost&);
    ~TabsHost();

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void get(const API::Number& tabId, const API::Function& callback);
    void getCurrent(const API::Function& callback);
    void connect(const API::Number& tabId, const API::Dictionary& connectInfo);
    void sendRequest(const API::Number& tabId, const API::Dictionary& request, const API::Function& responseCallback);
    void sendMessage(const API::Number& tabId, const API::Dictionary& message, const API::Dictionary& options, const API::Function& responseCallback);
    void getSelected(const API::Number& windowId, const API::Function& callback);
    void getAllInWindow(const API::Number& windowId, const API::Function& callback);
    void create(const API::Dictionary& createProperties, const API::Function& callback);
    void duplicate(const API::Number& tabId, const API::Function& callback);
    void query(const API::Dictionary& queryInfo, const API::Function& callback);
    void highlight(const API::Dictionary& highlightInfo, const API::Function& callback);
    void update(const API::Number& tabId, const API::Dictionary& updateProperties, const API::Function& callback);
    void move(const API::Array& tabIds, const API::Dictionary& moveProperties, const API::Function& callback);
    void reload(const API::Number& tabId, const API::Dictionary& reloadProperties, const API::Function& callback);
    void remove(const API::Array& tabIds, const API::Function& callback);
    void detectLanguage(const API::Array& tabIds, const API::Function& callback);
    void captureVisibleTab(const API::Number& windowId, const API::Dictionary& options, const API::Function& callback);
    void executeScript(const API::Number& tabId, const API::Dictionary& details, const API::Function& callback);
    void insertCSS(const API::Number& tabId, const API::Dictionary& details, const API::Function& callback);
    void setZoom(const API::Number& tabId, const API::Number& zoomFactor, const API::Function& callback);
    void getZoom(const API::Number& tabId, const API::Function& callback);

    ChromeHost& m_chrome;
};

}
}

#endif // Chrome_TabsHost_h
