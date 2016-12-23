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

#ifndef ArgumentCoders_h
#define ArgumentCoders_h

#include "ArgumentDecoder.h"
#include "ArgumentEncoder.h"
#include "ProcessIdentifier.h"
#include <WebKit/WKBundlePage.h>
#include <WebKit/WKData.h>
#include <WebKit/WKDictionary.h>
#include <WebKit/WKNumber.h>
#include <WebKit/WKPage.h>

namespace IPC {

template<typename T> struct ArgumentCoder<Ref<T>> {
    static void encode(ArgumentEncoder& encoder, const Ref<T>& ref)
    {
        encoder << static_reference_cast<API::Object>(ref);
    }

    static bool decode(ArgumentDecoder&, Ref<T>&)
    {
        ASSERT_NOT_REACHED();
    }
};

template<typename T> struct ArgumentCoder<RefPtr<T>> {
    static void encode(ArgumentEncoder& encoder, const RefPtr<T>& ref)
    {
        encoder << static_pointer_cast<API::Object>(ref);
    }

    static bool decode(ArgumentDecoder&, RefPtr<T>&)
    {
        ASSERT_NOT_REACHED();
    }
};

template<> struct ArgumentCoder<WKPageRef> {
    static void encode(ArgumentEncoder&, const WKPageRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKPageRef& page)
    {
        if (!decoder.decode(page, WKPageGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKBundlePageRef> {
    static void encode(ArgumentEncoder&, const WKBundlePageRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKBundlePageRef& page)
    {
        if (!decoder.decode(page, WKBundlePageGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKBooleanRef> {
    static void encode(ArgumentEncoder&, const WKBooleanRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKBooleanRef& number)
    {
        if (!decoder.decode(number, WKBooleanGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKDoubleRef> {
    static void encode(ArgumentEncoder&, const WKDoubleRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKDoubleRef& number)
    {
        if (!decoder.decode(number, WKDoubleGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKUInt64Ref> {
    static void encode(ArgumentEncoder&, const WKUInt64Ref&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKUInt64Ref& number)
    {
        if (!decoder.decode(number, WKUInt64GetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKStringRef> {
    static void encode(ArgumentEncoder&, const WKStringRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKStringRef& string)
    {
        if (!decoder.decode(string, WKStringGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKArrayRef> {
    static void encode(ArgumentEncoder&, const WKArrayRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKArrayRef& array)
    {
        if (!decoder.decode(array, WKArrayGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKDataRef> {
    static void encode(ArgumentEncoder&, const WKDataRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKDataRef& data)
    {
        if (!decoder.decode(data, WKDataGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<WKDictionaryRef> {
    static void encode(ArgumentEncoder&, const WKDictionaryRef&)
    {
        ASSERT_NOT_REACHED();
    }

    static bool decode(ArgumentDecoder& decoder, WKDictionaryRef& dictionary)
    {
        if (!decoder.decode(dictionary, WKDictionaryGetTypeID()))
            return false;
        return true;
    }
};

template<> struct ArgumentCoder<ProcessIdentifier> {
    static void encode(ArgumentEncoder& encoder, const ProcessIdentifier& processID)
    {
        encoder << processID.value;
    }

    static bool decode(ArgumentDecoder& decoder, ProcessIdentifier& processID)
    {
        if (!decoder.decode(processID.value))
            return false;
        return true;
    }
};

} // namespace IPC

#endif // ArgumentCoders_h
