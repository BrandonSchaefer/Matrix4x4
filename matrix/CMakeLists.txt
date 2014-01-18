find_package (PkgConfig)

set (MATRIX_REQUIRED
     glesv2
    )

pkg_check_modules(PC_MATRIX REQUIRED ${MATRIX_REQUIRED})

set (MATRIX_SRC
     Matrix4x4.cpp
    )

set (MATRIX_HDR
     Matrix4x4.h
    )

add_library (matrix ${MATRIX_SRC} ${MATRIX_HDR} ${PC_MATRIX})
