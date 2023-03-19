# Lamport clock database based on LevelDb

## Build

Clone repository with all submodules `git clone --recurse-submodules https://github.com/Oanikulin/ds-project-dev.git`

From cloned repository, call  `mkdir build && cd build && cmake ..`. All dependencies are included automatically.
After that code can be build using `make`.
Tested under GCC 7.5 compiler and Ubuntu 18.04 to be (probably) correct. Requires at least C++17.

## Usage

Application is one binary file, which should be launched with configuration given to it.
Currently web server to connect to database is not implemented, only internal part.

Proto files are provided as future API in `/proto/lseqDb.proto`

## Testing

Application uses GTest, test can also be used to check database behavior. They might be mulfunctional under different compiler or OS 
