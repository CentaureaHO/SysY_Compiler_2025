clang test.ll -c -o test.o -w
clang -static test.o -L./lib -lsysy_x86 -o test.bin
