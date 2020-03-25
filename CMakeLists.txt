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
# Target server
add_executable(server src/server/server.cpp)
target_include_directories(server
        PUBLIC
        inc
        inc/tcp
        inc/multithreading
        PRIVATE
        )
target_compile_definitions(server
        PRIVATE
        ASIO_STANDALONE=1
        ASIO_HAS_STD_ADDRESSOF=1
        ASIO_HAS_STD_ARRAY=1
        ASIO_HAS_CSTDINT=1
        ASIO_HAS_STD_SHARED_PTR=1
        ASIO_HAS_STD_TYPE_TRAITS=1
        ASIO_HAS_VARIADIC_TEMPLATES=1
        ASIO_HAS_STD_FUNCTION=1
        ASIO_HAS_STD_CHRONO=1
        BOOST_ALL_NO_LIB=1
        _WIN32_WINNT=0x0501
        _WINSOCK_DEPRECATED_NO_WARNINGS=1
        )
target_sources(server
        PRIVATE
        src/server/server.cpp
        src/server/multithreading/threadpool.cpp
        src/server/tcp/tcpclientlist.cpp
        src/tcp/tcpencoder.cpp
        src/tcp/tcpdecoder.cpp
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
        wsock32
        ws2_32
        )
set_target_properties(server
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )


# Target client
add_executable(client src/client/client.cpp)
target_include_directories(client
        PUBLIC
        PRIVATE
        inc
        inc/tcp
        inc/multithreading
        )
target_compile_definitions(client
        PRIVATE
        ASIO_STANDALONE=1
        ASIO_HAS_STD_ADDRESSOF=1
        ASIO_HAS_STD_ARRAY=1
        ASIO_HAS_CSTDINT=1
        ASIO_HAS_STD_SHARED_PTR=1
        ASIO_HAS_STD_TYPE_TRAITS=1
        ASIO_HAS_VARIADIC_TEMPLATES=1
        ASIO_HAS_STD_FUNCTION=1
        ASIO_HAS_STD_CHRONO=1
        BOOST_ALL_NO_LIB=1
        _WIN32_WINNT=0x0501
        _WINSOCK_DEPRECATED_NO_WARNINGS=1
        )
target_sources(client
        PRIVATE
        src/client/client.cpp
        src/client/clientflow.cpp
        src/tcp/tcpencoder.cpp
        src/tcp/tcpdecoder.cpp
        )
target_compile_options(client
        PUBLIC
        -D_DEBUG
        ${FPIC_FLAG}
        )
target_compile_features(client
        PUBLIC
        cxx_auto_type
        cxx_range_for
        )
target_link_libraries(client
        PUBLIC
        Threads::Threads
        asio::asio
        wsock32
        ws2_32
        )
set_target_properties(client
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
