# WCC 
WCC is a C compiler written in C99. It's a toy compiler that I wrote for fun, and it shouldn't be used for any real world scenarios.

## Usage
WCC is really simple. Like *really* simple. I've tried to keep the CLI tool as similar to GCC as possible, but it's lacking a lot of the features. Here's a list of added and planned features:  
```
Added:

Planned:
-o <file>             Specify output file
-c                    Don't link
-O <level>            Optimize on level `level`. I'll try to make all optimizations match GCC
-Wall                 All warnings
-Werror               Mark warnings as errors
-Wextra               Extra warnings
-S                    This will invoke `wcc-nasm`, as I really don't wanna deal with gnuisance
```
To use the CLI tool, simply:
```
wcc <flags> <file(s)>
```
Or you can use any of the subtools (which haven't been created yet):
```
wcc-asm  <flags> <file(s)>                 Assembler for the WCC IR
wcc-nasm <flags> <file(s)>                 Produce nasm assembly from a C99 file
wcc-gnu  <extensions> <flags> <file(s)>    Compile `file(s)` with gnu extensions (specified in `<extensions`)           
```
WCC produces ELF binaries for x86 and x86_64. It also links with libc to produce runnable binaries.
## Compiling WCC
If you want to compile WCC yourself, which you might wanna do for various reasons, I've tried to make it as easy as possible.  
In the repository I've included a `build.py` file which can be used like so:
```
python3 build.py                               Builds WCC normally. Auto-detects C compiler, and compiles with generic flags
CC=wcc python3 build.py                        Builds WCC with WCC. The build scripts can detect if you're using WCC, so it will exculde any non-supported C flags
CFLAGS=-Wall,-Werror,-O2 python3 build.py      Builds WCC given comma-seperated C flags.
python3 build.py clean                         Deletes binaries, object files
python3 build.py debug                         Builds WCC with some pre-defined debug flags
```
Any combination of the above can be used. The binary will be placed in `bin/release/wcc(-xxxx)` or `bin/release/wcc(-xxxx)`
## Contributing
Feel like you have an addition you'd like to make? Feel free to open a PR. I'll review it as soon as I have time. Make sure to include good reasoning as to why you're making the change that you're proposing, or make sure you're showing which bug you're fixing and how to reproduce it.  
<br/><br/><br/><br/><br/><br/><br/><br/>
You've scrolled too far!!!
