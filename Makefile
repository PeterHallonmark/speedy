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


override CFLAGS += -Wall

# This is the array that simulates the rest of the rc.conf
# So far it doesn't contain that much.
sysinit := test_thread1 test_thread2 test_thread3 \
           test_thread4 test_thread5 test_thread6
#start udev loopback fsck mount swap hwclock randomseed files \
#           hostname locale end

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons :=

# Specifies the system and which predefined scripts and configs that are
# going to be used. 
system := archlinux

# The project is built inside a build directory. 
build := build

# Variables that are going to be exported.
export CFLAGS
export sysinit
export daemons
export system

.DEFAULT: release

all: release

init:
	@./scripts/$(system)/init.sh        
		
copy: $(build)
	cp scripts $(build) -r -u
	cp scripts/Makefile $(build) -u
	cp core $(build) -r -u
	cp sysinit $(build) -r -u
	cp daemons $(build) -r -u
	cp lib $(build) -r -u
    
$(build) :
	mkdir -p $(build)
			           
release: init copy
	$(MAKE) -j 1 -r -C $(build) init_release
	$(MAKE) -j 4 -r -C $(build) release
	
debug: CFLAGS += -rdynamic -g
debug: init copy
	$(MAKE) -j 1 -r -C $(build) init_debug
	$(MAKE) -j 4 -r -C $(build) debug

build_objects: copy
	$(MAKE) -r -C $(build) build_objects
	
clean : 
	rm -rf $(build)

.NOTPARALLEL: $(build) copy init

.PHONY: init clean all release debug copy build_objects

.SUFFIXES: