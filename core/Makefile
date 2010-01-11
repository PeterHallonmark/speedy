files := core/core.c \
         $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c)

dependencies := $(foreach init_item, $(sysinit), sysinit/config/$(init_item).h) \
                $(foreach init_item, $(sysinit), sysinit/$(init_item).h) \
                $(foreach daemon, $(daemons), daemons/$(daemon).h)

.DEFAULT: test

# Rule to transform .c files into .o files
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

debug : test

release : test
	
test : Makefile init $(files:.c) $(dependencies) gen_config.sh $(files:.c=.o)
	cc -o test $(files:.c=.o)

init :
	rm -f core/core.o

# Rule to generate the config.h file
gen_config.sh:
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)
		
.PHONY: gen_config.sh init debug release
.NOTPARALLEL: init
