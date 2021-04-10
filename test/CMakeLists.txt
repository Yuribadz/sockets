 # This adds another subdirectory, which has 'project(gtest)'.
 project(sockets)
 add_subdirectory(googletest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()
  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests test1.cpp)

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests 
  PRIVATE
    gtest 
    gtest_main
    Threads::Threads
    asio::asio
    client_test
    wsock32
    ws2_32)
  target_compile_definitions(runUnitTests
        PRIVATE
        ASIO_STANDALONE=1
        ASIO_HAS_EXCEPTIONS=1
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

  add_test(NAME All-tests COMMAND runUnitTests)
