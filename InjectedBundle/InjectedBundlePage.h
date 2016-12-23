/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#ifndef InjectedBundlePage_h
#define InjectedBundlePage_h

#include "Connection.h"
#include "Function.h"
#include "MessageReceiverMap.h"
#include "ScriptExtension.h"
#include <WebKit/WKBundlePage.h>
#include <WebKit/WKBundleScriptWorld.h>
#include <WebKit/WKRetainPtr.h>

namespace Extensions {

class InjectedBundlePageResourceLoaderClient;

class InjectedBundlePage : public IPC::MessageReceiver {
public:
    InjectedBundlePage(WKBundlePageRef);
    ~InjectedBundlePage();

    void initialize();

    WKBundlePageRef page() const { return m_page; }
    uint64_t pageID() const { return m_pageID; }
    
    void setResourceLoaderClient(InjectedBundlePageResourceLoaderClient*);

    IPC::Connection& connection() { return m_connection; }
    void addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver&);
    void addMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID, IPC::MessageReceiver&);
    void removeMessageReceiver(IPC::StringReference messageReceiverName);
    void removeMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID);

    void dispatchCallback(const API::Function&, const API::Array&);

    void didReceiveMessage(WKStringRef, WKTypeRef);

private:
    void dispatchMessage(IPC::Connection&, IPC::MessageDecoder&);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::MessageDecoder&) override;

    void dispatchFunction(const API::Function&, const API::Array&);

    // Loader Client
    static void didStartProvisionalLoadForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didReceiveServerRedirectForProvisionalLoadForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didFailProvisionalLoadWithErrorForFrame(WKBundlePageRef, WKBundleFrameRef, WKErrorRef, WKTypeRef*, const void*);
    static void didCommitLoadForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didFinishLoadForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didFinishProgress(WKBundlePageRef, const void*);
    static void didFinishDocumentLoadForFrame(WKBundlePageRef, WKBundleFrameRef,  WKTypeRef*, const void*);
    static void didFailLoadWithErrorForFrame(WKBundlePageRef, WKBundleFrameRef, WKErrorRef, WKTypeRef*, const void*);
    static void didReceiveTitleForFrame(WKBundlePageRef, WKStringRef title, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didClearWindowForFrame(WKBundlePageRef, WKBundleFrameRef, WKBundleScriptWorldRef, const void*);
    static void didCancelClientRedirectForFrame(WKBundlePageRef, WKBundleFrameRef, const void*);
    static void willPerformClientRedirectForFrame(WKBundlePageRef, WKBundleFrameRef, WKURLRef url, double delay, double date, const void*);
    static void didSameDocumentNavigationForFrame(WKBundlePageRef, WKBundleFrameRef, WKSameDocumentNavigationType, WKTypeRef*, const void*);
    static void didHandleOnloadEventsForFrame(WKBundlePageRef, WKBundleFrameRef, const void*);
    static void didDisplayInsecureContentForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didRunInsecureContentForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);
    static void didDetectXSSForFrame(WKBundlePageRef, WKBundleFrameRef, WKTypeRef*, const void*);

    void didStartProvisionalLoadForFrame(WKBundleFrameRef);
    void didReceiveServerRedirectForProvisionalLoadForFrame(WKBundleFrameRef);
    void didFailProvisionalLoadWithErrorForFrame(WKBundleFrameRef, WKErrorRef);
    void didCommitLoadForFrame(WKBundleFrameRef);
    void didFinishLoadForFrame(WKBundleFrameRef);
    void didFinishProgress();
    void didFailLoadWithErrorForFrame(WKBundleFrameRef, WKErrorRef);
    void didReceiveTitleForFrame(WKStringRef title, WKBundleFrameRef);
    void didClearWindowForFrame(WKBundleFrameRef, WKBundleScriptWorldRef);
    void didCancelClientRedirectForFrame(WKBundleFrameRef);
    void willPerformClientRedirectForFrame(WKBundlePageRef, WKBundleFrameRef, WKURLRef, double delay, double date);
    void didSameDocumentNavigationForFrame(WKBundleFrameRef, WKSameDocumentNavigationType);
    void didFinishDocumentLoadForFrame(WKBundleFrameRef);
    void didHandleOnloadEventsForFrame(WKBundleFrameRef);
    void didDisplayInsecureContentForFrame(WKBundleFrameRef);
    void didRunInsecureContentForFrame(WKBundleFrameRef);
    void didDetectXSSForFrame(WKBundleFrameRef);

    // WKBundlePagePolicyClient
    static WKBundlePagePolicyAction decidePolicyForNavigationAction(WKBundlePageRef, WKBundleFrameRef, WKBundleNavigationActionRef, WKURLRequestRef, WKTypeRef*, const void*);
    static WKBundlePagePolicyAction decidePolicyForNewWindowAction(WKBundlePageRef, WKBundleFrameRef, WKBundleNavigationActionRef, WKURLRequestRef, WKStringRef frameName, WKTypeRef*, const void*);
    static WKBundlePagePolicyAction decidePolicyForResponse(WKBundlePageRef, WKBundleFrameRef, WKURLResponseRef, WKURLRequestRef, WKTypeRef*, const void*);
    static void unableToImplementPolicy(WKBundlePageRef, WKBundleFrameRef, WKErrorRef, WKTypeRef*, const void*);
    WKBundlePagePolicyAction decidePolicyForNavigationAction(WKBundlePageRef, WKBundleFrameRef, WKBundleNavigationActionRef, WKURLRequestRef, WKTypeRef*);
    WKBundlePagePolicyAction decidePolicyForNewWindowAction(WKBundlePageRef, WKBundleFrameRef, WKBundleNavigationActionRef, WKURLRequestRef, WKStringRef frameName, WKTypeRef*);
    WKBundlePagePolicyAction decidePolicyForResponse(WKBundlePageRef, WKBundleFrameRef, WKURLResponseRef, WKURLRequestRef, WKTypeRef*);
    void unableToImplementPolicy(WKBundlePageRef, WKBundleFrameRef, WKErrorRef, WKTypeRef*);

    // UI Client
    static void willAddMessageToConsole(WKBundlePageRef, WKStringRef message, uint32_t lineNumber, const void* clientInfo);
    static void willSetStatusbarText(WKBundlePageRef, WKStringRef statusbarText, const void* clientInfo);
    static void willRunJavaScriptAlert(WKBundlePageRef, WKStringRef message, WKBundleFrameRef frame, const void* clientInfo);
    static void willRunJavaScriptConfirm(WKBundlePageRef, WKStringRef message, WKBundleFrameRef frame, const void* clientInfo);
    static void willRunJavaScriptPrompt(WKBundlePageRef, WKStringRef message, WKStringRef defaultValue, WKBundleFrameRef frame, const void* clientInfo);
    static void didReachApplicationCacheOriginQuota(WKBundlePageRef, WKSecurityOriginRef, int64_t totalBytesNeeded, const void* clientInfo);
    static uint64_t didExceedDatabaseQuota(WKBundlePageRef, WKSecurityOriginRef, WKStringRef databaseName, WKStringRef databaseDisplayName, uint64_t currentQuotaBytes, uint64_t currentOriginUsageBytes, uint64_t currentDatabaseUsageBytes, uint64_t expectedUsageBytes, const void* clientInfo);
    void willAddMessageToConsole(WKStringRef message, uint32_t lineNumber);
    void willSetStatusbarText(WKStringRef statusbarText);
    void willRunJavaScriptAlert(WKStringRef message, WKBundleFrameRef);
    void willRunJavaScriptConfirm(WKStringRef message, WKBundleFrameRef);
    void willRunJavaScriptPrompt(WKStringRef message, WKStringRef defaultValue, WKBundleFrameRef);
    void didReachApplicationCacheOriginQuota(WKSecurityOriginRef, int64_t totalBytesNeeded);
    uint64_t didExceedDatabaseQuota(WKSecurityOriginRef, WKStringRef databaseName, WKStringRef databaseDisplayName, uint64_t currentQuotaBytes, uint64_t currentOriginUsageBytes, uint64_t currentDatabaseUsageBytes, uint64_t expectedUsageBytes);
    
    // Page Resource Loader Client
    static void didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, bool pageIsProvisionallyLoading, const void* clientInfo);
    static WKURLRequestRef willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, WKURLResponseRef redirectResponse, const void *clientInfo);
    static void didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLResponseRef, const void* clientInfo);
    static void didReceiveContentLengthForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, uint64_t contentLength, const void* clientInfo);
    static void didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, const void* clientInfo);
    static void didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKErrorRef, const void* clientInfo);
    static bool shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, const void* clientInfo);
    static bool shouldUseCredentialStorage(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, const void* clientInfo);       
    
#if ENABLE(FULLSCREEN_API)
    // Full Screen client
    static bool supportsFullScreen(WKBundlePageRef, WKFullScreenKeyboardRequestType);
    static void setHasCustomFullScreenBehavior(WKBundlePageRef, bool value);
    static void enterFullScreenForElement(WKBundlePageRef, WKBundleNodeHandleRef element);
    static void exitFullScreenForElement(WKBundlePageRef, WKBundleNodeHandleRef element);
    static void beganEnterFullScreen(WKBundlePageRef, WKRect initialFrame, WKRect finalFrame);
    static void beganExitFullScreen(WKBundlePageRef, WKRect initialFrame, WKRect finalFrame);
    static void closeFullScreen(WKBundlePageRef);
#endif

    // Editor client
    static bool shouldBeginEditing(WKBundlePageRef, WKBundleRangeHandleRef, const void* clientInfo);
    static bool shouldEndEditing(WKBundlePageRef, WKBundleRangeHandleRef, const void* clientInfo);
    static bool shouldInsertNode(WKBundlePageRef, WKBundleNodeHandleRef, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType, const void* clientInfo);
    static bool shouldInsertText(WKBundlePageRef, WKStringRef, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType, const void* clientInfo);
    static bool shouldDeleteRange(WKBundlePageRef, WKBundleRangeHandleRef, const void* clientInfo);
    static bool shouldChangeSelectedRange(WKBundlePageRef, WKBundleRangeHandleRef fromRange, WKBundleRangeHandleRef toRange, WKAffinityType, bool stillSelecting, const void* clientInfo);
    static bool shouldApplyStyle(WKBundlePageRef, WKBundleCSSStyleDeclarationRef style, WKBundleRangeHandleRef range, const void* clientInfo);
    static void didBeginEditing(WKBundlePageRef, WKStringRef notificationName, const void* clientInfo);
    static void didEndEditing(WKBundlePageRef, WKStringRef notificationName, const void* clientInfo);
    static void didChange(WKBundlePageRef, WKStringRef notificationName, const void* clientInfo);
    static void didChangeSelection(WKBundlePageRef, WKStringRef notificationName, const void* clientInfo);
    bool shouldBeginEditing(WKBundleRangeHandleRef);
    bool shouldEndEditing(WKBundleRangeHandleRef);
    bool shouldInsertNode(WKBundleNodeHandleRef, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType);
    bool shouldInsertText(WKStringRef, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType);
    bool shouldDeleteRange(WKBundleRangeHandleRef);
    bool shouldChangeSelectedRange(WKBundleRangeHandleRef fromRange, WKBundleRangeHandleRef toRange, WKAffinityType, bool stillSelecting);
    bool shouldApplyStyle(WKBundleCSSStyleDeclarationRef style, WKBundleRangeHandleRef range);
    void didBeginEditing(WKStringRef notificationName);
    void didEndEditing(WKStringRef notificationName);
    void didChange(WKStringRef notificationName);
    void didChangeSelection(WKStringRef notificationName);

    WKBundlePageRef m_page;
    uint64_t m_pageID;
    WKRetainPtr<WKBundleScriptWorldRef> m_world;

    IPC::ConnectionToPage m_connection;
    IPC::MessageReceiverMap m_messageReceiverMap;

    ScriptExtension m_scriptExtension;
    
    InjectedBundlePageResourceLoaderClient* m_pageResourceLoaderClient;
};

} // namespace Extensions

#endif // InjectedBundlePage_h
