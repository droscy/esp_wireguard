# Consumer project for the non-ESP-IDF branch of the top level CMakeLists.txt.
#
# This is a configure-time test, not a compile test. The sources need lwip,
# mbedtls and libsodium headers, none of which exist on a stock host, so
# building them here is not achievable without stubbing out most of an RTOS.
# What is worth checking is the part this repository actually owns: that
# add_subdirectory() defines the documented targets, that the include
# directories it advertises really contain the public headers, that the
# warning suppression is applied, and that the optional sodium link is guarded
# correctly. Everything below runs during `cmake -S ... -B ...`; there is
# deliberately no build step.
cmake_minimum_required(VERSION 3.13)

project(wireguard_cmake_consumer C)

# The if(TARGET sodium) guard in the library has two branches, so the test is
# configured twice: once with a sodium target already defined and once without.
option(WIREGUARD_TEST_WITH_SODIUM "Define a sodium target before add_subdirectory()" OFF)

if(WIREGUARD_TEST_WITH_SODIUM)
  add_library(sodium INTERFACE)
endif()

# Generating would already fail here if any source listed in WIREGUARD_SRCS had
# been renamed or removed.
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../.." wireguard)

function(expect_target target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "expected add_subdirectory() to define the target '${target}'")
  endif()
endfunction()

expect_target(wireguard)
expect_target(esphome::wireguard)

# The README tells consumers to link esphome::wireguard, so link it for real
# rather than only asserting that the alias exists.
add_library(consumer INTERFACE)
target_link_libraries(consumer INTERFACE esphome::wireguard)

# Public headers must be reachable through the advertised include dirs. Both
# entries matter: esp_wireguard.h is in include/, and the esp_wireguard_err.h
# it includes is in src/, which is why src is public.
get_target_property(include_dirs wireguard INTERFACE_INCLUDE_DIRECTORIES)
if(NOT include_dirs)
  message(FATAL_ERROR "wireguard has no INTERFACE_INCLUDE_DIRECTORIES")
endif()

foreach(header "esp_wireguard.h" "esp_wireguard_err.h")
  set(found NO)
  foreach(dir ${include_dirs})
    if(EXISTS "${dir}/${header}")
      set(found YES)
    endif()
  endforeach()
  if(NOT found)
    message(FATAL_ERROR
      "'${header}' is not reachable through the public include dirs: ${include_dirs}")
  endif()
endforeach()

get_target_property(compile_options wireguard COMPILE_OPTIONS)
if(NOT "-Wno-unused-result" IN_LIST compile_options)
  message(FATAL_ERROR
    "expected -Wno-unused-result on the wireguard target, got: ${compile_options}")
endif()

get_target_property(link_libraries wireguard LINK_LIBRARIES)
if(NOT link_libraries)
  set(link_libraries "")
endif()
if(WIREGUARD_TEST_WITH_SODIUM)
  if(NOT "sodium" IN_LIST link_libraries)
    message(FATAL_ERROR
      "a sodium target exists, so wireguard should link it, got: ${link_libraries}")
  endif()
else()
  if("sodium" IN_LIST link_libraries)
    message(FATAL_ERROR
      "no sodium target exists, so wireguard should not link it, got: ${link_libraries}")
  endif()
endif()

message(STATUS "wireguard CMake consumer checks passed (sodium target: ${WIREGUARD_TEST_WITH_SODIUM})")
