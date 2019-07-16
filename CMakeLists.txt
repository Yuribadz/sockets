cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project (sockets)
# Lookup for necessary libraries in system
find_package(Threads)
# Target server
add_executable(server src/server.cpp)
target_include_directories(server
    PUBLIC 
      inc
      /usr/include
    PRIVATE 
)
#target_compile_definitions(server
#    PRIVATE
#)
target_sources(server
	PRIVATE 
      src/server.cpp
      src/main.cpp
)
target_compile_options(server
    PUBLIC
)
target_compile_features(server
    PUBLIC
	  cxx_auto_type
	  cxx_range_for
)
target_link_libraries(server 
    PUBLIC
)
set_target_properties(server
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
