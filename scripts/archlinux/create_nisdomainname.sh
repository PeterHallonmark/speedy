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

. /etc/conf.d/nisdomainname

add_const_string()
{
    filename=$1
    printf "const char %s[] = \"%s\";\n\n" $2 $3 >> $filename
}

create_nisdomainname()
{
    filename=$1
    
    printf "/* This is a generated file. */\n\n" > $filename
    add_const_string $filename "nisdomainname" $NISDOMAINNAME
}

create_nisdomainname $@