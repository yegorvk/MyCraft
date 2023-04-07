# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-src"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-build"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/tmp"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/src/cmrc-populate-stamp"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/src"
  "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/src/cmrc-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/src/cmrc-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/egorv/CLionProjects/ShitCraft/cmake-build-debug/_deps/cmrc-subbuild/cmrc-populate-prefix/src/cmrc-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
