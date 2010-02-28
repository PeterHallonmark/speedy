#!/bin/bash

# Copyright (c) 2010, Peter Johansson <zeronightfall@gmx.com>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

add_define()
{
    filename=$1
    if [ -n "$3" ]; then 
        printf "#define %s \"%s\"\n\n" $2 $3 >> $filename
    fi
}

create_start()
{
    filename=$1
    tmpfile="tmp/sysinit_start.tmp"
    
    printf "/* This is a generated file. */\n\n" > $filename
    grep RTC_MAJOR= /etc/rc.sysinit > $tmpfile
    . $tmpfile
    add_define $filename "RTC_MAJOR" $RTC_MAJOR
}

create_start $@