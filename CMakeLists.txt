#
# CMake configuratoin for nara-to-yaml
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

PROJECT(nara-to-yaml LANGUAGES C)

INCLUDE(GNUInstallDirs)

IF (NOT DEFINED SHOULD_OMIT_RPATHS)
    OPTION(SHOULD_OMIT_RPATHS "Do not embed library prefix paths into executables." FALSE)
ENDIF ()
IF (NOT DEFINED ICONV_PREFIX)
    SET(ICONV_PREFIX "" CACHE PATH "Install prefix for the iconv library.")
ENDIF ()

OPTION(NARA_1976_FORMAT "If true the program uses the fixed record size 1976 NARA file format." FALSE)

IF (ICONV_PREFIX)
    SET(Iconv_IS_BUILT_IN FALSE)
    SET(CMAKE_PREFIX_PATH ${ICONV_PREFIX} ${CMAKE_PREFIX_PATH})
    FIND_PACKAGE(Iconv)
    IF (Iconv_IS_BUILT_IN)
        MESSAGE(WARNING "The C library has built-in support for iconv; external libraries will not be used.")
    ENDIF ()
ELSE ()
    FIND_PACKAGE(Iconv)
ENDIF ()
IF (NOT Iconv_IS_BUILT_IN)
    IF (NOT Iconv_FOUND)
        MESSAGE(FATAL "No iconv library support found.")
    ENDIF ()
ENDIF ()

IF (SHOULD_OMIT_RPATHS)
    SET(CMAKE_SKIP_RPATH TRUE)
ENDIF()

CONFIGURE_FILE(nara_base.h.in nara_base.h)

ADD_EXECUTABLE(nara-to-yaml nara_base.c nara_state_header.c nara_record_header.c nara_record.c nara-to-yaml.c)
IF (NARA_1976_FORMAT)
    SET_SOURCE_FILES_PROPERTIES(nara_record.c PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_classroom_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_district_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_school_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_classroom.h;${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_district.h;${CMAKE_CURRENT_SOURCE_DIR}/1976/nara_school.h")
ELSE ()
    SET_SOURCE_FILES_PROPERTIES(nara_record.c PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_classroom_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_district_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_school_impl.c;${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_classroom.h;${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_district.h;${CMAKE_CURRENT_SOURCE_DIR}/pre-1976/nara_school.h")
ENDIF()
IF (Iconv_INCLUDE_DIRS)
    TARGET_INCLUDE_DIRECTORIES(nara-to-yaml PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
ELSE()
    TARGET_INCLUDE_DIRECTORIES(nara-to-yaml PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${Iconv_INCLUDE_DIRS})
ENDIF ()
IF (Iconv_LIBRARIES)
    TARGET_LINK_LIBRARIES(nara-to-yaml ${Iconv_LIBRARIES})
ENDIF ()

INSTALL(TARGETS nara-to-yaml DESTINATION ${CMAKE_INSTALL_BINDIR})
