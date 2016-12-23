/*
 * Copyright (C) 2010, 2014-2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "TestController.h"

#include "Options.h"
#include "PlatformWebView.h"
#include <JavaScriptCore/InitializeThreading.h>
#include <WebKit/WKContextConfigurationRef.h>
#include <WebKit/WKContext.h>
#include <WebKit/WKContextPrivate.h>
#include <WebKit/WKPage.h>
#include <WebKit/WKPageConfigurationRef.h>
#include <WebKit/WKPageGroup.h>
#include <WebKit/WKString.h>
#include <WebKit/WKURL.h>
#include <WebKit/WKUserContentControllerRef.h>
#include <wtf/MainThread.h>
#include <wtf/RunLoop.h>
#include <wtf/TemporaryChange.h>
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>

#include <direct.h>

#if OS(WINDOWS)
#define PATH_MAX MAX_PATH
#define getcwd _getcwd
#endif

namespace TestRunner {

static TestController* controller;

static const double shortTimeout = 5.0;
static const double noTimeout = -1;

static const char* webProcessName()
{
    // FIXME: Find a way to not hardcode the process name.
    return "WebProcess";
}

static WKURLRef createTestURL(const char* pathOrURL)
{
    if (strstr(pathOrURL, "http://") || strstr(pathOrURL, "https://") || strstr(pathOrURL, "file://"))
        return WKURLCreateWithUTF8CString(pathOrURL);

    // Creating from filesytem path.
    size_t length = strlen(pathOrURL);
    if (!length)
        return 0;

    const char separator = '/';
    bool isAbsolutePath = pathOrURL[0] == separator;
    const char* filePrefix = "file://";
    static const size_t prefixLength = strlen(filePrefix);

    std::unique_ptr<char[]> buffer;
    if (isAbsolutePath) {
        buffer = std::make_unique<char[]>(prefixLength + length + 1);
        strcpy(buffer.get(), filePrefix);
        strcpy(buffer.get() + prefixLength, pathOrURL);
    } else {
        buffer = std::make_unique<char[]>(prefixLength + PATH_MAX + length + 2); // 1 for the separator
        strcpy(buffer.get(), filePrefix);
        if (!getcwd(buffer.get() + prefixLength, PATH_MAX))
            return 0;
        size_t numCharacters = strlen(buffer.get());
        buffer[numCharacters] = separator;
        strcpy(buffer.get() + numCharacters + 1, pathOrURL);
    }

    return WKURLCreateWithUTF8CString(buffer.get());
}

TestController& TestController::singleton()
{
    ASSERT(controller);
    return *controller;
}

TestController::TestController(int argc, const char* argv[])
{
    initialize(argc, argv);
    controller = this;
    run();
    controller = 0;
}

TestController::~TestController()
{
    platformDestroy();
}

void TestController::initialize(int argc, const char* argv[])
{
    JSC::initializeThreading();
    WTF::initializeMainThread();
    RunLoop::initializeMainRunLoop();

    WebKitExtensionInitialize(didCreateBundle, this);
    platformInitialize();

    Options options;
    OptionsHandler optionsHandler(options);

    if (argc < 2) {
        optionsHandler.printHelp();
        exit(1);
    }
    if (!optionsHandler.parse(argc, argv))
        exit(1);

    m_pathToBackground = options.pathToBackground;
    m_paths = options.paths;

    initializeInjectedBundlePath();

    WKRetainPtr<WKStringRef> pageGroupIdentifier(AdoptWK, WKStringCreateWithUTF8CString("ExtensionsTestRunnerPageGroup"));
    m_pageGroup.adopt(WKPageGroupCreateWithIdentifier(pageGroupIdentifier.get()));
}

WKRetainPtr<WKContextConfigurationRef> TestController::generateContextConfiguration() const
{
    auto configuration = adoptWK(WKContextConfigurationCreate());
    WKContextConfigurationSetInjectedBundlePath(configuration.get(), injectedBundlePath());
    WKContextConfigurationSetFullySynchronousModeIsAllowedForTesting(configuration.get(), true);
    return configuration;
}

WKRetainPtr<WKPageConfigurationRef> TestController::generatePageConfiguration(WKContextConfigurationRef configuration)
{
    m_context = platformAdjustContext(adoptWK(WKContextCreateWithConfiguration(configuration)).get(), configuration);

    WKContextUseTestingNetworkSession(m_context.get());
    WKContextSetCacheModel(m_context.get(), kWKCacheModelDocumentBrowser);

    platformInitializeContext();

    WKContextInjectedBundleClientV1 injectedBundleClient = {
        { 1, this },
        0, // didReceiveMessageFromInjectedBundle
        0, // didReceiveSynchronousMessageFromInjectedBundle
        getInjectedBundleInitializationUserData
    };
    WKContextSetInjectedBundleClient(m_context.get(), &injectedBundleClient.base);

    auto pageConfiguration = adoptWK(WKPageConfigurationCreate());
    WKPageConfigurationSetContext(pageConfiguration.get(), m_context.get());
    WKPageConfigurationSetPageGroup(pageConfiguration.get(), m_pageGroup.get());
    WKPageConfigurationSetUserContentController(pageConfiguration.get(), adoptWK(WKUserContentControllerCreate()).get());
    return pageConfiguration;
}

void TestController::createWebViewWithOptions()
{
    auto contextConfiguration = generateContextConfiguration();
    auto configuration = generatePageConfiguration(contextConfiguration.get());

    platformCreateWebView(configuration.get());

    m_mainWebView->didInitializeClients();
}

void TestController::run()
{
    for (size_t i = 0; i < m_paths.size(); ++i) {
        if (!runTest(m_paths[i].c_str()))
            break;
    }
}

void TestController::runUntil(bool& done, double timeoutSeconds)
{
    platformRunUntil(done, timeoutSeconds);
}

bool TestController::runTest(const char* pathOrURL)
{
    WKRetainPtr<WKURLRef> wkURL(AdoptWK, createTestURL(pathOrURL));

    createWebViewWithOptions();

    bool shouldOpenExternalURLs = false;

    runUntil(m_gotInitialResponse, shortTimeout);
    if (!m_gotInitialResponse) {
        m_errorMessage = "Timed out waiting for initial response from web process\n";
        m_webProcessIsUnresponsive = true;
        goto end;
    }
    if (m_error)
        goto end;

    WKPageLoadURL(mainWebView()->page(), wkURL.get());

    runUntil(m_gotFinalMessage, noTimeout);
    if (m_error)
        goto end;

    dumpResults();

    return true;
end:
    return false;
}

void TestController::dumpResults()
{
}

void TestController::processDidCrash()
{
    // This function can be called multiple times when crash logs are being saved on Windows, so
    // ensure we only print the crashed message once.
    if (!m_didPrintWebProcessCrashedMessage) {
        fprintf(stderr, "#CRASHED - %s\n", webProcessName());
        fflush(stderr);
        m_didPrintWebProcessCrashedMessage = true;
    }

    if (m_shouldExitWhenWebProcessCrashes)
        exit(1);
}

WKTypeRef TestController::getInjectedBundleInitializationUserData(WKContextRef context, const void* clientInfo)
{
    return static_cast<TestController*>(const_cast<void*>(clientInfo))->getInjectedBundleInitializationUserData(context);
}

extern "C" API_EXPORT WKTypeRef WKContextInitializeInjectedBundleClient(WKContextRef);

WKTypeRef TestController::getInjectedBundleInitializationUserData(WKContextRef context)
{
    return WKContextInitializeInjectedBundleClient(context);
}

void TestController::didCreateBundle(WebKitExtensionRef bundle, WKContextRef context, const void* clientInfo)
{
    TestController* controller = static_cast<TestController*>(const_cast<void*>(clientInfo));
    controller->didCreateBundle(bundle, context);
}

void TestController::didCreatePage(WebKitExtensionRef bundle, WKPageRef page, const void* clientInfo)
{
    TestController* controller = static_cast<TestController*>(const_cast<void*>(clientInfo));
    controller->didCreatePage(bundle, page);
}

void TestController::didCreateBundle(WebKitExtensionRef bundle, WKContextRef context)
{
    WebKitExtensionClientV0 client = {
        { 0, this },
        didCreatePage,
    };
    WebKitExtensionSetClient(bundle, &client.base);
}

void TestController::didCreatePage(WebKitExtensionRef bundle, WKPageRef page)
{
    WebKitExtensionPageClientV0 pageClient = {
        { 0, this },
        0, // browserActionSetTitle,
        0, // browserActionGetTitle,
        0, // browserActionSetIcon,
        0, // browserActionSetPopup,
        0, // browserActionGetPopup,
        0, // browserActionSetBadgeText,
        0, // browserActionGetBadgeText,
        0, // browserActionSetBadgeBackgroundColor,
        0, // browserActionGetBadgeBackgroundColor,
        0, // browserActionEnable,
        0, // browserActionDisable,
        0, // contextMenusCreate,
        0, // contextMenusUpdate,
        0, // contextMenusRemove,
        0, // contextMenusRemoveAll,
        0, // i18nGetAcceptLanguages,
        0, // i18nGetMessage,
        0, // i18nGetUILanguage,
        0, // i18nDetectLanguage,
        0, // storageGet,
        0, // storageGetBytesInUse,
        0, // storageSet,
        0, // storageRemove,
        0, // storageClear,
        0, // tabsGet,
        0, // tabsGetCurrent,
        0, // tabsConnect,
        0, // tabsSendRequest,
        0, // tabsSendMessage,
        0, // tabsGetSelected,
        0, // tabsGetAllInWindow,
        0, // tabsCreate,
        0, // tabsDuplicate,
        0, // tabsQuery,
        0, // tabsHighlight,
        0, // tabsUpdate,
        0, // tabsMove,
        0, // tabsReload,
        0, // tabsRemove,
        0, // tabsDetectLanguage,
        0, // tabsCaptureVisibleTab,
        0, // tabsExecuteScript,
        0, // tabsInsertCSS,
        0, // tabsSetZoom,
        0, // tabsGetZoom,
        0, // windowsGet,
        0, // windowsGetCurrent,
        0, // windowsGetLastFocused,
        0, // windowsGetAll,
        0, // windowsCreate,
        0, // windowsUpdate,
        0, // windowsRemove,
    };
    WebKitExtensionSetPageClient(bundle, page, &pageClient.base);

    m_gotInitialResponse = true;
}

} // namespace TestRunner
