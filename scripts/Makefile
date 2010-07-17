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
         $(foreach test, $(tests), tests/$(test).c) \
         lib/run.c lib/dir.c lib/file.c lib/network.c lib/modprobe.c \
         core/core.c core/queue.c core/observer.c core/subject.c \
         core/hash.c core/hash_lookup.c core/task_handler.c core/task.c \
         core/thread_pool.c core/task_parser.c

header_files := $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach init_item, $(sysinit), sysinit/config/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h) \
                $(foreach test, $(tests), tests/$(test).h)
   

init_release: target := release
init_release: init .status/init

init_debug: target := debug
init_debug: init .status/init

init: sysinit/config
	$(MAKE) -r -j 4 -C scripts/$(system) update 
	cp scripts/$(system)/sysinit/*.h sysinit/config -u
	cp scripts/$(system)/config.h lib/config.h -u
    
reinit: clean init

debug: speedy

release: speedy .status/strip 

build_objects: .status/release object_test

clean :
	rm -rf .status
	rm -f $(files:.c=.o)

sysinit/config:
	@mkdir sysinit/config
    
.status :
	@mkdir $@
    
.status/debug: target := debug
.status/debug: .status
	scripts/if_file_exist.sh .status/release $(MAKE) -r reinit
	@touch $@
	@touch .status/target
    
.status/release: target := release
.status/release: .status
	scripts/if_file_exist.sh .status/debug $(MAKE) -r reinit
	@touch $@
	@touch .status/target
    
.status/init: .status ../Makefile .status/$(target) sysinit/*.h
	scripts/gen_config.sh core/core.o core/config_sysinit.h sysinit $(sysinit)
	scripts/gen_config.sh core/core.o core/config_daemons.h daemons $(daemons)
	scripts/gen_config.sh core/core.o core/config_tests.h tests $(tests)
	@touch $@
	
.status/strip: .status speedy
	strip speedy
	@touch $@
    
daemons/%.c:
	@touch $@
	
daemons/%.h:
	@touch $@
		
sysinit/%.o: sysinit/%.c sysinit/%.h lib/*.h sysinit/config/%.h .status/$(target)
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
daemons/%.o: daemons/%.c daemons/%.h lib/*.h .status/$(target)
	$(CC) $(CFLAGS) -I. -c $< -o $@

tests/%.o: tests/%.c tests/%.h lib/*.h .status/$(target)
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
core/%.o: core/%.c .status/init .status/$(target)
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
lib/%.o: lib/%.c lib/*.c .status/$(target)
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
speedy : $(files:.c=.o) $(header_files) .status/init .status/$(target)
	$(CC) -o speedy -lpthread $(files:.c=.o)

object_test : .status/init core/*.o lib/*.o sysinit/*.o \
              core/*.h lib/*.h sysinit/*.h
	$(CC) -o speedy_test core/*.o lib/*.o sysinit/*.o
	
.PHONY: debug release init init_release init_debug reinit clean build_objects

.SILENT: .status .status/release .status/debug

.SUFFIXES:
