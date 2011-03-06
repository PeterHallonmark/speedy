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

# The project is built inside a build directory. 
build := build

# Variables that are going to be exported.
export CFLAGS
export system

release: target := release
release: copy build_$(target)

init:
	mkdir -p $(build)
	mkdir -p $(build)/release
	mkdir -p $(build)/debug
	mkdir -p $(build)/unittest
	mkdir -p $(build)/lcov
	mkdir -p $(build)/trucov
	
copy: init
	cp src/Makefile $(build)/$(target) -u
	cp src $(build)/$(target) -r -u

test_copy: init
	cp test/Makefile $(build)/unittest -u
	cp test $(build)/unittest -r -u
	cp src $(build)/unittest -r -u

debug: target := debug
debug: CFLAGS += -Wextra -g
debug: copy build_$(target)

test: target := unittest
test: test_copy build_$(target) 

all:
	$(MAKE) -j 1 -r -C . release
	$(MAKE) -j 1 -r -C . debug
	$(MAKE) -j 1 -r -C . test

build_$(target):
	$(MAKE) -j 4 -r -C $(build)/$(target) $(target)

clean : 
	rm -rf $(build)

loc : clean
	cloc .

lcov: test
	lcov -d build/unittest -z
	build/unittest/unittest
	lcov -d build/unittest -b build/unittest -c -o build/lcov/unittest.info
	genhtml -o build/lcov build/lcov/unittest.info

trucov: test
	find build/unittest -name "*.gcda" -exec rm {} \;
	build/unittest/unittest
	trucov report --chdir build/unittest --output ../trucov

.NOTPARALLEL: copy init clean all

.PHONY: init clean all release debug copy loc lcov build_$(target)

.SUFFIXES:
