CFLAGS := -Wall

# This is the array that simulates the rest of the rc.conf
# So far it doesn't contain that much.
sysinit := hostname keymap

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons := alsa network samba

export CFLAGS
export sysinit
export daemons

# The project is now built inside a build directory. 
# This has mostly beenn changed to make it possible to override
# the C implementations of daemons in case we still need to run 
# the bash implementations. 
build := build

.DEFAULT: all
	
all : $(build)
	cp core/Makefile $(build)
	cp core $(build) -r -u
	cp sysinit $(build) -r -u
	cp daemons $(build) -r -u
	$(MAKE) -j4 -C $(build)

$(build) :
	mkdir $(build)
	
clean : 
	rm -rf $(build)
	
.PHONY: clean all
.NOTPARALLEL: $(build)
