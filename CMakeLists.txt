project(sfTD)
cmake_minimum_required(VERSION 2.8.1)
aux_source_directory(. SRC_LIST)

set(CHAI_VERSION 5.4.0)

set(buildDir ${CMAKE_CURRENT_SOURCE_DIR}/../SfTD-build/)
set(EXECUTABLE_OUTPUT_PATH ${buildDir} CACHE PATH "Build Directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${buildDir} CACHE PATH "Build Directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${buildDir})
set(CMAKE_BUILD_FILES_DIRECTORY ${buildDir})
set(CMAKE_BUILD_DIRECTORY ${buildDir})
set(CMAKE_BINARY_DIR  ${buildDir})
SET(EXECUTABLE_OUTPUT_PATH ${buildDir})
SET(LIBRARY_OUTPUT_PATH ${buildDir}/lib)
SET(CMAKE_CACHEFILE_DIR ${buildDir})

include_directories("./include")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message("Using CXX as Clang.")
    set(CMAKE_CXX_FLAGS "-std=c++1y  -Wno-c++98-compat -Wno-c++11-extensions -Wno-padded -Weverything -Wno-switch-enum")
else()
    message("Using CXX as GCC.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wno-switch-enum -Wundef -Wlogical-op -Winline -Wunused -Wuninitialized -std=c++1y")
endif()
file(GLOB_RECURSE CPP_LIST src/*.cpp
                            include/*.tpp)
file(GLOB_RECURSE HPP_LIST include/*.hpp)

find_package(SFML 2 REQUIRED system window graphics audio)
find_package(Thor REQUIRED)
find_package(SFGUI REQUIRED)

add_subdirectory("./extlibs/sfml-tmxloader")

add_subdirectory("./extlibs/luawrapper/luawrapper")

include_directories("./extlibs/sfml-tmxloader/include")

include_directories("./extlibs/rapidjson")

include_directories("./extlibs/luawrapper")

add_executable(${PROJECT_NAME} ${CPP_LIST} ${HPP_LIST})

target_link_libraries(${PROJECT_NAME} sfml-tmxloader)
target_link_libraries(${PROJECT_NAME} pugi)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})


target_link_libraries(${PROJECT_NAME} ${THOR_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARY})

add_custom_target(VeraStyle ALL
  vera++ -s
  --warning
  ${CPP_LIST} ${HPP_LIST})

include_directories(${LUA_INCLUDE_DIR})



