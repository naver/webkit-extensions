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

#ifndef TestController_h
#define TestController_h

#include "WebKitExtension.h"
#include <WebKit/WKBase.h>
#include <WebKit/WKRetainPtr.h>
#include <string>
#include <vector>

namespace TestRunner {

class PlatformWebView;

class TestController {
public:
    static TestController& singleton();

    TestController(int argc, const char* argv[]);
    ~TestController();

    PlatformWebView* mainWebView() { return m_mainWebView.get(); }

    WKStringRef injectedBundlePath() const { return m_injectedBundlePath.get(); }

private:
    void initialize(int, const char*[]);
    WKRetainPtr<WKPageConfigurationRef> generatePageConfiguration(WKContextConfigurationRef);
    WKRetainPtr<WKContextConfigurationRef> generateContextConfiguration() const;
    void createWebViewWithOptions();
    void run();
    void runUntil(bool& done, double timeoutSeconds);
    bool runTest(const char* pathOrURL);
    void dumpResults();
    void notifyDone();
    void processDidCrash();

    void platformInitialize();
    void platformDestroy();
    WKContextRef platformAdjustContext(WKContextRef, WKContextConfigurationRef);
    void platformInitializeContext();
    void platformCreateWebView(WKPageConfigurationRef);
    void platformRunUntil(bool& condition, double timeout);
    WKPreferencesRef platformPreferences();
    void initializeInjectedBundlePath();

    static WKTypeRef getInjectedBundleInitializationUserData(WKContextRef, const void*);
    WKTypeRef getInjectedBundleInitializationUserData(WKContextRef);

    static void didCreateBundle(WebKitExtensionRef, WKContextRef, const void*);
    static void didCreatePage(WebKitExtensionRef, WKPageRef, const void*);

    void didCreateBundle(WebKitExtensionRef, WKContextRef);
    void didCreatePage(WebKitExtensionRef, WKPageRef);

    std::string m_pathToBackground;
    std::vector<std::string> m_paths;

    WKRetainPtr<WKStringRef> m_injectedBundlePath;

    std::unique_ptr<PlatformWebView> m_mainWebView;
    WKRetainPtr<WKContextRef> m_context;
    WKRetainPtr<WKPageGroupRef> m_pageGroup;
    WKRetainPtr<WKPageConfigurationRef> m_pageConfiguration;

    bool m_gotInitialResponse { false };
    bool m_gotFinalMessage { false };
    bool m_error { false };

    bool m_didPrintWebProcessCrashedMessage { false };
    bool m_shouldExitWhenWebProcessCrashes { true };
    bool m_webProcessIsUnresponsive { false };

    std::string m_errorMessage;
};

} // namespace TestRunner

#endif // TestController_h
