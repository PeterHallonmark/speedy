/*
    Copyright (c) 2010, Peter Johansson <zeronightfall@gmx.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "locale.h"
#include "config/locale.h"
#include "lib/file.h"
#include "lib/command.h"

#include <stdlib.h>

#ifndef LOCALE
#define LOCALE "en_US"
#endif

#ifndef PROFILE_LOCALE
#define PROFILE_LOCALE "/etc/profile.d/locale.sh"
#endif

const char *locale_get_name(void)
{
    static const char priv_locale_name[] = "locale";
    
    return priv_locale_name;
}

void locale_init(void)
{    
    file_empty(PROFILE_LOCALE);
    file_chmod(PROFILE_LOCALE, 0755);
    file_write(PROFILE_LOCALE, "export LANG=" LOCALE "\n");
}
