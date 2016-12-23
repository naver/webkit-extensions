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

#ifndef Chrome_StorageArea_h
#define Chrome_StorageArea_h

#include "JSWrappable.h"
#include <JavaScriptCore/JSValueRef.h>

namespace Extensions {
namespace chrome {

class Chrome;

class StorageArea : public JSWrappable {
public:
    StorageArea(Chrome&);

    JSObjectRef wrap(JSContextRef) override;

    void get(RefPtr<JSValue> keys, RefPtr<JSFunction> callback);
    void getBytesInUse(RefPtr<JSValue> keys, RefPtr<JSFunction> callback);

    void set(RefPtr<JSDictionary> items, RefPtr<JSFunction> callback);

    void remove(RefPtr<JSArray> keys, RefPtr<JSFunction> callback);
    void clear(RefPtr<JSFunction> callback);

private:
    Chrome& m_chrome;
};

}
}

#endif // Chrome_StorageArea_h
