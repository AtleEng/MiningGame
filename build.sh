#!/bin/bash

timestamp=$(date +%s)

defines="-DENGINE"
libs="-luser32 -lopengl32 -lgdi32 -lole32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"
includes="-IthirdParty -IthirdParty/Include"

clang++ $includes -g src/main.cpp -o vaultsBelow.exe $libs $warnings $defines

rm -f game_*
clang++ -g "src/game/game.cpp" -shared -o game_$timestamp.dll $warnings $defines
mv game_$timestamp.dll game.dll
