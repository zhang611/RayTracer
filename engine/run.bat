rm -r build
cmake -B build
cmake --build build --config Release
.\build\source\Release\TinyRayTracer.exe