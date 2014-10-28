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


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused -Wuseless-cast -Wnon-virtual-dtor")

file(GLOB_RECURSE CPP_LIST src/*.cpp
                            include/*.tpp)
file(GLOB_RECURSE HPP_LIST include/*.hpp)

find_package(SFML 2 REQUIRED system window graphics audio)
find_package(Thor REQUIRED)

add_subdirectory("./extlibs/sfml-tmxloader")

#add_subdirectory("./extlibs/ChaiScript")

add_subdirectory("./extlibs/luawrapper/luawrapper")

include_directories("./extlibs/sfml-tmxloader/include")

include_directories("./extlibs/rapidjson")

include_directories("./extlibs/luawrapper")

link_directories("./extlibs/sfml-tmxloader/lib")

add_executable(${PROJECT_NAME} ${CPP_LIST} ${HPP_LIST})

target_link_libraries(${PROJECT_NAME} sfml-tmxloader)
target_link_libraries(${PROJECT_NAME} pugi)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})


target_link_libraries(${PROJECT_NAME} ${THOR_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})

add_custom_target(VeraStyle ALL
  vera++ -s
  --warning
  ${CPP_LIST} ${HPP_LIST})

include_directories(${LUA_INCLUDE_DIR})



