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
#include "PageResourceLoaderClient.h"

#include <WebKit/WKRetainPtr.h>

namespace Extensions {
namespace chrome {

void PageResourceLoaderClient::didInitiateLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, bool pageIsProvisionallyLoading)
{
}

WKURLRequestRef PageResourceLoaderClient::willSendRequestForFrame(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLRequestRef request, WKURLResponseRef redirectResponse)
{
    WKRetain(request);
    return request;
}

void PageResourceLoaderClient::didReceiveResponseForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKURLResponseRef request)
{
}

void PageResourceLoaderClient::didReceiveContentLengthForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, uint64_t contentLength)
{
}

void PageResourceLoaderClient::didFinishLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier)
{
}

void PageResourceLoaderClient::didFailLoadForResource(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier, WKErrorRef error)
{
}

bool PageResourceLoaderClient::shouldCacheResponse(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier)
{
    return true;
}

bool PageResourceLoaderClient::shouldUseCredentialStorage(WKBundlePageRef page, WKBundleFrameRef frame, uint64_t resourceIdentifier)
{
    return true;
}

}
} // namespace Extensions
