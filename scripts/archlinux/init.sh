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

function_path=`echo "$0" | sed 's/init.sh/..\/functions/g'`
. $function_path
. /etc/rc.conf

create_locale()
{
    echo locale

    filename=$1
    printf "/* This is a generated file. */\n\n" > $filename
    printf "#define LOCALE \"%s\"\n\n" $LOCALE >> $filename
    printf "#define KEYMAP \"%s\"\n\n" $KEYMAP >> $filename
    
    if echo "$LOCALE" | /bin/grep -qi utf ; then
        printf "#define UTF8\n" >> $filename
    else
        printf "#define LEGACY\n" >> $filename    
    fi
}

main()
{
    printf "\nBuilding speedy for Arch Linux...\n\n"

    create_locale "sysinit/config/locale.h.tmp"
    update_file "sysinit/config/locale.h" "sysinit/config/locale.h.tmp"
}

main $*
