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

#ifndef API_PageHandle_h
#define API_PageHandle_h

#include "Object.h"
#include <WebKit/WKBundlePage.h>
#include <WebKit/WKPage.h>

namespace IPC {
class ArgumentDecoder;
class ArgumentEncoder;
}

namespace API {

class PageHandle final : public Object {
public:
    PageHandle() = default;
    explicit PageHandle(WKTypeRef);
    explicit PageHandle(WKPageRef);
    explicit PageHandle(WKBundlePageRef);
    ~PageHandle() = default;

    PageHandle(const PageHandle& other) { m_impl = other.m_impl; }
    PageHandle(PageHandle&& other) { m_impl.swap(other.m_impl); }
    PageHandle& operator=(const PageHandle& other) { m_impl = other.m_impl; return *this; }
    PageHandle& operator=(PageHandle&& other) { m_impl.swap(other.m_impl); return *this; }

    WKPageRef page() const;
    WKBundlePageRef bundlePage() const;

    void encode(IPC::ArgumentEncoder&) const;
    static bool decode(IPC::ArgumentDecoder&, PageHandle&);
};

inline PageHandle::PageHandle(WKTypeRef impl)
    : Object(impl)
{
    ASSERT(WKGetTypeID(impl) == WKPageGetTypeID() || WKGetTypeID(impl) == WKBundlePageGetTypeID());
}

inline PageHandle::PageHandle(WKPageRef page)
    : Object(page)
{
}

inline PageHandle::PageHandle(WKBundlePageRef page)
    : Object(page)
{
}

inline WKPageRef PageHandle::page() const
{
    if (typeID() != WKPageGetTypeID())
        return nullptr;

    return impl<WKPageRef>();
}

inline WKBundlePageRef PageHandle::bundlePage() const
{
    if (typeID() != WKBundlePageGetTypeID())
        return nullptr;

    return impl<WKBundlePageRef>();
}

}

#endif // API_PageHandle_h
