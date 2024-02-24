# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/connor/pico/pico-sdk/tools/pioasm"
  "/home/connor/pico/DC_Motor/build/pioasm"
  "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/connor/pico/DC_Motor/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()