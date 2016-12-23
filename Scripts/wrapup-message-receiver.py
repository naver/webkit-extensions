#!/usr/bin/env python2

# Copyright (C) 2016 Naver Corp. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os, sys
import mmap
import re

if __name__ == '__main__':
    if len(sys.argv) < 1:
        sys.exit(1)

    outpath = sys.argv[1]
    if not os.path.isfile(outpath):
        sys.exit(-1)

    inpath = sys.argv[2]
    if not os.path.isfile(inpath):
        sys.exit(-1)

    outfile = open(outpath, 'rb')
    outfiledata = outfile.read(-1)

    opener = ''.join([os.linesep, 'namespace WebKit {'])
    closer = ''.join([os.linesep, '} // namespace WebKit'])

    if re.search(opener, outfiledata) is None or re.search(closer, outfiledata) is None:
        sys.exit(0)

    infile = open(inpath, 'rb')
    infiledata = infile.read(-1)

    namespaces = []
    for match in re.finditer('namespace ([a-zA-Z]+)', infiledata):
        namespaces.append(match.group(1))

    if len(namespaces) == 0:
        sys.exit(0)

    openers = ''
    for namespace in namespaces:
        openers = os.linesep.join([openers, ' '.join(['namespace', namespace, '{'])])

    closers = ''
    for namespace in reversed(namespaces):
        closers = os.linesep.join([closers, ' '.join(['}', '//', 'namespace', namespace])])

    modified = re.sub(closer, closers, re.sub(opener, openers, outfiledata))

    if modified is None:
        sys.exit(-1)

    outfile.close()
    outfile = open(outpath, 'wb')
    outfile.write(modified)
