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

#ifndef Chrome_BrowserAction_h
#define Chrome_BrowserAction_h

#include "JSWrappable.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {
namespace chrome {

class Chrome;

class BrowserAction : public JSWrappable {
public:  
    BrowserAction(Chrome&);

    JSObjectRef wrap(JSContextRef) override;

    void setTitle(RefPtr<JSDictionary> details);
    void getTitle(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void setIcon(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void setPopup(RefPtr<JSDictionary> details);
    void getPopup(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void setBadgeText(RefPtr<JSDictionary> details);
    void getBadgeText(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void setBadgeBackgroundColor(RefPtr<JSDictionary> details);
    void getBadgeBackgroundColor(RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void enable(double tabId);
    void disable(double tabId);

private:
    Chrome& m_chrome;
};

}
}

#endif // Chrome_BrowserAction_h
