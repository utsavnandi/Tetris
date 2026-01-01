# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspace/build-gcc/_deps/raylib-src"
  "/workspace/build-gcc/_deps/raylib-build"
  "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix"
  "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspace/build-gcc/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
