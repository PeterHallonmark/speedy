files := $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c) \
         lib/run.c core/core.c

header_files := $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h)

         
lib/%.o : lib/%.c lib/%.h
	$(CC) -c $(CFLAGS) $< -o $@
    
sysinit/%.o : sysinit/%.c sysinit/%.h sysinit/config/%.h lib/*.h
	$(CC) -I. -c $(CFLAGS) $< -o $@

daemons/%.o : daemons/%.c daemons/%.h lib/*.h
	$(CC) -I. -c $(CFLAGS) $< -o $@    
    
core/%.o : core/%.c core/*.h ../Makefile
	$(CC) -c $(CFLAGS) $< -o $@

# Rule to generate the config.h file
gen_config.sh: ../Makefile
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)
    
test : ../Makefile gen_config.sh $(header_files) $(files) $(files:.c=.o)
	cc -o test $(files:.c=.o)

debug : test

release : test
		
.PHONY: debug release
