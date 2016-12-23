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

#ifndef Test_TestExtension_h
#define Test_TestExtension_h

#include "JSWrappable.h"

namespace Extensions {

class ScriptExtension;

namespace test {

class TestExtension : public JSWrappable {
public:
    TestExtension(ScriptExtension&);

    JSObjectRef wrap(JSContextRef) override;

    void favoriteFruit(double number, RefPtr<JSString> string);
    RefPtr<JSDictionary> otherFruits(RefPtr<JSArray> arr1, RefPtr<JSArray> arr2);
    void chooseSweetest(RefPtr<JSDictionary> dict, RefPtr<JSFunction> callback);

    double brix();
    void setBrix(double number);
    RefPtr<JSString> name();
    void setName(RefPtr<JSString> string);

private:
    ScriptExtension& m_extension;

    double m_brix;
    RefPtr<JSString> m_name;
};

}
}

#endif // Test_TestExtension_h
