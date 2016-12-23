/*
 * Copyright (C) 2010-2016 Apple Inc. All rights reserved.
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
#include "InjectedBundlePage.h"

#include "InjectedBundle.h"
#include "InjectedBundlePageProxyMessages.h"
#include "InjectedBundlePageResourceLoaderClient.h"
#include <WebKit/WKArray.h>
#include <WebKit/WKBundle.h>
#include <WebKit/WKBundleFrame.h>
#include <WebKit/WKBundlePagePrivate.h>

namespace Extensions {

static uint64_t generatePageID()
{
    static uint64_t pageID;
    return ++pageID;
}

InjectedBundlePage::InjectedBundlePage(WKBundlePageRef page)
    : m_page(page)
    , m_pageID(generatePageID())
    , m_world(AdoptWK, WKBundleScriptWorldCreateWorld())
    , m_connection(page)
    , m_scriptExtension(*this)
{
    WKBundlePageLoaderClientV8 loaderClient = {
        { 8, this },
        didStartProvisionalLoadForFrame,
        didReceiveServerRedirectForProvisionalLoadForFrame,
        didFailProvisionalLoadWithErrorForFrame,
        didCommitLoadForFrame,
        didFinishDocumentLoadForFrame,
        didFinishLoadForFrame,
        didFailLoadWithErrorForFrame,
        didSameDocumentNavigationForFrame,
        didReceiveTitleForFrame,
        0, // didFirstLayoutForFrame
        0, // didFirstVisuallyNonEmptyLayoutForFrame
        0, // didRemoveFrameFromHierarchy
        didDisplayInsecureContentForFrame,
        didRunInsecureContentForFrame,
        didClearWindowForFrame,
        didCancelClientRedirectForFrame,
        willPerformClientRedirectForFrame,
        didHandleOnloadEventsForFrame,
        0, // didLayoutForFrame
        0, // didNewFirstVisuallyNonEmptyLayout_unavailable
        didDetectXSSForFrame,
        0, // shouldGoToBackForwardListItem
        0, // didCreateGlobalObjectForFrame
        0, // willDisconnectDOMWindowExtensionFromGlobalObject
        0, // didReconnectDOMWindowExtensionToGlobalObject
        0, // willDestroyGlobalObjectForDOMWindowExtension
        didFinishProgress, // didFinishProgress
        0, // shouldForceUniversalAccessFromLocalURL
        0, // didReceiveIntentForFrame
        0, // registerIntentServiceForFrame
        0, // didLayout
        0, // featuresUsedInPage
        0, // willLoadURLRequest
        0, // willLoadDataRequest
        0, // willDestroyFrame_unavailable
        0, // userAgentForURL
    };
    WKBundlePageSetPageLoaderClient(m_page, &loaderClient.base);

    WKBundlePagePolicyClientV0 policyClient = {
        { 0, this },
        decidePolicyForNavigationAction,
        decidePolicyForNewWindowAction,
        decidePolicyForResponse,
        unableToImplementPolicy
    };
    WKBundlePageSetPolicyClient(m_page, &policyClient.base);

    WKBundlePageUIClientV2 uiClient = {
        { 2, this },
        willAddMessageToConsole,
        willSetStatusbarText,
        willRunJavaScriptAlert,
        willRunJavaScriptConfirm,
        willRunJavaScriptPrompt,
        0, /*mouseDidMoveOverElement*/
        0, /*pageDidScroll*/
        0, /*paintCustomOverhangArea*/
        0, /*shouldGenerateFileForUpload*/
        0, /*generateFileForUpload*/
        0, /*shouldRubberBandInDirection*/
        0, /*statusBarIsVisible*/
        0, /*menuBarIsVisible*/
        0, /*toolbarsAreVisible*/
        didReachApplicationCacheOriginQuota,
        didExceedDatabaseQuota,
        0, /*plugInStartLabelTitle*/
        0, /*plugInStartLabelSubtitle*/
        0, /*plugInExtraStyleSheet*/
        0, /*plugInExtraScript*/
    };
    WKBundlePageSetUIClient(m_page, &uiClient.base);

    WKBundlePageEditorClientV1 editorClient = {
        { 1, this },
        shouldBeginEditing,
        shouldEndEditing,
        shouldInsertNode,
        shouldInsertText,
        shouldDeleteRange,
        shouldChangeSelectedRange,
        shouldApplyStyle,
        didBeginEditing,
        didEndEditing,
        didChange,
        didChangeSelection,
        0, /* willWriteToPasteboard */
        0, /* getPasteboardDataForRange */
        0  /* didWriteToPasteboard */
    };
    WKBundlePageSetEditorClient(m_page, &editorClient.base);

#if ENABLE(FULLSCREEN_API)
    WKBundlePageFullScreenClientV1 fullScreenClient = {
        { 1, this },
        supportsFullScreen,
        enterFullScreenForElement,
        exitFullScreenForElement,
        beganEnterFullScreen,
        beganExitFullScreen,
        closeFullScreen,
    };
    WKBundlePageSetFullScreenClient(m_page, &fullScreenClient.base);
#endif
}

InjectedBundlePage::~InjectedBundlePage()
{
}

void InjectedBundlePage::setResourceLoaderClient(InjectedBundlePageResourceLoaderClient* pageResourceLoaderClient)
{
    m_pageResourceLoaderClient = pageResourceLoaderClient;
    
    WKBundlePageResourceLoadClientV1 pageResourceLoadClient = {
        { 1, m_pageResourceLoaderClient },
        didInitiateLoadForResource,
        willSendRequestForFrame,
        didReceiveResponseForResource,
        didReceiveContentLengthForResource,
        didFinishLoadForResource,
        didFailLoadForResource,
        shouldCacheResponse,
        shouldUseCredentialStorage
    };
    WKBundlePageSetResourceLoadClient(m_page, &pageResourceLoadClient.base);
}

void InjectedBundlePage::initialize()
{
    m_scriptExtension.initialize();
}

void InjectedBundlePage::addMessageReceiver(IPC::StringReference messageReceiverName, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, messageReceiver);
}

void InjectedBundlePage::addMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, destinationID, messageReceiver);
}

void InjectedBundlePage::removeMessageReceiver(IPC::StringReference messageReceiverName)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName);
}

void InjectedBundlePage::removeMessageReceiver(IPC::StringReference messageReceiverName, uint64_t destinationID)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName, destinationID);
}

void InjectedBundlePage::dispatchCallback(const API::Function& callback, const API::Array& arguments)
{
    connection().send(Messages::InjectedBundlePageProxy::DispatchFunction(callback, arguments), 0);
}

void InjectedBundlePage::didReceiveMessage(WKStringRef messageName, WKTypeRef messageBody)
{
    IPC::MessageDecoder decoder(APIString(messageName), messageBody);

    if (decoder.messageReceiverName() == IPC::StringReference("IPC")) {
        if (decoder.messageName() == IPC::StringReference("SyncMessageReply"))
            m_connection.dispatchSyncReply(std::make_unique<IPC::MessageDecoder>(WTF::move(decoder)));
        return;
    }

    dispatchMessage(m_connection, decoder);
}

void InjectedBundlePage::dispatchMessage(IPC::Connection& connection, IPC::MessageDecoder& decoder)
{
    if (!m_messageReceiverMap.dispatchMessage(connection, decoder))
        didReceiveMessage(connection, decoder);
}

void InjectedBundlePage::dispatchFunction(const API::Function& function, const API::Array& arguments)
{
    function.function(nullptr)(arguments);
}

// Loader Client Callbacks

void InjectedBundlePage::didStartProvisionalLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didStartProvisionalLoadForFrame(frame);
}

void InjectedBundlePage::didReceiveServerRedirectForProvisionalLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didReceiveServerRedirectForProvisionalLoadForFrame(frame);
}

void InjectedBundlePage::didFailProvisionalLoadWithErrorForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKErrorRef error, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didFailProvisionalLoadWithErrorForFrame(frame, error);
}

void InjectedBundlePage::didCommitLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didCommitLoadForFrame(frame);
}

void InjectedBundlePage::didFinishLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didFinishLoadForFrame(frame);
}

void InjectedBundlePage::didFinishProgress(WKBundlePageRef, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didFinishProgress();
}

void InjectedBundlePage::didFinishDocumentLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didFinishDocumentLoadForFrame(frame);
}

void InjectedBundlePage::didFailLoadWithErrorForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKErrorRef error, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didFailLoadWithErrorForFrame(frame, error);
}

void InjectedBundlePage::didReceiveTitleForFrame(WKBundlePageRef page, WKStringRef title, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didReceiveTitleForFrame(title, frame);
}

void InjectedBundlePage::didClearWindowForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef world, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didClearWindowForFrame(frame, world);
}

void InjectedBundlePage::didCancelClientRedirectForFrame(WKBundlePageRef page, WKBundleFrameRef frame, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didCancelClientRedirectForFrame(frame);
}

void InjectedBundlePage::willPerformClientRedirectForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKURLRef url, double delay, double date, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willPerformClientRedirectForFrame(page, frame, url, delay, date);
}

void InjectedBundlePage::didSameDocumentNavigationForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKSameDocumentNavigationType type, WKTypeRef*, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didSameDocumentNavigationForFrame(frame, type);
}

void InjectedBundlePage::didHandleOnloadEventsForFrame(WKBundlePageRef page, WKBundleFrameRef frame, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didHandleOnloadEventsForFrame(frame);
}

void InjectedBundlePage::didDisplayInsecureContentForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didDisplayInsecureContentForFrame(frame);
}

void InjectedBundlePage::didDetectXSSForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didDetectXSSForFrame(frame);
}

void InjectedBundlePage::didRunInsecureContentForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didRunInsecureContentForFrame(frame);
}

void InjectedBundlePage::didStartProvisionalLoadForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didReceiveServerRedirectForProvisionalLoadForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didFailProvisionalLoadWithErrorForFrame(WKBundleFrameRef frame, WKErrorRef error)
{
}

void InjectedBundlePage::didCommitLoadForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didFinishProgress()
{
}

void InjectedBundlePage::didFinishLoadForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didFailLoadWithErrorForFrame(WKBundleFrameRef frame, WKErrorRef)
{
}

void InjectedBundlePage::didReceiveTitleForFrame(WKStringRef title, WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didClearWindowForFrame(WKBundleFrameRef frame, WKBundleScriptWorldRef world)
{
    m_scriptExtension.injectExtensions(WKBundleFrameGetJavaScriptContextForWorld(frame, world));
}

void InjectedBundlePage::didCancelClientRedirectForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::willPerformClientRedirectForFrame(WKBundlePageRef, WKBundleFrameRef frame, WKURLRef url, double delay, double date)
{
}

void InjectedBundlePage::didSameDocumentNavigationForFrame(WKBundleFrameRef frame, WKSameDocumentNavigationType type)
{
}

void InjectedBundlePage::didFinishDocumentLoadForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didHandleOnloadEventsForFrame(WKBundleFrameRef frame)
{
}

void InjectedBundlePage::didDisplayInsecureContentForFrame(WKBundleFrameRef)
{
}

void InjectedBundlePage::didRunInsecureContentForFrame(WKBundleFrameRef)
{
}

void InjectedBundlePage::didDetectXSSForFrame(WKBundleFrameRef)
{
}

// Policy Client Callbacks

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNavigationAction(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleNavigationActionRef navigationAction, WKURLRequestRef request, WKTypeRef* userData, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->decidePolicyForNavigationAction(page, frame, navigationAction, request, userData);
}

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNewWindowAction(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleNavigationActionRef navigationAction, WKURLRequestRef request, WKStringRef frameName, WKTypeRef* userData, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->decidePolicyForNewWindowAction(page, frame, navigationAction, request, frameName, userData);
}

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForResponse(WKBundlePageRef page, WKBundleFrameRef frame, WKURLResponseRef response, WKURLRequestRef request, WKTypeRef* userData, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->decidePolicyForResponse(page, frame, response, request, userData);
}

void InjectedBundlePage::unableToImplementPolicy(WKBundlePageRef page, WKBundleFrameRef frame, WKErrorRef error, WKTypeRef* userData, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->unableToImplementPolicy(page, frame, error, userData);
}

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNavigationAction(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleNavigationActionRef navigationAction, WKURLRequestRef request, WKTypeRef* userData)
{
    return WKBundlePagePolicyActionPassThrough;
}

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNewWindowAction(WKBundlePageRef, WKBundleFrameRef, WKBundleNavigationActionRef, WKURLRequestRef, WKStringRef, WKTypeRef*)
{
    return WKBundlePagePolicyActionUse;
}

WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForResponse(WKBundlePageRef page, WKBundleFrameRef, WKURLResponseRef response, WKURLRequestRef, WKTypeRef*)
{
    return WKBundlePageCanShowMIMEType(page, 0) ? WKBundlePagePolicyActionUse : WKBundlePagePolicyActionPassThrough;
}

void InjectedBundlePage::unableToImplementPolicy(WKBundlePageRef, WKBundleFrameRef, WKErrorRef, WKTypeRef*)
{
}

// UI Client Callbacks

void InjectedBundlePage::willAddMessageToConsole(WKBundlePageRef page, WKStringRef message, uint32_t lineNumber, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willAddMessageToConsole(message, lineNumber);
}

void InjectedBundlePage::willSetStatusbarText(WKBundlePageRef page, WKStringRef statusbarText, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willSetStatusbarText(statusbarText);
}

void InjectedBundlePage::willRunJavaScriptAlert(WKBundlePageRef page, WKStringRef message, WKBundleFrameRef frame, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willRunJavaScriptAlert(message, frame);
}

void InjectedBundlePage::willRunJavaScriptConfirm(WKBundlePageRef page, WKStringRef message, WKBundleFrameRef frame, const void *clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willRunJavaScriptConfirm(message, frame);
}

void InjectedBundlePage::willRunJavaScriptPrompt(WKBundlePageRef page, WKStringRef message, WKStringRef defaultValue, WKBundleFrameRef frame, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->willRunJavaScriptPrompt(message, defaultValue, frame);
}

void InjectedBundlePage::didReachApplicationCacheOriginQuota(WKBundlePageRef page, WKSecurityOriginRef origin, int64_t totalBytesNeeded, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didReachApplicationCacheOriginQuota(origin, totalBytesNeeded);
}

uint64_t InjectedBundlePage::didExceedDatabaseQuota(WKBundlePageRef page, WKSecurityOriginRef origin, WKStringRef databaseName, WKStringRef databaseDisplayName, uint64_t currentQuotaBytes, uint64_t currentOriginUsageBytes, uint64_t currentDatabaseUsageBytes, uint64_t expectedUsageBytes, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didExceedDatabaseQuota(origin, databaseName, databaseDisplayName, currentQuotaBytes, currentOriginUsageBytes, currentDatabaseUsageBytes, expectedUsageBytes);
}

void InjectedBundlePage::willAddMessageToConsole(WKStringRef message, uint32_t lineNumber)
{
}

void InjectedBundlePage::willSetStatusbarText(WKStringRef statusbarText)
{
}

void InjectedBundlePage::willRunJavaScriptAlert(WKStringRef message, WKBundleFrameRef)
{
}

void InjectedBundlePage::willRunJavaScriptConfirm(WKStringRef message, WKBundleFrameRef)
{
}

void InjectedBundlePage::willRunJavaScriptPrompt(WKStringRef message, WKStringRef defaultValue, WKBundleFrameRef)
{
}

void InjectedBundlePage::didReachApplicationCacheOriginQuota(WKSecurityOriginRef origin, int64_t totalBytesNeeded)
{
}

uint64_t InjectedBundlePage::didExceedDatabaseQuota(WKSecurityOriginRef origin, WKStringRef databaseName, WKStringRef databaseDisplayName, uint64_t currentQuotaBytes, uint64_t currentOriginUsageBytes, uint64_t currentDatabaseUsageBytes, uint64_t expectedUsageBytes)
{
    return std::numeric_limits<uint64_t>::max();
}

// Editor Client Callbacks

bool InjectedBundlePage::shouldBeginEditing(WKBundlePageRef page, WKBundleRangeHandleRef range, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldBeginEditing(range);
}

bool InjectedBundlePage::shouldEndEditing(WKBundlePageRef page, WKBundleRangeHandleRef range, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldEndEditing(range);
}

bool InjectedBundlePage::shouldInsertNode(WKBundlePageRef page, WKBundleNodeHandleRef node, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType action, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldInsertNode(node, rangeToReplace, action);
}

bool InjectedBundlePage::shouldInsertText(WKBundlePageRef page, WKStringRef text, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType action, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldInsertText(text, rangeToReplace, action);
}

bool InjectedBundlePage::shouldDeleteRange(WKBundlePageRef page, WKBundleRangeHandleRef range, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldDeleteRange(range);
}

bool InjectedBundlePage::shouldChangeSelectedRange(WKBundlePageRef page, WKBundleRangeHandleRef fromRange, WKBundleRangeHandleRef toRange, WKAffinityType affinity, bool stillSelecting, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldChangeSelectedRange(fromRange, toRange, affinity, stillSelecting);
}

bool InjectedBundlePage::shouldApplyStyle(WKBundlePageRef page, WKBundleCSSStyleDeclarationRef style, WKBundleRangeHandleRef range, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->shouldApplyStyle(style, range);
}

void InjectedBundlePage::didBeginEditing(WKBundlePageRef page, WKStringRef notificationName, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didBeginEditing(notificationName);
}

void InjectedBundlePage::didEndEditing(WKBundlePageRef page, WKStringRef notificationName, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didEndEditing(notificationName);
}

void InjectedBundlePage::didChange(WKBundlePageRef page, WKStringRef notificationName, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didChange(notificationName);
}

void InjectedBundlePage::didChangeSelection(WKBundlePageRef page, WKStringRef notificationName, const void* clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didChangeSelection(notificationName);
}

bool InjectedBundlePage::shouldBeginEditing(WKBundleRangeHandleRef range)
{
    return true;
}

bool InjectedBundlePage::shouldEndEditing(WKBundleRangeHandleRef range)
{
    return true;
}

bool InjectedBundlePage::shouldInsertNode(WKBundleNodeHandleRef node, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType action)
{
    return true;
}

bool InjectedBundlePage::shouldInsertText(WKStringRef text, WKBundleRangeHandleRef rangeToReplace, WKInsertActionType action)
{
    return true;
}

bool InjectedBundlePage::shouldDeleteRange(WKBundleRangeHandleRef range)
{
    return true;
}

bool InjectedBundlePage::shouldChangeSelectedRange(WKBundleRangeHandleRef fromRange, WKBundleRangeHandleRef toRange, WKAffinityType affinity, bool stillSelecting)
{
    return true;
}

bool InjectedBundlePage::shouldApplyStyle(WKBundleCSSStyleDeclarationRef style, WKBundleRangeHandleRef range)
{
    return true;
}

void InjectedBundlePage::didBeginEditing(WKStringRef notificationName)
{
}

void InjectedBundlePage::didEndEditing(WKStringRef notificationName)
{
}

void InjectedBundlePage::didChange(WKStringRef notificationName)
{
}

void InjectedBundlePage::didChangeSelection(WKStringRef notificationName)
{
}

// Page Resource Loader Client
void InjectedBundlePage::didInitiateLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, bool pageIsProvisionallyLoading, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->didInitiateLoadForResource(page, frame, resourceIdentifier, request, pageIsProvisionallyLoading);
}

WKURLRequestRef InjectedBundlePage::willSendRequestForFrame(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, WKURLResponseRef redirectResponse, const void *clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->willSendRequestForFrame(page, frame, resourceIdentifier, request, redirectResponse);
}

void InjectedBundlePage::didReceiveResponseForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLResponseRef request, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->didReceiveResponseForResource(page, frame, resourceIdentifier, request);
}

void InjectedBundlePage::didReceiveContentLengthForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, uint64_t contentLength, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->didReceiveContentLengthForResource(page, frame, resourceIdentifier, contentLength);
}

void InjectedBundlePage::didFinishLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->didFinishLoadForResource(page, frame, resourceIdentifier);
}

void InjectedBundlePage::didFailLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKErrorRef error, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->didFailLoadForResource(page, frame, resourceIdentifier, error);
}

bool InjectedBundlePage::shouldCacheResponse(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->shouldCacheResponse(page, frame, resourceIdentifier);
}

bool InjectedBundlePage::shouldUseCredentialStorage(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, const void* clientInfo)
{
    return static_cast<InjectedBundlePageResourceLoaderClient*>(const_cast<void*>(clientInfo))->shouldUseCredentialStorage(page, frame, resourceIdentifier);
}

#if ENABLE(FULLSCREEN_API)
bool InjectedBundlePage::supportsFullScreen(WKBundlePageRef pageRef, WKFullScreenKeyboardRequestType requestType)
{
    return true;
}

void InjectedBundlePage::enterFullScreenForElement(WKBundlePageRef pageRef, WKBundleNodeHandleRef elementRef)
{
}

void InjectedBundlePage::exitFullScreenForElement(WKBundlePageRef pageRef, WKBundleNodeHandleRef elementRef)
{
}

void InjectedBundlePage::beganEnterFullScreen(WKBundlePageRef, WKRect, WKRect)
{
}

void InjectedBundlePage::beganExitFullScreen(WKBundlePageRef, WKRect, WKRect)
{
}

void InjectedBundlePage::closeFullScreen(WKBundlePageRef pageRef)
{
}
#endif

} // namespace Extensions
