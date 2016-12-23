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

#ifndef Chrome_Tab_h
#define Chrome_Tab_h

#include "JSWrappable.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {

class ScriptExtension;

namespace chrome {

class Tab : public JSWrappable {
public:  
    Tab(ScriptExtension&);

    JSObjectRef wrap(JSContextRef) override;

    double id() { return m_id; }
    double index() { return m_index; }
    double windowId() { return m_windowId; }
    double openerTabId() { return m_openerTabId; }
    bool selected() { return m_selected; }
    bool highlighted() { return m_highlighted; }
    bool active() { return m_active; }
    bool pinned() { return m_pinned; }
    bool audible() { return m_audible; }
    RefPtr<JSDictionary> mutedInfo() { return m_mutedInfo; }
    RefPtr<JSString> url() { return m_url; }
    RefPtr<JSString> title() { return m_title; }
    RefPtr<JSString> favIconUrl() { return m_favIconUrl; }
    RefPtr<JSString> status() { return m_status; }
    bool incognito() { return m_incognito; }
    double width() { return m_width; }
    double height() { return m_height; }
    RefPtr<JSString> sessionId() { return m_sessionId; }

private:
    ScriptExtension& m_extension;

    double m_id;
    double m_index;
    double m_windowId;
    double m_openerTabId;
    bool m_selected;
    bool m_highlighted;
    bool m_active;
    bool m_pinned;
    bool m_audible;
    RefPtr<JSDictionary> m_mutedInfo;
    RefPtr<JSString> m_url;
    RefPtr<JSString> m_title;
    RefPtr<JSString> m_favIconUrl;
    RefPtr<JSString> m_status;
    bool m_incognito;
    double m_width;
    double m_height;
    RefPtr<JSString> m_sessionId;
};

}
}

#endif // Chrome_Tab_h
