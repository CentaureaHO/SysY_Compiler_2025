riscv64-unknown-linux-gnu-gcc test.s -c -o test.o -w
riscv64-unknown-linux-gnu-gcc -static test.o -L./lib -lsysy_riscv -o test.bin
