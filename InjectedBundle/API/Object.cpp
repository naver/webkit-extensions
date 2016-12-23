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

#include "config.h"
#include "Object.h"

#include "APIString.h"
#include "Array.h"
#include "Dictionary.h"
#include "Function.h"
#include "Number.h"

namespace API {

Ref<Object> Object::create(WKTypeRef impl)
{
    if (WKGetTypeID(impl) == WKBooleanGetTypeID())
        return create<Number>(static_cast<WKBooleanRef>(impl));
    else if (WKGetTypeID(impl) == WKDoubleGetTypeID())
        return create<Number>(static_cast<WKDoubleRef>(impl));
    else if (WKGetTypeID(impl) == WKUInt64GetTypeID())
        return create<Number>(static_cast<WKUInt64Ref>(impl));
    else if (WKGetTypeID(impl) == WKStringGetTypeID())
        return create<APIString>(impl);
    else if (WKGetTypeID(impl) == WKArrayGetTypeID())
        return create<Array>(impl);
    else if (WKGetTypeID(impl) == WKDataGetTypeID())
        return create<Data>(impl);
    else if (WKGetTypeID(impl) == WKDictionaryGetTypeID())
        return create<Dictionary>(impl);

    ASSERT_NOT_REACHED();
    Object* o = nullptr;
    return *o;
}

}
