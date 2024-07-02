lightdb is a lightweight disk-oriented relational database written in C++ which implements some key components of a traditional RDBMS including: Buffer Pool Manager (for memory management), Hash Index (storage engine), Query Executors & Query Optimizer (operators & optimizer), and Concurrency Control.

It is based on Dr Andy Pavlo's (@[pavlo](https://www.cs.cmu.edu/~pavlo/)) BusTub database system template which is as simple as:

```
bustub
├── build_support
│   ├── build-web-shell.sh
│   ├── clang_format_exclusions.txt
│   ├── cmake
│   ├── cpplint.py
│   ├── gtest_CMakeLists.txt.in
│   ├── packages.sh
│   ├── run_clang_format.py
│   ├── run_clang_tidy.py
│   └── run_clang_tidy_extra.py
├── src
│   ├── binder
│   ├── buffer
│   ├── catalog
│   ├── common
│   ├── concurrency
│   ├── container
│   ├── execution
│   ├── include
│   ├── optimizer
│   ├── planner
│   ├── primer
│   ├── recovery
│   ├── storage
│   └── type
├── test
│   ├── ...
├── third_party
│   ├── argparse
│   ├── backward-cpp
│   ├── cpp_random_distributions
│   ├── fmt
│   ├── googletest
│   ├── libfort
│   ├── libpg_query
│   ├── linenoise
│   ├── murmur3
│   ├── readerwriterqueue
│   ├── utf8proc
│   └── versions.txt
└── tools
    ├── b_plus_tree_printer
    ├── backtrace.cpp
    ├── bpm_bench
    ├── btree_bench
    ├── htable_bench
    ├── nc-shell
    ├── shell
    ├── sqllogictest
    ├── terrier_bench
    ├── wasm-bpt-printer
    └── wasm-shell
```

The `src` and `test` directories are empty and provide a template to build a more complex database.

I started this as a side-project in 2021 while learning database system at university and am in the process of migrating the code and docs to GitHub.

## Build

It is preferred to build lightdb on Ubuntu 22.04, or macOS (M1/M2/Intel).

### Linux (Recommended) / macOS (Development Only)

To ensure that you have the proper packages on your machine, run the following script to automatically install them:

```
# Linux
$ sudo build_support/packages.sh
# macOS
$ build_support/packages.sh
```

Then run the following commands to build the system:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If you want to compile the system in debug mode, pass in the following flag to cmake:
Debug mode:

```
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -j`nproc`
```
This enables [AddressSanitizer](https://github.com/google/sanitizers) by default.

If you want to use other sanitizers,

```
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUSTUB_SANITIZER=thread ..
$ make -j`nproc`
```

There are some differences between macOS and Linux (i.e., mutex behavior) that might cause test cases
to produce different results in different platforms. We recommend students to use a Linux VM for running
test cases and reproducing errors whenever possible.
