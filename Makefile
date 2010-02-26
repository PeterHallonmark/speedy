override CFLAGS += -Wall

# This is the array that simulates the rest of the rc.conf
# So far it doesn't contain that much.
sysinit := start udev loopback fsck mount swap hwclock randomseed files hostname locale end

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons := alsa

build := build

export CFLAGS
export sysinit
export daemons

# The project is now built inside a build directory. 
# This has mostly beenn changed to make it possible to override
# the C implementations of daemons in case we still need to run 
# the bash implementations. 
.DEFAULT: release

all: release
		
copy: $(build)
	cp core/Makefile $(build) -u
	cp core $(build) -r -u
	cp sysinit $(build) -r -u
	cp daemons $(build) -r -u
	cp lib $(build) -r -u

$(build) :
	mkdir -p $(build)
				
release: copy
	$(MAKE) -r -C $(build) release
	
debug: CFLAGS += -rdynamic -g
debug: copy
	$(MAKE) -r -C $(build) debug
	
clean : 
	rm -rf $(build)

.NOTPARALLEL: $(build) copy

.PHONY: clean all release debug copy
