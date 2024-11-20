llc -march=riscv64 -mattr=+m,+a,+f,+d,+c -relocation-model=pic test.ll -o test.s
riscv64-linux-gnu-gcc -static -march=rv64imafdc -mabi=lp64d test.s -o test.bin -L./library -lsysy_rv -lm