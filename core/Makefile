files := $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c) \
         lib/run.c lib/file.c lib/dir.c lib/network.c core/core.c

header_files := $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach init_item, $(sysinit), sysinit/config/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h)  \
                core/config_sysinit.h core/config_daemons.h
                
sysinit/config/%.h:
	@touch $@

lib/%.o: lib/%.c lib/%.h
	$(CC) $(CFLAGS) -I. -c $< -o $@
    
sysinit/%.o: sysinit/%.c lib/*.h sysinit/%.h sysinit/config/%.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

daemons/%.o: daemons/%.c lib/*.h daemons/%.h
	$(CC) $(CFLAGS) -I. -c $< -o $@    
    
core/%.o: core/%.c core/*.h ../Makefile
	$(CC) $(CFLAGS) -I. -c $< -o $@

speedy : $(header_files) $(files) $(files:.c=.o)
	cc -o speedy $(files:.c=.o)

debug : .status/debug speedy

release : .status/release speedy .status/strip

init : .status/init

reinit : clean init

clean :
	rm -rf .status
	rm -f $(files:.c=.o)

.status :
	mkdir $@

.status/debug :
	if file .status/release; \
	then make -j 1 reinit; \
	else make -j 1 init; \
	fi
	@touch $@
    
.status/release :
	if file .status/debug; \
	then make -j 1 reinit; \
	else make -j 1 init; \
	fi
	@touch $@   
    
.status/strip : 
	strip speedy
	@touch $@
        
.status/init : .status ../Makefile
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)
	@touch $@
    
.PHONY: debug release init reinit clean
.NOTPARALLEL: .status .status/init .status/release .status/debug .status/strip clean
.SILENT: .status .status/release .status/debug clean
