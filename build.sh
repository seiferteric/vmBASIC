#!/usr/bin/env bash
# Place this file next to src/ folder

cmake -S src/ -B build/     # Generate configs from the src/ folder into a build/ one
cmake --build build/        # Start building inside the build/ folder
#./build/vmBASIC               # Run the compiled program.  
