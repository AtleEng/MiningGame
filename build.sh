#!/bin/bash

timestamp=$(date +%s)

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"
includes="-IthirdParty -IthirdParty/Include"

clang++ $includes -g src/main.cpp -o vaultsBelow.exe $libs $warnings

rm -f game_*
clang++ -g "src/game.cpp" -shared -o game_$timestamp.dll $warnings
mv game_$timestamp.dll game.dll
