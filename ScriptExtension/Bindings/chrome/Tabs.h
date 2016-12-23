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

#ifndef Chrome_Tabs_h
#define Chrome_Tabs_h

#include "JSWrappable.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {
namespace chrome {

class Chrome;

class Tabs : public JSWrappable {
public:  
    Tabs(Chrome&);

    JSObjectRef wrap(JSContextRef) override;

    void get(double tabId, RefPtr<JSFunction> callback);
    void getCurrent(RefPtr<JSFunction> callback);
    void connect(double tabId, RefPtr<JSDictionary> connectInfo);
    void sendRequest(double tabId, RefPtr<JSDictionary> request, RefPtr<JSFunction> responseCallback);
    void sendMessage(double tabId, RefPtr<JSDictionary> message, RefPtr<JSDictionary> options, RefPtr<JSFunction> responseCallback);
    void getSelected(double windowId, RefPtr<JSFunction> callback);
    void getAllInWindow(double windowId, RefPtr<JSFunction> callback);
    void create(RefPtr<JSDictionary> createProperties, RefPtr<JSFunction> callback);
    void duplicate(double tabId, RefPtr<JSFunction> callback);
    void query(RefPtr<JSDictionary> queryInfo, RefPtr<JSFunction> callback);
    void highlight(RefPtr<JSDictionary> highlightInfo, RefPtr<JSFunction> callback);
    void update(double tabId, RefPtr<JSDictionary> updateProperties, RefPtr<JSFunction> callback);
    void move(RefPtr<JSArray> tabIds, RefPtr<JSDictionary> moveProperties, RefPtr<JSFunction> callback);
    void reload(double tabId, RefPtr<JSDictionary> reloadProperties, RefPtr<JSFunction> callback);
    void remove(RefPtr<JSArray> tabIds, RefPtr<JSFunction> callback);
    void detectLanguage(RefPtr<JSArray> tabIds, RefPtr<JSFunction> callback);
    void captureVisibleTab(double windowId, RefPtr<JSDictionary> options, RefPtr<JSFunction> callback);
    void executeScript(double tabId, RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void insertCSS(double tabId, RefPtr<JSDictionary> details, RefPtr<JSFunction> callback);
    void setZoom(double tabId, double zoomFactor, RefPtr<JSFunction> callback);
    void getZoom(double tabId, RefPtr<JSFunction> callback);

private:
    Chrome& m_chrome;
};

}
}

#endif // Chrome_Tabs_h
