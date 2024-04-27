# CC

C compiler written in C++, with error accumulation, float/int parsing, pointers, 
if/else, while and other features. 

## Quickstart

To build the compiler, use `make`. This requires `llvm-14` to be installed 
(`brew install llvm@14` for macs, `sudo apt-get install llvm` for ubuntu). `make`
generates the `cc` executable in the current directory. 

To run tests, create the `tests/cc` and `tests/clang` directory, and run `make test`.

## Usage

```
Usage: cc [--help] [--version] [--object VAR] [--print-ast] source

Positional arguments:
  source           Source file to compile 

Optional arguments:
  -h, --help       shows help message and exits 
  -v, --version    prints version information and exits 
  -o, --object     Object file to generate 
  -t, --print-ast  Print AST 
```

## About

Credits: Aniruddha Deb, Jaivardhan Singh.

Open source libraries used: [argparse](https://github.com/p-ranav/argparse) (MIT license)
