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

#ifndef PageResourceLoaderClient_h
#define PageResourceLoaderClient_h

#include "InjectedBundlePageResourceLoaderClient.h"

namespace Extensions {
namespace chrome {

class PageResourceLoaderClient : public InjectedBundlePageResourceLoaderClient {
public:
    PageResourceLoaderClient() = default;
    ~PageResourceLoaderClient() = default;
      
     void didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, bool pageIsProvisionallyLoading) override;
     WKURLRequestRef willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, WKURLResponseRef redirectResponse) override;
     void didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLResponseRef) override;
     void didReceiveContentLengthForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, uint64_t contentLength) override;
     void didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;
     void didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKErrorRef) override;
     bool shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;
     bool shouldUseCredentialStorage(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) override;
};

}
} // namespace Extensions

#endif // PageResourceLoaderClient_h