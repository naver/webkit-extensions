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

#ifndef InjectedBundle_h
#define InjectedBundle_h

#include "InjectedBundleProcess.h"
#include "WebKitExtension.h"
#include <API/Dictionary.h>
#include <WebKit/WKRetainPtr.h>

namespace Extensions {

class InjectedBundlePage;

class InjectedBundle : public InjectedBundleProcess {
public:
    static InjectedBundle& singleton();

    void initialize(WKBundleRef, WKTypeRef initializationUserData);

    WKBundleRef bundle() const { return m_bundle; }
    WKBundlePageGroupRef pageGroup() const { return m_pageGroup; }

    size_t pageCount() const { return m_pages.size(); }
    InjectedBundlePage* page(WKBundlePageRef page) const { return m_pages.get(page); }

private:
    InjectedBundle();
    ~InjectedBundle();

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override { }

    static void didCreatePage(WKBundleRef, WKBundlePageRef, const void* clientInfo);
    static void willDestroyPage(WKBundleRef, WKBundlePageRef, const void* clientInfo);
    static void didInitializePageGroup(WKBundleRef, WKBundlePageGroupRef, const void* clientInfo);
    static void didReceiveMessage(WKBundleRef, WKStringRef messageName, WKTypeRef messageBody, const void* clientInfo);
    static void didReceiveMessageToPage(WKBundleRef, WKBundlePageRef, WKStringRef messageName, WKTypeRef messageBody, const void* clientInfo);

    void didCreatePage(WKBundlePageRef);
    void willDestroyPage(WKBundlePageRef);
    void didInitializePageGroup(WKBundlePageGroupRef);
    void didReceiveMessage(WKStringRef messageName, WKTypeRef messageBody);
    void didReceiveMessageToPage(WKBundlePageRef, WKStringRef messageName, WKTypeRef messageBody);

    void didConnect(const ProcessIdentifier&) override;

    WKBundleRef m_bundle;
    WKBundlePageGroupRef m_pageGroup;
    HashMap<WKBundlePageRef, std::unique_ptr<InjectedBundlePage>> m_pages;
};

} // namespace Extensions

#endif // InjectedBundle_h
