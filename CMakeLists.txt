cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
IF (WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/$ENV{USERNAME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
ENDIF ()
project(sockets)

find_package(asio CONFIG REQUIRED)
find_package(Threads)
add_subdirectory(src/server)
add_subdirectory(src/client)