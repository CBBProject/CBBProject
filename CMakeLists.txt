################################################################################
#
# CBB project canvas
#
# Copyright (c) INRIA 2014. All rights reserved.
# See LICENSE.txt for details.
# 
################################################################################

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

project(CBB)

include(CheckEnvironment)
include(PackageVersion)

PackageVersion(CBB 0 1 0)

set(dtk_DIR ${CMAKE_CURRENT_BINARY_DIR}/dtk)

foreach(i Utils Maths Images)
    include_directories(platform/${i}/include)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/platform/${i}/include)
endforeach()
include_directories(platform/interface include)

subdirs(dtk platform plugins)
