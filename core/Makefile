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


files := core/core.c core/queue.c core/observer.c core/subject.c \
         core/hash.c core/hash_lookup.c core/task_handler.c core/task.c \
         core/thread_pool.c core/task_parser.c core/config_parser.c
    
debug: speedy

release: speedy
		    
core/%.o: core/%.c
	$(CC) $(CFLAGS) -I. -c $< -o $@
        
speedy : $(files:.c=.o)
	$(CC) -o speedy -lpthread $(files:.c=.o)
	
.PHONY: debug release clean build

.SILENT:
.SUFFIXES:
