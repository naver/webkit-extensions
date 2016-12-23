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

#ifndef Chrome_Windows_h
#define Chrome_Windows_h

#include "JSWrappable.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {
namespace chrome {

class Chrome;

class ChromeWindows : public JSWrappable {
public:  
    ChromeWindows(Chrome&);

    JSObjectRef wrap(JSContextRef) override;

    void get(double windowId, RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback);
    void getCurrent(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback);
    void getLastFocused(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback);
    void getAll(RefPtr<JSDictionary> getInfo, RefPtr<JSFunction> callback);
    void create(RefPtr<JSDictionary> createData, RefPtr<JSFunction> callback);
    void update(double windowId, RefPtr<JSDictionary> updateInfo, RefPtr<JSFunction> callback);
    void remove(double windowId, RefPtr<JSFunction> callback);

private:
    Chrome& m_chrome;
};

}
}

#endif // Chrome_Windows_h
