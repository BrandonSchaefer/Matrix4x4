set (GMOCK_ROOT_DIR
  "/usr/src/gmock"
  CACHE
  PATH
  "Path to Google mock srcs")

set (GMOCK_INCLUDE_DIR
  "/usr/include/gmock/include"
  CACHE
  PATH
  "Path to Google mock include")

set (GTEST_ROOT_DIR
     "${GMOCK_ROOT_DIR}/gtest"
     CACHE
     PATH
     "Path to Google Tests source"
     FORCE)

set (GTEST_INCLUDE_DIR
     "${GMOCK_ROOT_DIR}/gtest/include"
     CACHE
     PATH
     "Path to Google Tests include"
     FORCE)


#find_path (GTEST_INCLUDE_DIR gtests/gtest.h)

#if (GTEST_INCLUDE_DIR)
  #add_subdirectory (
    #${GTEST_ROOT_DIR}
    #gtest
   #)
#endif (GTEST_INCLUDE_DIR)

enable_testing ()

#find_library (GMOCK_LIB gmock)
#find_library (GMOCK_MAIN_LIB gmock_main)
#find_path (GTEST_SRC_DIR gtest.cc PATHS /usr/src/gtest/src)

set (GMOCK_LIB gmock)
set (GMOCK_MAIN_LIB gmock_main)

message ("${GTEST_SRC_DIR} - ${GMOCK_LIB} - ${GMOCK_MAIN_LIB}")

#if (GTEST_SRC_DIR AND
    #GMOCK_LIB AND
    #GMOCK_MAIN_LIB)

if (GMOCK_LIB AND GMOCK_MAIN_LIB)

  set (GTEST_GMOCK_LIB
       gtest
       ${GMOCK_LIB}
       ${GMOCK_MAIN_LIB}
      )

  set (GTEST_MATRIX_SRC
      test_main.cpp
      test_matrix4x4.cpp
      )

  include_directories (${MATRIX_SOURCE_DIR}/matrix
                       ${GMOCK_INCLUDE_DIR}
                       ${GTEST_INCLUDE_DIR}
                      )

  add_subdirectory(${GMOCK_ROOT_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

  add_executable (../gtest-test-matrix ${GTEST_MATRIX_SRC})
  target_link_libraries (../gtest-test-matrix matrix ${GMOCK_LIB} ${GMOCK_MAIN_LIB})
  add_test (MatrixGTest ../gtest-test-matrix)

  add_custom_target (check COMMAND ../gtest-test-matrix)

endif (GMOCK_LIB AND GMOCK_MAIN_LIB)

#endif (GTEST_SRC_DIR AND
       #GMOCK_LIB AND
       #GMOCK_MAIN_LIB)
