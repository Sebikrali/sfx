project := "SFX_GL"
install_dir := "bin"
build_dir := "build"

alias c := cmake
alias cl := clean
alias bd := debug
alias br := release
alias rr := run_release
alias r := run

# run_debug
default: run_debug

# Generates all CMake files (Debug&Release)
cmake:
    cmake -H. -B{{build_dir}}/debug -G "Ninja" -DCMAKE_INSTALL_PREFIX={{install_dir}} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    cmake -H. -B{{build_dir}}/release -G "Ninja" -DCMAKE_INSTALL_PREFIX={{install_dir}} -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1

clangd: cmake
    @rm -rf compile_commands.json
    @ln -s {{build_dir}}/debug/compile_commands.json compile_commands.json

# Builds in Debug Configuration
debug:
    @echo "Building debug..."
    @cmake --build {{build_dir}}/debug --target install
    @cp {{build_dir}}/debug/"{{project}}" {{install_dir}}

# Builds in Release Configuration
release:
    @echo "Building release..."
    @cmake --build {{build_dir}}/release --target install
    @cp {{build_dir}}/release/"{{project}}" {{install_dir}}

# Runs existing binary
run:
    @echo "Running {{project}}"
    @{{install_dir}}/{{project}}

# Builds debug, then runs binary
run_debug: debug
    {{build_dir}}/debug/"{{project}}"

# Builds release, then runs binary
run_release: release
    {{build_dir}}/release/"{{project}}"

clean:
    rm -rf {{build_dir}}
    rm -rf {{install_dir}}
