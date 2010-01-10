CFLAGS := -Wall

# This is the array that simulates the DAEMONS array in the rc.conf 
daemons := alsa network samba

files := test.c $(foreach daemon, $(daemons), daemons/$(daemon).c)

.DEFAULT: test

# Rule to transforms .c files into .o files
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
test : Makefile init $(files:.c) gen_config.sh $(files:.c=.o)
	cc -o test $(files:.c=.o)

init :
	rm -f test.o

# Rule to generate the config.h file
gen_config.sh:
	./gen_config.sh config.h daemons $(daemons)
	
clean : 
	rm -f *.o
	rm -f daemons/*.o	
	rm -f test
	
.PHONY: clean all gen_config.sh init
.NOTPARALLEL: init
