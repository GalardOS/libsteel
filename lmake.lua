lmake_compatibility_version(1)

BUILD_WITH_TESTS = false

COMPILER = "/bin/clang"

LINKER = "/bin/aarch64-linux-gnu-ld"

CXX_FLAGS = "-Wall -nostdlib -ffreestanding -Ilib -Isrc/klib -Isrc/kernel -mgeneral-regs-only"
ASM_FLAGS = "-Ilib -nostdlib"

ARCH = "aarch64"
CLANG_TARGET = ""
LLD_TARGET = ""

if ARCH == "aarch64" then
    CLANG_TARGET = "--target=aarch64 "
    --LLD_TARGET = "-m aarch64elf "
end

function compile_files(cpp_files, asm_files)
    lmake_set_compiler(COMPILER)
    lmake_set_compiler_flags(CLANG_TARGET .. CXX_FLAGS)
    lmake_set_compiler_out("bin/%.o")
    lmake_compile(cpp_files)

    lmake_set_compiler_flags(CLANG_TARGET .. ASM_FLAGS)
    lmake_compile(asm_files)
end

function link_executable(obj_files, linker_path)
    lmake_set_linker(LINKER)
    lmake_set_linker_flags(LLD_TARGET .. "-T " .. linker_path)
    lmake_set_linker_out("bin/kernel.elf")
    lmake_link(obj_files)
end

function build_obj()
    local c_cc_files = lmake_find("lib/**.cc") .. lmake_find("lib/**.c")

    if BUILD_WITH_TESTS == true then
        c_cc_files = c_cc_files .. "src/test.cc"
    end

    local asm_files = lmake_find("lib/**.S")

    lmake_set_compiler(COMPILER)
    lmake_set_compiler_flags(CLANG_TARGET .. CXX_FLAGS)
    lmake_set_compiler_out("bin/%.o")
    lmake_compile(c_cc_files)

    lmake_set_compiler_flags(CLANG_TARGET .. ASM_FLAGS)
    lmake_compile(asm_files)
end

function lib()
    build_obj()

    local obj_files = lmake_find("bin/*.o")
    lmake_exec("ar rcs bin/libsteel.a " .. obj_files)
end

function executable()
    BUILD_WITH_TESTS = true

    build_obj()

    local obj_files = lmake_find("bin/*.o")

    lmake_set_linker(LINKER)
    lmake_set_linker_flags(LLD_TARGET .. "-T linker/linker.ld")
    lmake_set_linker_out("bin/kernel.elf")
    lmake_link(obj_files)

    lmake_exec("aarch64-linux-gnu-objcopy bin/kernel.elf -O binary kernel8.img")
end

function execute()
    -- Build with tests if not built
    BUILD_WITH_TESTS = true

    executable()

    lmake_exec("qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio -d int")
end

function clean()
    obj_files = lmake_find("bin/*") .. "kernel8.img"
    lmake_exec("rm " .. obj_files)
end