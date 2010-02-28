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

add_define()
{
    filename=$1
    if [ -n "$3" ]; then 
        printf "#define %s \"%s\"\n\n" $2 $3 >> $filename
    fi
}


create_locale()
{
    filename=$1    
    printf "/* This is a generated file. */\n\n" > $filename
    
    if [ -z "$LOCALE" ]; then
        LOCALE="en_US"
    fi
    add_define $filename "LOCALE" $LOCALE
    add_define $filename "KEYMAP" $KEYMAP
    
    if echo "$LOCALE" | /bin/grep -qi utf ; then
        printf "#define UTF8\n" >> $filename
    else
        printf "#define LEGACY\n" >> $filename    
    fi
}

create_hwclock()
{
    filename=$1
    printf "/* This is a generated file. */\n\n" > $filename

    if [ "$HARDWARECLOCK" = "UTC" ]; then    
        HWCLOCK_PARAMS="$HWCLOCK_PARAMS --utc"
    elif [ "$HARDWARECLOCK" = "localtime" ]; then
        HWCLOCK_PARAMS="$HWCLOCK_PARAMS --localtime"
    else
        HWCLOCK_PARAMS=""
    fi

    add_define $filename "HWCLOCK_PARAMS" $HWCLOCK_PARAMS
    
    if [ "$TIMEZONE" != "" -a -e "/usr/share/zoneinfo/$TIMEZONE" ]; then
        printf "#define TIMEZONE \"%s\"\n\n" $TIMEZONE >> $filename
    fi
}

create_fsck()
{
    filename=$1
    tmpfile=$2
    
    printf "/* This is a generated file. */\n\n" > $filename
    
    grep NETFS= /etc/rc.sysinit > $tmpfile
    . $tmpfile
    
    add_define $filename "NETFS" $NETFS    
}

main()
{
    printf "\nBuilding speedy for Arch Linux...\n\n"
    tmpfile=scripts/archlinux/archlinux.tmp
    
    create_locale "sysinit/config/locale.h.tmp"
    update_file "sysinit/config/locale.h" "sysinit/config/locale.h.tmp"

    create_hwclock "sysinit/config/hwclock.h.tmp"
    update_file "sysinit/config/hwclock.h" "sysinit/config/hwclock.h.tmp"
    
    create_fsck "sysinit/config/fsck.h.tmp" $tmpfile
    update_file "sysinit/config/fsck.h" "sysinit/config/fsck.h.tmp"
}

main $*
