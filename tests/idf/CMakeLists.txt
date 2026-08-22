# Minimal ESP-IDF project that builds the repository root as a component.
#
# This exercises both files the component build is made of: CMakeLists.txt, via
# the idf_component_register() branch, and idf_component.yml, whose
# esphome/libsodium dependency the component manager resolves from the
# component registry while configuring this project.
#
# main requires that component by name and nothing else, so main.c is compiled
# with only what the component makes public. That is what gives the test its
# teeth: if include/ or src/ stops being a public include dir, or lwip stops
# being a public REQUIRES, main.c stops compiling.
cmake_minimum_required(VERSION 3.16)

get_filename_component(WIREGUARD_COMPONENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(EXTRA_COMPONENT_DIRS "${WIREGUARD_COMPONENT_DIR}")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(wireguard_idf_compile_test)
