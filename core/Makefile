files := $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c) \
         lib/run.c core/core.c

header_files := $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach init_item, $(sysinit), sysinit/config/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h)  \
                core/config_sysinit.h core/config_daemons.h
                
sysinit/config/%.h:
	touch $@

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

debug : speedy

release : speedy

init : 
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)

.PHONY: debug release init
.NOTPARALLEL: init
