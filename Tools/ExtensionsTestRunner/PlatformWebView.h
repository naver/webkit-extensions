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

#ifndef PlatformWebView_h
#define PlatformWebView_h

#include "config.h"
#include <WebKit/WKRetainPtr.h>
#include <WebKit/WKGeometry.h>

#if OS(WINDOWS)
#include <WebKit/WebKit.h>
#include <WebKit/WebKitCOMAPI.h>
typedef IWebView* PlatformWKView;
typedef HWND PlatformWindow;
#endif

namespace TestRunner {

class PlatformWebView {
public:
    PlatformWebView(WKPageConfigurationRef);
    ~PlatformWebView();

    WKPageRef page();
    PlatformWKView platformView() { return m_view; }
    PlatformWindow platformWindow() { return m_window; }
    void resizeTo(unsigned width, unsigned height);
    void focus();

    WKRect windowFrame();
    void setWindowFrame(WKRect);

    void didInitializeClients();
    
private:
    PlatformWKView m_view;
    PlatformWindow m_window;

#if OS(WINDOWS)
    PlatformWindow m_viewWindow;
#endif
};

} // namespace TestRunner

#endif // PlatformWebView_h
