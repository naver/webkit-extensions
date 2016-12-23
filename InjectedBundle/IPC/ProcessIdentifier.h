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

#ifndef ProcessIdentifier_h
#define ProcessIdentifier_h

#include <wtf/HashTraits.h>
#include <wtf/Hasher.h>

namespace IPC {

struct ProcessIdentifier {
    uint64_t value;
    ProcessIdentifier() : value(std::numeric_limits<uint64_t>::max()) { }
    ProcessIdentifier(uint64_t v) : value(v) { }
    ProcessIdentifier(WTF::HashTableDeletedValueType) : value(std::numeric_limits<uint64_t>::max() - 1) { }
    operator bool() const { return value != std::numeric_limits<uint64_t>::max(); }
    friend bool operator==(const ProcessIdentifier& a, const ProcessIdentifier& b) { return a.value == b.value; }
    friend bool operator==(uint64_t a, const ProcessIdentifier& b) { return a == b.value; }
    friend bool operator==(const ProcessIdentifier& a, uint64_t b) { return a.value == b; }
};

struct ProcessIdentifierHash {
    static unsigned hash(const ProcessIdentifier& a) { return IntHash<uint64_t>::hash(a.value); }
    static bool equal(const ProcessIdentifier& a, const ProcessIdentifier& b) { return a.value == b.value; }
    static const bool safeToCompareToEmptyOrDeleted = true;
};

ProcessIdentifier currentProcess();

} // namespace IPC

namespace WTF {

template<typename T> struct DefaultHash;

template<> struct DefaultHash<IPC::ProcessIdentifier> {
    typedef IPC::ProcessIdentifierHash Hash;
};

template<> struct HashTraits<IPC::ProcessIdentifier> : GenericHashTraits<IPC::ProcessIdentifier> {
    static const bool emptyValueIsZero = false;
    static const bool hasIsEmptyValueFunction = true;
    static void constructDeletedValue(IPC::ProcessIdentifier& slot) { slot.value = std::numeric_limits<uint64_t>::max() - 1; }
    static bool isDeletedValue(const IPC::ProcessIdentifier& value) { return value.value == std::numeric_limits<uint64_t>::max() - 1; }
    static bool isEmptyValue(const IPC::ProcessIdentifier& value) { return value.value == std::numeric_limits<uint64_t>::max(); }
};

} // namespace WTF

using IPC::ProcessIdentifier;

#endif // ProcessIdentifier_h
