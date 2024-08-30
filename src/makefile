cc = clang
cflags = -std=c17 -Wall -Wcompare-distinct-pointer-types
exe_name = white_pine.exe

make: rnapi.o vm.o
	$(cc) main.c vm.o rnapi.o $(cflags) -o $(exe_name)

fresh: clean make

rnapi.o: rnapi.c
	$(cc) -c rnapi.c $(cflags)

vm.o: vm.c
	$(cc) -c vm.c $(cflags)
	
clean:
	del *.o
	del *.exe