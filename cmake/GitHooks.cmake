#   Install a update hook to recover automatically packages such as dtk.

find_package(Git)
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git/config")

    #   Install the git update hook.

    set(WARNING_MESSAGE "WARNING: DO NOT MODIFY\n# This file has been automatically generated from utils/git/pre-commit.")
    configure_file(${CMAKE_SOURCE_DIR}/utils/git/update ${CMAKE_SOURCE_DIR}/.git/hooks/update @ONLY)
endif()
