all: unscrambler.c
	gcc -o unscrambler.exe unscrambler.c
clean: 
	$(RM) unscrambler.exe