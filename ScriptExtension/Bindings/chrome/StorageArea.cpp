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
#include "StorageArea.h"

#include "Chrome.h"
#include "InjectedBundlePage.h"
#include "JSStorageArea.h"
#include "StorageAreaHostMessages.h"
#include "ScriptExtension.h"
#include <JavaScriptCore/JSRetainPtr.h>

namespace Extensions {
namespace chrome {

StorageArea::StorageArea(Chrome& chrome)
    : m_chrome(chrome)
{
}

JSObjectRef StorageArea::wrap(JSContextRef context)
{
    return JSObjectMake(context, JSStorageArea::storageAreaClass(), this);
}

void StorageArea::get(RefPtr<JSValue> keys, RefPtr<JSFunction> callback)
{
    RefPtr<API::Dictionary> key;
    if (keys->isString()) {
        WKStringRef keyString = WKStringCreateWithUTF8CString(keys->toAPIString()->utf8().data());
        WKTypeRef values = nullptr;
        key = API::Object::create<API::Dictionary>(WKDictionaryCreate(&keyString, &values, 1));
    } else if (keys->isObject()) {
        key = keys->toAPIDictionary();
    } else
        key = nullptr;

    m_chrome.connection().send(Messages::StorageAreaHost::Get(*key, *toAPI(callback)), 0);
}

void StorageArea::getBytesInUse(RefPtr<JSValue> keys, RefPtr<JSFunction> callback)
{
    RefPtr<API::Dictionary> key;
    if (keys->isString()) {
        WKStringRef keyString = WKStringCreateWithUTF8CString(keys->toAPIString()->utf8().data());
        key = API::Object::create<API::Dictionary>(WKDictionaryCreate(&keyString, 0, 1));
    }
    else
        key = keys->toAPIDictionary();

    m_chrome.connection().send(Messages::StorageAreaHost::GetBytesInUse(*key, *toAPI(callback)), 0);
}

void StorageArea::set(RefPtr<JSDictionary> items, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::StorageAreaHost::Set(*toAPI(items), *toAPI(callback)), 0);
}

void StorageArea::remove(RefPtr<JSArray> keys, RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::StorageAreaHost::Remove(*toAPI(keys), *toAPI(callback)), 0);
}

void StorageArea::clear(RefPtr<JSFunction> callback)
{
    m_chrome.connection().send(Messages::StorageAreaHost::Clear(*toAPI(callback)), 0);
}
    
}
}
