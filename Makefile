CFLAGS := -Wall

# This is the array that simulates the rest of the rc.conf
# So far it doesn't contain that much.
sysinit := hostname keymap

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons := alsa network samba

files := core/core.c \
         $(foreach init_item, $(sysinit), sysinit/$(init_item).c) \
         $(foreach daemon, $(daemons), daemons/$(daemon).c)

.DEFAULT: test

# Rule to transform .c files into .o files
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
test : Makefile init $(files:.c) gen_config.sh $(files:.c=.o)
	cc -o test $(files:.c=.o)

init :
	rm -f core/core.o

# Rule to generate the config.h file
gen_config.sh:
	core/gen_config.sh core/config_sysinit.h sysinit $(sysinit)
	core/gen_config.sh core/config_daemons.h daemons $(daemons)
	
clean : 
	rm -f core/*.o
	rm -f sysinit/*.o
	rm -f daemons/*.o	
	rm -f test
	
.PHONY: clean all gen_config.sh init
.NOTPARALLEL: init
