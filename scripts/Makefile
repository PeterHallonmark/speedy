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
   
init: .status sysinit/config
	$(MAKE) -r -C scripts/$(system) update 
	cp scripts/$(system)/sysinit/*.h sysinit/config -u
	cp scripts/$(system)/config.h lib/config.h -u
    
reinit: clean init

debug: init .status/debug speedy

release: init .status/release speedy .status/strip 

build_objects: .status/release object_test

clean :
	rm -rf .status
	rm -f $(files:.c=.o)

sysinit/config :
	@mkdir sysinit/config
    
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
    
sysinit/%.o: sysinit/%.c sysinit/%.h lib/*.h sysinit/config/%.h .status/init 
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
daemons/%.o: daemons/%.c daemons/%.h lib/*.h .status/init 
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
core/%.o: core/%.c .status/init
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
lib/%.o: lib/%.c lib/*.c .status/init 
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
speedy : $(files:.c=.o) $(header_files) .status/init 
	$(CC) -o speedy $(files:.c=.o)

object_test : .status/init core/*.o lib/*.o sysinit/*.o \
              core/*.h lib/*.h sysinit/*.h
	$(CC) -o speedy_test core/*.o lib/*.o sysinit/*.o
	
.PHONY: debug release init reinit clean build_objects

.NOTPARALLEL: .status .status/release .status/debug .status/strip \
              init reinit clean

.SILENT: .status .status/release .status/debug
