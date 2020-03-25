cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
# Target client
add_executable(client client.cpp)
target_include_directories(client
        PUBLIC
        PRIVATE
        ../../inc
        ../../inc/tcp
        ../../inc/multithreading
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
        client.cpp
        clientflow.cpp
        ../tcp/tcpencoder.cpp
        ../tcp/tcpdecoder.cpp
        )
target_compile_options(client
        PUBLIC
        -D_DEBUG
        )
target_compile_features(client
        PUBLIC
        cxx_std_17
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