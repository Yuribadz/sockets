cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project (sockets)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Lookup for necessary libraries in system
find_package(Threads)
# Target server
add_executable(server src/server.cpp)
target_include_directories(server
    PUBLIC 
      inc
    PRIVATE 
)
#target_compile_definitions(server
#    PRIVATE
#)
target_sources(server
  PRIVATE 
  src/server.cpp
  src/EchoServ.cpp
)
target_compile_options(server
    PUBLIC
    -g
)
target_compile_features(server
  PUBLIC
  cxx_std_17
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


# Target client
add_executable(client src/main.cpp)
target_include_directories(client
  PUBLIC 
  inc
  PRIVATE 
  )
#target_compile_definitions(client
#    PRIVATE
#)
target_sources(client
  PRIVATE 
  src/main.cpp
  src/ThreadPool.cpp
  )
target_compile_options(client
  PUBLIC
  #-fsanitize=thread -O2 -g
  )
target_compile_features(client
  PUBLIC
  cxx_std_17
  )
target_link_libraries(client 
  PUBLIC
  Threads::Threads
  #-latomic
  #-fsanitize=thread
  )
set_target_properties(client
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#  LINK_FLAGS -fsanitize=thread
#  COMPILE_FLAGS -fsanitize=thread
  )
