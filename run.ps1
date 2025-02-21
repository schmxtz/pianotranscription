clear
cmake -S . -B build
cmake --build build --config Debug --target $target
./build/$target