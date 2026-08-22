# ESP-IDF gives a component the name of its directory, and the name of the
# checkout directory is not fixed: it is "wireguard" in CI, but a clone or a
# git worktree can be called anything. Derive it here rather than reading a
# variable set by the top level CMakeLists.txt, because ESP-IDF resolves
# REQUIRES in a separate `cmake -P` subprocess that does not see project
# variables, and an empty REQUIRES would silently fall back to main's
# "depend on every component in the build" behaviour. That fallback would
# hand main every include dir in the build and mask a component whose
# INCLUDE_DIRS or REQUIRES are wrong, which is the thing under test.
get_filename_component(WIREGUARD_COMPONENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
get_filename_component(WIREGUARD_COMPONENT_NAME "${WIREGUARD_COMPONENT_DIR}" NAME)

idf_component_register(
  SRCS "main.c"
  REQUIRES ${WIREGUARD_COMPONENT_NAME}
)
