cmake_minimum_required(VERSION 4.1.1)
project(Pegasus) 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(protobuf_INSTALL OFF CACHE BOOL "" FORCE)
set(utf8_range_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(ABSL_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(gRPC_INSTALL OFF CACHE BOOL "" FORCE)
set(gRPC_BUILD_GRPCPP_REFLECTION ON CACHE BOOL "" FORCE)
set(build_examples OFF CACHE BOOL "" FORCE)
set(build_tests OFF CACHE BOOL "" FORCE)
set(deprecated-functions OFF CACHE BOOL "" FORCE)

option(USE_SYSTEM_GRPC "Use system-installed gRPC instead of fetching it" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
find_package(OpenSSL REQUIRED)
message(STATUS "Pegasus using OpenSSL ${OPENSSL_VERSION}")
message(STATUS "  Include: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "  SSL lib: ${OPENSSL_SSL_LIBRARY}")
message(STATUS "  Crypto lib: ${OPENSSL_CRYPTO_LIBRARY}")
if(USE_SYSTEM_GRPC)
    list(APPEND CMAKE_PREFIX_PATH "/opt/grpc")
    find_package(Protobuf CONFIG REQUIRED)
    find_package(gRPC CONFIG REQUIRED)
else()
    FetchContent_Declare(

      grpc
      GIT_REPOSITORY https://github.com/grpc/grpc
      GIT_TAG v1.82.0
      GIT_SHALLOW TRUE

    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(grpc)
    include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
endif()


find_package(CURL REQUIRED)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})


set(PROTO_FILES 

  ${CMAKE_CURRENT_SOURCE_DIR}/protos/Anime.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/protos/Download.proto
)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

# --- Precompute expected output filenames for each proto ---
set(ALL_PROTO_SRCS "")
set(ALL_PROTO_HDRS "")

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

    list(APPEND ALL_PROTO_SRCS ${GENERATED_DIR}/${PROTO_NAME}.pb.cc)
    list(APPEND ALL_PROTO_HDRS ${GENERATED_DIR}/${PROTO_NAME}.pb.h)
    list(APPEND ALL_PROTO_SRCS ${GENERATED_DIR}/${PROTO_NAME}.grpc.pb.cc)
    list(APPEND ALL_PROTO_HDRS ${GENERATED_DIR}/${PROTO_NAME}.grpc.pb.h)
endforeach()

# --- Single protoc invocation for all protos at once ---

if(USE_SYSTEM_GRPC)
    add_custom_command(
            OUTPUT ${ALL_PROTO_SRCS} ${ALL_PROTO_HDRS}
            COMMAND $<TARGET_FILE:protobuf::protoc>
            ARGS --grpc_out=${GENERATED_DIR}
            --cpp_out=${GENERATED_DIR}
            --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
            -I ${CMAKE_CURRENT_SOURCE_DIR}/protos
            ${PROTO_FILES}
            DEPENDS ${PROTO_FILES}
            COMMENT "Running protoc on all proto files"
    )
else()
    add_custom_command(
    OUTPUT ${ALL_PROTO_SRCS} ${ALL_PROTO_HDRS}
    COMMAND $<TARGET_FILE:protoc>
    ARGS --grpc_out=${GENERATED_DIR}
         --cpp_out=${GENERATED_DIR}
         --plugin=protoc-gen-grpc=$<TARGET_FILE:grpc_cpp_plugin>
         -I ${CMAKE_CURRENT_SOURCE_DIR}/protos
         ${PROTO_FILES}
    DEPENDS ${PROTO_FILES}
    COMMENT "Running protoc on all proto files"
    )
endif()

add_library(expected INTERFACE)
target_include_directories(expected INTERFACE ${CMAKE_SOURCE_DIR}/thirdparty/expected/include)
add_library(proto-objects OBJECT ${ALL_PROTO_SRCS} ${ALL_PROTO_HDRS})
if(USE_SYSTEM_GRPC)
        target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf gRPC::grpc++)
else()
    target_link_libraries(proto-objects PUBLIC libprotobuf grpc++)
endif()
target_include_directories(proto-objects PUBLIC ${GENERATED_DIR})

add_library(
        audiobook_core
        src/lib/ftp_text_source.cpp
        src/lib/config/config.cpp
        src/lib/config/config.h
)

target_include_directories(
        audiobook_core PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)


target_link_libraries(audiobook_core PUBLIC 
  expected
  CURL::libcurl
)
add_subdirectory(thirdparty/libtorrent)


add_executable(Pegasus src/main.cpp
        src/services/anime_service.cpp
        src/services/anime_service.h
        src/lib/ftp_helper/ftp_helper.cpp
        src/services/download_service.cpp
        src/services/download_service.h
        src/lib/torrent_helper/torrent_helper.cpp
        src/lib/torrent_helper/torrent_helper.h
)
if(USE_SYSTEM_GRPC)
    target_link_libraries(Pegasus PRIVATE audiobook_core proto-objects gRPC::grpc++ gRPC::grpc++_reflection torrent-rasterbar)
else()
    target_link_libraries(Pegasus PRIVATE audiobook_core proto-objects grpc++ grpc++_reflection torrent-rasterbar)
endif()



# On Windows, copy SDL3.dll to the build directory
if(WIN32)
    add_custom_command(
            TARGET Pegasus POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Lagrangian>
            VERBATIM
    )
endif()
