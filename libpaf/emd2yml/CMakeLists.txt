cmake_minimum_required(VERSION 3.5)

project(emd2yml C)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 11)

set(COMMON_COMPILE_OPTION "-Wl,-q -Wall -O2 -fno-inline -fno-builtin -fshort-wchar -Wunused-result")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_OPTION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_OPTION} -fno-rtti -fno-exceptions")

enable_language(CXX)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/sha1.c
)

target_link_libraries(${PROJECT_NAME}
)

if(DEFINED WIN_BUILD)
  set_target_properties(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS "-static -lgcc -lstdc++ -lwinpthread"
  )
endif()
