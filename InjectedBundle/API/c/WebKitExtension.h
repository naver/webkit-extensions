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

#ifndef WebKitExtension_h
#define WebKitExtension_h

#include <WebKit/WKBase.h>

typedef const struct OpaqueWKView* WKViewRef;

#include <WebKit/WKView.h>

#undef API_EXPORT
#if defined(API_NO_EXPORT)
#define API_EXPORT
#elif defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC__)
#define API_EXPORT __attribute__((visibility("default")))
#elif defined(WIN32)
#if defined(ExtensionsInjectedBundle_EXPORTS)
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else /* !defined(API_NO_EXPORT) */
#define API_EXPORT
#endif /* defined(API_NO_EXPORT) */

#ifdef __cplusplus
extern "C" {
#endif

typedef const struct OpaqueWebKitExtension* WebKitExtensionRef;

typedef void (*WebKitExtensionDidCreateBundleCallback)(WebKitExtensionRef, WKContextRef context, const void *clientInfo);

// Extension Client
typedef void (*WebKitExtensionDidCreatePageCallback)(WebKitExtensionRef, WKPageRef page, const void* clientInfo);
typedef void (*WebKitExtensionWillDestroyPageCallback)(WebKitExtensionRef, WKPageRef page, const void* clientInfo);

typedef struct WebKitExtensionClientBase {
    int                                                                 version;
    const void *                                                        clientInfo;
} WebKitExtensionClientBase;

typedef struct WebKitExtensionClientV0 {
    WebKitExtensionClientBase                                           base;

    // Version 0.
    WebKitExtensionDidCreatePageCallback                                didCreatePage;
    WebKitExtensionWillDestroyPageCallback                              willDestroyPage;
} WebKitExtensionClientV0;

// Extension Page Client
typedef void (*WebKitExtensionPageBrowserActionSetTitleCallback)(WKPageRef, WKDictionaryRef details, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionGetTitleCallback)(WKPageRef, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionSetIconCallback)(WKPageRef, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionSetPopupCallback)(WKPageRef, WKDictionaryRef details, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionGetPopupCallback)(WKPageRef, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionSetBadgeTextCallback)(WKPageRef, WKDictionaryRef details, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionGetBadgeTextCallback)(WKPageRef, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionSetBadgeBackgroundColorCallback)(WKPageRef, WKDictionaryRef details, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionGetBadgeBackgroundColorCallback)(WKPageRef, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionEnableCallback)(WKPageRef, WKDoubleRef tabId, const void *clientInfo);
typedef void (*WebKitExtensionPageBrowserActionDisableCallback)(WKPageRef, WKDoubleRef tabId, const void *clientInfo);

typedef WKDoubleRef (*WebKitExtensionPageContextMenusCreateCallback)(WKPageRef, WKDictionaryRef createProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageContextMenusUpdateCallback)(WKPageRef, WKDoubleRef id, WKDictionaryRef updateProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageContextMenusRemoveCallback)(WKPageRef, WKDoubleRef menuItemId, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageContextMenusRemoveAllCallback)(WKPageRef, WKArrayRef *result, const void *clientInfo);
    
typedef void (*WebKitExtensionPageI18nGetAcceptLanguagesCallback)(WKPageRef, WKArrayRef *result, const void *clientInfo);
typedef WKStringRef (*WebKitExtensionPageI18nGetMessageCallback)(WKPageRef, WKStringRef messageName, WKDictionaryRef substitutions, const void *clientInfo);
typedef WKStringRef (*WebKitExtensionPageI18nGetUILanguageCallback)(WKPageRef, WKStringRef messageName, WKDictionaryRef substitutions, const void *clientInfo);
typedef void (*WebKitExtensionPageI18nDetectLanguageCallback)(WKPageRef, WKArrayRef *result, const void *clientInfo);

typedef void (*WebKitExtensionPageStorageGetCallback)(WKPageRef, WKDictionaryRef keys, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageStorageGetBytesInUseCallback)(WKPageRef, WKDictionaryRef keys, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageStorageSetCallback)(WKPageRef, WKDictionaryRef items, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageStorageRemoveCallback)(WKPageRef, WKArrayRef keys, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageStorageClearCallback)(WKPageRef, WKArrayRef *result, const void *clientInfo);

typedef void (*WebKitExtensionPageTabsGetCallback)(WKPageRef, WKDoubleRef tabId, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsGetCurrentCallback)(WKPageRef, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsConnectCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef connectInfo, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsSendRequestCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef request, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsSendMessageCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef message, WKDictionaryRef options, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsGetSelectedCallback)(WKPageRef, WKDoubleRef windowId, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsGetAllInWindowCallback)(WKPageRef, WKDoubleRef windowId, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsCreateCallback)(WKPageRef, WKDictionaryRef createProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsDuplicateCallback)(WKPageRef, WKDoubleRef tabId, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsQueryCallback)(WKPageRef, WKDictionaryRef queryInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsHighlightCallback)(WKPageRef, WKDictionaryRef highlightInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsUpdateCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef updateProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsMoveCallback)(WKPageRef, WKArrayRef tabIds, WKDictionaryRef moveProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsReloadCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef reloadProperties, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsRemoveCallback)(WKPageRef, WKArrayRef tabIds, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsDetectLanguageCallback)(WKPageRef, WKArrayRef tabIds, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsCaptureVisibleTabCallback)(WKPageRef, WKDoubleRef windowId, WKDictionaryRef options, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsExecuteScriptCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsInsertCSSCallback)(WKPageRef, WKDoubleRef tabId, WKDictionaryRef details, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsSetZoomCallback)(WKPageRef, WKDoubleRef tabId, WKDoubleRef zoomFactor, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageTabsGetZoomCallback)(WKPageRef, WKDoubleRef tabId, WKArrayRef *result, const void *clientInfo);

typedef void (*WebKitExtensionPageWindowsGetCallback)(WKPageRef, WKDoubleRef windowId, WKDictionaryRef getInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsGetCurrentCallback)(WKPageRef, WKDictionaryRef getInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsGetLastFocusedCallback)(WKPageRef, WKDictionaryRef getInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsGetAllCallback)(WKPageRef, WKDictionaryRef getInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsCreateCallback)(WKPageRef, WKDictionaryRef createData, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsUpdateCallback)(WKPageRef, WKDoubleRef windowId, WKDictionaryRef updateInfo, WKArrayRef *result, const void *clientInfo);
typedef void (*WebKitExtensionPageWindowsRemoveCallback)(WKPageRef, WKDoubleRef windowId, WKArrayRef *result, const void *clientInfo);

typedef struct WebKitExtensionPageClientBase {
    int                                                                 version;
    const void *                                                        clientInfo;
} WebKitExtensionPageClientBase;

typedef struct WebKitExtensionPageClientV0 {
    WebKitExtensionPageClientBase                                       base;

    // Version 0.
    WebKitExtensionPageBrowserActionSetTitleCallback                    browserActionSetTitle;
    WebKitExtensionPageBrowserActionGetTitleCallback                    browserActionGetTitle;
    WebKitExtensionPageBrowserActionSetIconCallback                     browserActionSetIcon;
    WebKitExtensionPageBrowserActionSetPopupCallback                    browserActionSetPopup;
    WebKitExtensionPageBrowserActionGetPopupCallback                    browserActionGetPopup;
    WebKitExtensionPageBrowserActionSetBadgeTextCallback                browserActionSetBadgeText;
    WebKitExtensionPageBrowserActionGetBadgeTextCallback                browserActionGetBadgeText;
    WebKitExtensionPageBrowserActionSetBadgeBackgroundColorCallback     browserActionSetBadgeBackgroundColor;
    WebKitExtensionPageBrowserActionGetBadgeBackgroundColorCallback     browserActionGetBadgeBackgroundColor;
    WebKitExtensionPageBrowserActionEnableCallback                      browserActionEnable;
    WebKitExtensionPageBrowserActionDisableCallback                     browserActionDisable;

    WebKitExtensionPageContextMenusCreateCallback                       contextMenusCreate;
    WebKitExtensionPageContextMenusUpdateCallback                       contextMenusUpdate;
    WebKitExtensionPageContextMenusRemoveCallback                       contextMenusRemove;
    WebKitExtensionPageContextMenusRemoveAllCallback                    contextMenusRemoveAll;

    WebKitExtensionPageI18nGetAcceptLanguagesCallback                   i18nGetAcceptLanguages;
    WebKitExtensionPageI18nGetMessageCallback                           i18nGetMessage;
    WebKitExtensionPageI18nGetUILanguageCallback                        i18nGetUILanguage;
    WebKitExtensionPageI18nDetectLanguageCallback                       i18nDetectLanguage;

    WebKitExtensionPageStorageGetCallback                               storageGet;
    WebKitExtensionPageStorageGetBytesInUseCallback                     storageGetBytesInUse;
    WebKitExtensionPageStorageSetCallback                               storageSet;
    WebKitExtensionPageStorageRemoveCallback                            storageRemove;
    WebKitExtensionPageStorageClearCallback                             storageClear;

    WebKitExtensionPageTabsGetCallback                                  tabsGet;
    WebKitExtensionPageTabsGetCurrentCallback                           tabsGetCurrent;
    WebKitExtensionPageTabsConnectCallback                              tabsConnect;
    WebKitExtensionPageTabsSendRequestCallback                          tabsSendRequest;
    WebKitExtensionPageTabsSendMessageCallback                          tabsSendMessage;
    WebKitExtensionPageTabsGetSelectedCallback                          tabsGetSelected;
    WebKitExtensionPageTabsGetAllInWindowCallback                       tabsGetAllInWindow;
    WebKitExtensionPageTabsCreateCallback                               tabsCreate;
    WebKitExtensionPageTabsDuplicateCallback                            tabsDuplicate;
    WebKitExtensionPageTabsQueryCallback                                tabsQuery;
    WebKitExtensionPageTabsHighlightCallback                            tabsHighlight;
    WebKitExtensionPageTabsUpdateCallback                               tabsUpdate;
    WebKitExtensionPageTabsMoveCallback                                 tabsMove;
    WebKitExtensionPageTabsReloadCallback                               tabsReload;
    WebKitExtensionPageTabsRemoveCallback                               tabsRemove;
    WebKitExtensionPageTabsDetectLanguageCallback                       tabsDetectLanguage;
    WebKitExtensionPageTabsCaptureVisibleTabCallback                    tabsCaptureVisibleTab;
    WebKitExtensionPageTabsExecuteScriptCallback                        tabsExecuteScript;
    WebKitExtensionPageTabsInsertCSSCallback                            tabsInsertCSS;
    WebKitExtensionPageTabsSetZoomCallback                              tabsSetZoom;
    WebKitExtensionPageTabsGetZoomCallback                              tabsGetZoom;

    WebKitExtensionPageWindowsGetCallback                               windowsGet;
    WebKitExtensionPageWindowsGetCurrentCallback                        windowsGetCurrent;
    WebKitExtensionPageWindowsGetLastFocusedCallback                    windowsGetLastFocused;
    WebKitExtensionPageWindowsGetAllCallback                            windowsGetAll;
    WebKitExtensionPageWindowsCreateCallback                            windowsCreate;
    WebKitExtensionPageWindowsUpdateCallback                            windowsUpdate;
    WebKitExtensionPageWindowsRemoveCallback                            windowsRemove;
} WebKitExtensionPageClientV0;

API_EXPORT bool WebKitExtensionInitialize(WebKitExtensionDidCreateBundleCallback, const void *clientInfo);

API_EXPORT bool WebKitExtensionLoadExtension(WebKitExtensionRef, WKDictionaryRef);
API_EXPORT void WebKitExtensionSetClient(WebKitExtensionRef, const WebKitExtensionClientBase* client);
API_EXPORT void WebKitExtensionSetPageClient(WebKitExtensionRef, WKPageRef, const WebKitExtensionPageClientBase* client);

#ifdef __cplusplus
}
#endif

#endif /* WebKitExtension_h */
