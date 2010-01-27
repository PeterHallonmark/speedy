override CFLAGS += -Wall

# This is the array that simulates the rest of the rc.conf
# So far it doesn't contain that much.
sysinit := start udev loopback fsck mount swap hwclock randomseed

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons :=

export CFLAGS
export sysinit
export daemons

# The project is now built inside a build directory. 
# This has mostly beenn changed to make it possible to override
# the C implementations of daemons in case we still need to run 
# the bash implementations. 
build := build

.DEFAULT: release

all: release
	
release : $(build) copy init
	$(MAKE) -j4 -C $(build) release

debug: CFLAGS += -rdynamic -g
debug: $(build) copy  init
	$(MAKE) -j4 -C $(build) debug

simulate : CFLAGS += -DSIMULATE
simulate : $(build) copy init
	$(MAKE) -j4 -C $(build) simulate

copy:
	cp core/Makefile $(build) -u
	cp core $(build) -r -u
	cp sysinit $(build) -r -u
	cp daemons $(build) -r -u
	cp lib $(build) -r -u
	
$(build) :
	mkdir $(build)
	
clean : 
	rm -rf $(build)

init :
	$(MAKE) -C $(build) init
    
.PHONY: clean all release debug copy simulate init
.NOTPARALLEL: $(build) init
