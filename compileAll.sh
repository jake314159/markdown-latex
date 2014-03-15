##Used to number the output files with a version
ver='0.9'

## Note: you may need gcc-multilib

## compile for the current system (linux?) 32 bit
make -e CC='gcc -m32' FILE_OUT=markdownlatex_linux32_$ver

## compile for the current system (linux?) 36 bit
make -e CC='gcc -m64' FILE_OUT=markdownlatex_linux64_$ver


## compile for windows 32 bit
make -e CC=i686-w64-mingw32-gcc FILE_OUT=markdownlatex_win32_$ver

## compile for windows 64 bit
make -e CC=x86_64-w64-mingw32-gcc FILE_OUT=markdownlatex_win64_$ver

make clean
