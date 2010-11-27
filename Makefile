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

# This is the array that simulates the DAEMONS array in the rc.conf 

# Specifies the system and which predefined scripts and configs that are
# going to be used. 
system := archlinux

# The project is built inside a build directory. 
build := build

# Variables that are going to be exported.
export CFLAGS
export system

.DEFAULT: release

all: release

init:
	mkdir -p $(build)
	mkdir -p $(build)/release
	mkdir -p $(build)/debug
	mkdir -p $(build)/test
	
copy: init
	cp src/Makefile $(build)/$(target) -u
	cp src $(build)/$(target) -r -u

test_copy: init
	cp test/Makefile $(build)/test -u
	cp test $(build)/test -r -u
	
release: target := release
release: build_$(target)

debug: target := debug
debug: CFLAGS += -Wextra -g
debug: build_$(target)

test: target := test
test: test_copy build_$(target)

build_$(target): init copy
	$(MAKE) -j 4 -r -C $(build)/$(target) $(target)

clean : 
	rm -rf $(build)

loc : clean
	cloc .

.NOTPARALLEL: copy init clean $(build) $(build)/$(target)

.PHONY: init clean all release debug copy cloc build_$(target)

.SUFFIXES:
