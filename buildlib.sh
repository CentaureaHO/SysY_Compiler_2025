clang -c lib/utils.ll -o lib/utils.o
ar rcs lib/libutils_x86.a lib/utils.o
rm lib/utils.o

clang --target=riscv64-linux-gnu -march=rv64gc -mcpu=sifive-x280 -O2 -funroll-loops -finline-functions -finline-hint-functions -S lib/utils.ll -o lib/tmp.s
sed -E '/^\s*\.(file|section|globl|type|size|text|ident)/d; /^$/d' lib/tmp.s > lib/libutils_rv64.s
rm lib/tmp.s
