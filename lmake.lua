lmake_compatibility_version(1)

COMPILER = "/bin/clang"
LINKER = "/bin/ld.lld"
CXX_FLAGS = "-Wall -nostdlib -ffreestanding -Isrc -Isrc/klib -Isrc/kernel -mgeneral-regs-only"
ASM_FLAGS = "-Isrc -nostdlib"

ARCH = "aarch64"
CLANG_TARGET = ""
LLD_TARGET = ""

if ARCH == "aarch64" then
    CLANG_TARGET = "--target=aarch64 "
    LLD_TARGET = "-m aarch64elf "
end

function build()
    c_cc_files = lmake_find("src/**.cc") .. lmake_find("test/**.cc")
    asm_files = lmake_find("arch/aarch64/**.S")

    lmake_set_compiler(COMPILER)
    lmake_set_compiler_flags(CLANG_TARGET .. CXX_FLAGS)
    lmake_set_compiler_out("bin/%.o")
    lmake_compile(c_cc_files)

    lmake_set_compiler_flags(CLANG_TARGET .. ASM_FLAGS)
    lmake_compile(asm_files)

    obj_files = lmake_find("bin/*.o")

    lmake_set_linker(LINKER)
    lmake_set_linker_flags(LLD_TARGET .. "-T linker/linker.ld")
    lmake_set_linker_out("bin/kernel.elf")
    lmake_link(obj_files)

    lmake_exec("aarch64-linux-gnu-objcopy bin/kernel.elf -O binary kernel8.img")
end

function clean()
    obj_files = lmake_find("bin/*")
    lmake_exec("rm " .. obj_files)
end