cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
IF (WIN32)
    set (VCPKG_TARGET_TRIPLET "x64-windows")
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/$ENV{USERNAME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
ENDIF()
project (sockets)

find_package(asio CONFIG REQUIRED)
find_package(Threads)
# Target server
add_executable(server src/server.cpp)
target_include_directories(server
        PUBLIC
        inc
        inc/tcp
        inc/multithreading
        PRIVATE
        )
#target_compile_definitions(server
#    PRIVATE
#)
target_sources(server
        PRIVATE
        src/server/server.cpp
        src/multithreading/threadpool.cpp
        src/tcp/tcpclientlist.cpp
        )
target_compile_options(server
        PUBLIC
        -D_DEBUG
        ${FPIC_FLAG}
        )
target_compile_features(server
        PUBLIC
        cxx_auto_type
        cxx_range_for
        )
target_link_libraries(server
        PUBLIC
        Threads::Threads
        asio::asio
        )
set_target_properties(server
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )