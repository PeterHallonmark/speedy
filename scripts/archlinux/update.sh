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

function_path=`echo "$0" | sed 's/update.sh/..\/functions/g'`
. $function_path

update()
{
    update_file "sysinit/locale.h" "sysinit/locale.h.tmp"
    update_file "sysinit/hwclock.h" "sysinit/hwclock.h.tmp"
    update_file "sysinit/fsck.h" "sysinit/fsck.h.tmp"
    update_file "sysinit/mount.h" "sysinit/mount.h.tmp"
    update_file "sysinit/hostname.h" "sysinit/hostname.h.tmp"    
    update_file "sysinit/nisdomainname.h" "sysinit/nisdomainname.h.tmp"        
    update_file "sysinit/start.h" "sysinit/start.h.tmp"        
}

update $@