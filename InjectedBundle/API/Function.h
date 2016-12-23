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

#ifndef API_Function_h
#define API_Function_h

#include "ProcessLocalData.h"
#include <functional>

namespace API {

class Array;

typedef std::tuple<std::function<RefPtr<Array> (const Array&)>> APIFunctionLocalType;
typedef std::tuple<> APIFunctionProxyType;

typedef std::function<void (const Array&)> APIFunctionDelayedReturnCallback;

class Function final : public ProcessLocalDataImpl<APIFunctionLocalType, APIFunctionProxyType> {
public:
    Function();
    explicit Function(std::function<RefPtr<Array> (const Array&)>);
    ~Function();

    Function(const Function& other) { Data::operator=(other); ASSERT(!other.hasPendingDelayedReturn()); }
    Function(Function&& other) { Data::operator=(other); ASSERT(!other.hasPendingDelayedReturn()); }
    Function& operator=(const Function& other) { Data::operator=(other); ASSERT(!hasPendingDelayedReturn() && !other.hasPendingDelayedReturn()); return *this; }
    Function& operator=(Function&& other) { Data::operator=(other); ASSERT(!hasPendingDelayedReturn() && !other.hasPendingDelayedReturn()); return *this; }

    std::function<RefPtr<Array> (const Array&)> function() const;
    std::function<RefPtr<Array> (const Array&)> function(std::nullptr_t) const;
    std::function<RefPtr<Array> (const Array&)> function(APIFunctionDelayedReturnCallback&&) const;

    bool hasPendingDelayedReturn() const;
    void delayedReturn(const Array&) const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Function&);

private:
    struct FunctionContextData {
        RefPtr<Function> protect;
        std::unique_ptr<APIFunctionDelayedReturnCallback> delayedReturnCallback;
    };

    FunctionContextData& functionData() const;

    void initializeData(ObjectDictionary&, uint64_t, uint64_t) override;
    void encodeAttachments(ObjectDictionary&) const override { }
    APIFunctionProxyType decodeAttachments(ObjectDictionary&) override { return APIFunctionProxyType(); }
};

}

#endif // API_Function_h
