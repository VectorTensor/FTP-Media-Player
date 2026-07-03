cmake_minimum_required(VERSION 4.1.1)
project(Pegasus)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(CURL REQUIRED)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
add_library(
        audiobook_core
        src/lib/FTPTextSource.cpp
)

target_include_directories(
        audiobook_core PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(audiobook_core PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf  CURL::libcurl)

add_executable(Pegasus src/main.cpp

)
target_link_libraries(Pegasus PRIVATE audiobook_core)



# On Windows, copy SDL3.dll to the build directory
if(WIN32)
    add_custom_command(
            TARGET Pegasus POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Lagrangian>
            VERBATIM
    )
endif()
