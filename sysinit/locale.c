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
#include "lib/run.h"
#include "lib/file.h"
#include "lib/config.h"

#include <stdlib.h>

#ifndef UTF8
#define LEGACY
#endif

#ifndef LOCALE
#define LOCALE "en_US"
#define LEGACY
#endif

int locale_callback(char *filename);


const char *locale_get_name(void)
{
    static const char priv_locale_name[] = "locale";
    
    return priv_locale_name;
}

int locale_initialization(void)
{
#ifdef KEYMAP
#ifdef UTF8
    char * const loadkeys_arg[] = {CMD_LOADKEYS, "-q", "-u", KEYMAP, NULL};
#endif /* UTF8 */
#ifdef LEGACY
    char * const loadkeys_arg[] = {CMD_LOADKEYS, "-q", "", KEYMAP, NULL};
#endif /* LEGACY */
#endif /* KEYMAP */

    libspeedy_file_empty(FILE_PROFILE_LOCALE);
    libspeedy_file_chmod(FILE_PROFILE_LOCALE, 0755);
    libspeedy_file_write(FILE_PROFILE_LOCALE, "export LANG=" LOCALE "\n");

    libspeedy_file_tty_action(locale_callback);

#ifdef UTF8
    libspeedy_file_append(FILE_PROFILE_LOCALE, "if [ \"$CONSOLE\" = \"\" -a \"$TERM\" = "
               "\"linux\" -a -t 1 ]; then printf \"\\033%%G\"; fi");
#endif /* UTF8 */
#ifdef LEGACY
    libspeedy_file_append(FILE_PROFILE_LOCALE, "if [ \"$CONSOLE\" = \"\" -a \"$TERM\" = "
               "\"linux\" -a -t 1 ]; then printf \"\\033%%@\"; fi");
#endif /* LEGACY */
#ifdef KEYMAP
    libspeedy_run(loadkeys_arg);
#endif /* KEYMAP */
    return 0;
}

#ifdef UTF8
int locale_callback(char *filename)
{
    char * kbdmode_arg[] = {CMD_KBDMODE, "-u", filename, NULL};

    libspeedy_run(kbdmode_arg);
    libspeedy_file_write(filename, "\033%G");

    return 0;
}
#endif /* UTF8 */

#ifdef LEGACY
int locale_callback(char *filename)
{
    char * kbdmode_arg[] = {CMD_KBDMODE, "-a", filename, NULL};

    libspeedy_run(kbdmode_arg);
    libspeedy_file_write(filename, "\033%@");

    return 0;
}
#endif /* LEGACY */
