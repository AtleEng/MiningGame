#!/bin/bash

libs="-luser32 -lopengl32 -lgdi32"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations"
includes="-IthirdParty -IthirdParty/Include"

clang++ $includes -g src/main.cpp -o vaultsBelow.exe $libs $warnings