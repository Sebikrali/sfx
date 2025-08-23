project := "sfx"
install_dir := "bin"
build_dir := "build"

alias c := cmake
alias cl := clean

default: all_debug

# Generates all CMake files (Debug&Release)
cmake:
    cmake -H. -B{{build_dir}}/Debug -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    cmake -H. -B{{build_dir}}/Release -G "Ninja" -DCMAKE_BUILD_TYPE=Release

clangd: cmake
    @rm -rf compile_commands.json
    @ln -s {{build_dir}}/Debug/compile_commands.json compile_commands.json

# Build everything (lib+all demos)
all_debug:
    @echo "Building debug..."
    @cmake --build {{build_dir}}/Debug

all_release:
    @echo "Building release..."
    @cmake --build {{build_dir}}/Release

# Build single target
build target config:
    @echo "Building {{target}} in {{config}}..."
    @cmake --build {{build_dir}}/{{config}} --target {{target}}

# Runs existing binary
run target="dev" config="Debug":
    @just build {{target}} {{config}}
    @cd demos/{{target}} && ../../bin/{{config}}/{{target}}

clean:
    rm -rf {{build_dir}}
    rm -rf {{install_dir}}
