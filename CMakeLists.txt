cmake_minimum_required(VERSION 4.1.1)
project(Pegasus)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf REQUIRED)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
add_executable(Pegasus src/main.cpp

)

target_include_directories(Pegasus PRIVATE src src/main.cpp)

target_link_libraries(Pegasus PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# On Windows, copy SDL3.dll to the build directory
if(WIN32)
    add_custom_command(
            TARGET Pegasus POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Lagrangian>
            VERBATIM
    )
endif()
