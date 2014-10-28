cmake_minimum_required(VERSION 2.8)

set(CMAKE_GENERATOR_TOOLSET "v120" CACHE STRING "")			# building for VC++ 2013


# boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# lua
find_package(Lua51)
find_package(Lua52)
find_package(LuaJIT51)
if (NOT ${LUA51_FOUND} AND NOT ${LUA52_FOUND} AND NOT ${LUAJIT51_FOUND})
	message(FATAL_ERROR "Could not find lua library")
endif()
include_directories(${LUA_INCLUDE_DIR})

# including lua wrapper
include_directories(.)
# flags
if (${CMAKE_COMPILER_IS_GNUCXX})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
