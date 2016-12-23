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

#ifndef Chrome_BrowserActionHost_h
#define Chrome_BrowserActionHost_h

#include "Dictionary.h"
#include "Function.h"
#include "Number.h"
#include "MessageReceiver.h"

namespace Extensions {
namespace chrome {

class ChromeHost;

class BrowserActionHost : public IPC::MessageReceiver {
public:
    BrowserActionHost(ChromeHost&);
    ~BrowserActionHost();

private:
    void didReceiveMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder) override;

    void setTitle(const API::Dictionary& details);
    void getTitle(const API::Dictionary& details, const API::Function& callback);
    void setIcon(const API::Dictionary& details, const API::Function& callback);
    void setPopup(const API::Dictionary& details);
    void getPopup(const API::Dictionary& details, const API::Function& callback);
    void setBadgeText(const API::Dictionary& details);
    void getBadgeText(const API::Dictionary& details, const API::Function& callback);
    void setBadgeBackgroundColor(const API::Dictionary& details);
    void getBadgeBackgroundColor(const API::Dictionary& details, const API::Function& callback);
    void enable(const API::Number& tabId);
    void disable(const API::Number& tabId);

    ChromeHost& m_chrome;
};

}
}

#endif // Chrome_BrowserActionHost_h
