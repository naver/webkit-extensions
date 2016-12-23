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

#ifndef InjectedBundlePageResourceLoaderClient_h
#define InjectedBundlePageResourceLoaderClient_h

#include <WebKit/WKBundlePage.h>
#include <WebKit/WKURLRequest.h>

namespace Extensions {

class InjectedBundlePageResourceLoaderClient {
public:
    InjectedBundlePageResourceLoaderClient() = default;
    virtual ~InjectedBundlePageResourceLoaderClient() = default; 
    
    virtual void didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, bool pageIsProvisionallyLoading) = 0;
    virtual WKURLRequestRef willSendRequestForFrame(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLRequestRef, WKURLResponseRef redirectResponse) = 0;
    virtual void didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKURLResponseRef) = 0;
    virtual void didReceiveContentLengthForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, uint64_t contentLength) = 0;
    virtual void didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) = 0;
    virtual void didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier, WKErrorRef) = 0;
    virtual bool shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) = 0;
    virtual bool shouldUseCredentialStorage(WKBundlePageRef, WKBundleFrameRef, uint64_t resourceIdentifier) = 0; 
};

} // namespace Extensions

#endif // InjectedBundlePageResourceLoaderClient_h