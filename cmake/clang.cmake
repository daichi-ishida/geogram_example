set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER /usr/bin/clang-15)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-15)
set(CMAKE_AS /usr/bin/llvm-as-15)
set(CMAKE_NM /usr/bin/llvm-nm-15)
set(CMAKE_RANLIB  /usr/bin/llvm-ranlib-15)
set(CMAKE_OBJDUMP /usr/bin/llvm-objdump-15)
set(CMAKE_LINKER /usr/bin/lld-link-15)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")