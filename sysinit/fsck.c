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

#include "config/fsck.h"
#include "fsck.h"
#include "lib/run.h"

#include <sys/mount.h> 
#include <string.h>
#include <stdlib.h>

void fsck_reboot(void)
{
	exit(1);
}

const char *fsck_get_name(void)
{
    static const char priv_fsck_name[] = "fsck";
    
    return priv_fsck_name;
}

void fsck_init(void)
{    
    int fsck_ret;
    
    /* Mount root file system as read only. */
    system("/bin/mount -n -o remount,ro /");
    
    
    if (file_exists("/forcefsck")) {
        fsck_ret = system("/sbin/fsck -A -T -C -a -t "NETFS" -- -f >/dev/stdout 2>/dev/null");
    } else {
        fsck_ret = system("/sbin/fsck -A -T -C -a -t "NETFS" >/dev/stdout 2>/dev/null");
    }

    if (fsck_ret == 2) {
        printf("\n********************** REBOOT REQUIRED *********************\n");
		printf("*                                                          *\n");
		printf("* The system will be rebooted automatically in 15 seconds. *\n");
		printf("*                                                          *\n");
		printf("************************************************************\n\n");
		
        system("/bin/sleep 15");
        fsck_reboot();
        
    } else if ((fsck_ret > 1) && (fsck_ret == 32)) {
        printf("\n*****************  FILESYSTEM CHECK FAILED  ****************\n");
		printf("*                                                          *\n");
		printf("*  Please repair manually and reboot. Note that the root   *\n");
		printf("*  file system is currently mounted read-only. To remount  *\n");
		printf("*  it read-write type: mount -n -o remount,rw /            *\n");
		printf("*  When you exit the maintenance shell the system will     *\n");
		printf("*  reboot automatically.                                   *\n");
		printf("*                                                          *\n");
		printf("************************************************************\n\n");

        system("/sbin/sulogin -p");
        fsck_reboot();
    }
}
