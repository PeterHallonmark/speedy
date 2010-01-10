files := core/core.c \
         $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c)

.DEFAULT: test

# Rule to transform .c files into .o files
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

debug : test

release : test
	
test : Makefile init $(files:.c) gen_config.sh $(files:.c=.o)
	cc -o test $(files:.c=.o)

init :
	rm -f core/core.o

# Rule to generate the config.h file
gen_config.sh:
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)
		
.PHONY: gen_config.sh init debug release
.NOTPARALLEL: init
