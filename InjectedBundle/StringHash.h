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

#ifndef StringHash_h
#define StringHash_h

#include <string>
#include <wtf/HashTraits.h>
#include <wtf/Hasher.h>

namespace Extensions {

struct StringHash {
    static unsigned hash(const std::string& a) { return StringHasher::computeHash(reinterpret_cast<const unsigned char*>(a.c_str()), a.size()); }
    static bool equal(const std::string& a, const std::string& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = true;
};

} // namespace Extensions

namespace WTF {

template<typename T> struct DefaultHash;

template<> struct DefaultHash<std::string> {
    typedef Extensions::StringHash Hash;
};

template<> struct HashTraits<std::string> : GenericHashTraits<std::string> {
    static const bool emptyValueIsZero = false;
    static const bool hasIsEmptyValueFunction = true;
    static void constructDeletedValue(std::string& slot) { new (NotNull, std::addressof(slot)) std::string(1, 255); }
    static bool isDeletedValue(const std::string& value) { return value[0] == 255; }
    static bool isEmptyValue(const std::string& value) { return value.empty(); }
};

} // namespace WTF

#endif // StringHash_h
