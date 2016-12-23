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

#ifndef API_Prototype_h
#define API_Prototype_h

#include "ProcessLocalData.h"
#include "RefCountedAny.h"
#include <string>
#include <wtf/Vector.h>

namespace API {

typedef std::tuple<RefCountedAny*> APIPrototypeLocalType;
typedef std::tuple<std::string, Vector<std::string>, Vector<std::string>> APIPrototypeProxyType;

class Prototype final : public ProcessLocalDataImpl<APIPrototypeLocalType, APIPrototypeProxyType> {
public:
    Prototype();
    explicit Prototype(RefCountedAny*);
    explicit Prototype(WebKit::WKAdoptTag, RefCountedAny*);
    ~Prototype();

    RefCountedAny* ptr() const;
    std::string name() const;
    Vector<std::string> staticFunctions() const;
    Vector<std::string> staticValues() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, Prototype&);

private:
    void encodeAttachments(ObjectDictionary&) const override;
    APIPrototypeProxyType decodeAttachments(ObjectDictionary&) override;
};

}

#endif // API_Prototype_h
