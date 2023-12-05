# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Jessica/esp/esp-idf/components/bootloader/subproject"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix/tmp"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix/src/bootloader-stamp"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix/src"
  "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/TrabalhoM3/teste2/hello_world/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
