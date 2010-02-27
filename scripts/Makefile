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


files := $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c) \
         lib/run.c lib/dir.c lib/file.c lib/network.c core/core.c

header_files := $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach init_item, $(sysinit), sysinit/config/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h)
   
init: .status
                
reinit: clean .status

debug: .status/debug speedy

release: .status/release speedy .status/strip 

build_objects: .status/release object_test

clean :
	rm -rf .status
	rm -f $(files:.c=.o)

.status :
	@mkdir $@
    
.status/debug: target := debug
.status/debug: .status
	scripts/if_file_exist.sh .status/release $(MAKE) -r reinit
	@touch $@
	
.status/release: target := release
.status/release: .status
	scripts/if_file_exist.sh .status/debug $(MAKE) -r reinit
	@touch $@
	
.status/init: ../Makefile .status/$(target)
	scripts/$(system)/init.sh
	scripts/gen_config.sh core/core.o core/config_sysinit.h sysinit $(sysinit)
	scripts/gen_config.sh core/core.o core/config_daemons.h daemons $(daemons)
	@touch $@
	
.status/strip: .status speedy
	strip speedy
	@touch $@
	
daemons/%.c:
	@touch $@
	
daemons/%.h:
	@touch $@
		
sysinit/config/%.h:
	@touch $@

sysinit/%.o: sysinit/%.h sysinit/config/%.h lib/*.h

daemons/%.o: daemons/%.h lib/*.h 

core/%.o: .status/init

lib/%.o: lib/*.h

speedy : .status/init $(header_files) $(files:.c=.o) 
	$(CC) -o speedy $(files:.c=.o)

object_test : .status/init core/*.o lib/*.o sysinit/*.o \
              core/*.h lib/*.h sysinit/*.h
	$(CC) -o speedy_test core/*.o lib/*.o sysinit/*.o
	
%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@
	
.PHONY: debug release init reinit clean build_objects

.NOTPARALLEL: .status .status/release .status/debug .status/strip reinit clean

.SILENT: .status .status/release .status/debug
