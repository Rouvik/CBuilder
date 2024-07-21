# CBuilder
A simple header only C build script library to easily develop C based build scripts on different platforms (currently I am only targeting Windows)

# Why CBuilder?
We already have so many different sort of build scripts available like *Make, CMake, bash, batch, Ninja, etc...* but why a C build script?
Well, for starters most of these build scripts above dont support complex logic like a mature language C does, also may not be available on all systems
but C is! Plus, we get the added benefit of using no other build script other than C itself

# How to use CBuilder?
This build is not complete and perfect, examples coming soon...

# Roadmap:
- Add support for Linked lists in CBuilder_List and provide a different version of CBuild_Fs_dir which returns a CBuild_List
- Add support for predefined command generators to exclude the hassle to write down your own commands for each type of files
- Add support for depedency identification and analysis like Make to prevent re-compilation of already compiled dependencies

# Author
Rouvik Maji [gmail](mailto:majirouvik@gmail.com)
