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
#include "Chrome.h"

#include "BrowserAction.h"
#include "ChromeHostMessages.h"
#include "ChromeStrings.h"
#include "ChromeWindows.h"
#include "Connection.h"
#include "ContextMenus.h"
#include "Event.h"
#include "EventFactory.h"
#include "InjectedBundle.h"
#include "InjectedBundlePage.h"
#include "InjectedBundlePageProxyMessages.h"
#include "I18n.h"
#include "JSChrome.h"
#include "Runtime.h"
#include "RuntimeMessages.h"
#include "Storage.h"
#include "Tabs.h"
#include "ScriptExtension.h"
#include "WebRequest.h"
#include "WebRequestHost.h"
#include <WebKit/WKURL.h>

#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

Chrome::Chrome(ScriptExtension& extension)
    : m_extension(extension)
    , m_eventFactory(std::make_unique<EventFactory>(*this))
    , m_browserAction(adoptRef(*new BrowserAction(*this)))
    , m_windows(adoptRef(*new ChromeWindows(*this)))
    , m_contextMenus(adoptRef(*new ContextMenus(*this)))
    , m_i18n(adoptRef(*new I18n(*this)))
    , m_runtime(adoptRef(*new Runtime(*this)))
    , m_storage(adoptRef(*new Storage(*this)))
    , m_tabs(adoptRef(*new Tabs(*this)))
    , m_webRequest(adoptRef(*new WebRequest(*this)))
{
    m_extension.page().setResourceLoaderClient(this);
    m_extension.page().addMessageReceiver(Messages::Runtime::messageReceiverName(), m_runtime);
}

JSObjectRef Chrome::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSChrome::chromeClass(), this);
}

IPC::Connection& Chrome::connection() const
{
    return m_extension.page().connection();
}

void Chrome::didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, bool pageIsProvisionallyLoading)
{
}

WKURLRequestRef Chrome::willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef, uint64_t, WKURLRequestRef request, WKURLResponseRef)
{
    WKRetainPtr<WKURLRef> url = adoptWK(WKURLRequestCopyURL(request));
    WKRetainPtr<WKStringRef> urlString = adoptWK(WKURLCopyString(url.get()));
    Ref<API::Dictionary> blockingResponse = API::Object::create<API::Dictionary>();

    connection().sendSync(Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage(*ChromeStrings::chromeHost(), *ChromeStrings::scriptMessage(), 
        API::Dictionary::create(
            ChromeStrings::chromeEvent() ^ ChromeStrings::webRequestOnBeforeRequest(),
            ChromeStrings::arguments() ^ API::Array::create(API::Dictionary::create(
                API::Object::create<APIString>("url") ^ API::Object::create<APIString>(urlString.get())
                )
            )
        )),
        Messages::InjectedBundlePageProxy::BroadcastSyncScriptMessage::Reply(blockingResponse), 0);

    bool blocking = false;
    if (blockingResponse->containsKey(*ChromeStrings::cancel())) {
        RefPtr<API::Number> value(static_pointer_cast<API::Number>(blockingResponse->itemForKey(*ChromeStrings::cancel())));
        blocking = value->booleanValue();
    }

    if (blocking)
        return nullptr;

    WKRetain(request);
    return request;
}

void Chrome::didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLResponseRef)
{
}

void Chrome::didReceiveContentLengthForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, uint64_t contentLength)
{
}

void Chrome::didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier)
{
}

void Chrome::didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKErrorRef)
{
}

bool Chrome::shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier)
{
    return true;
}

bool Chrome::shouldUseCredentialStorage(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier)
{
    return true;
}

}
}
