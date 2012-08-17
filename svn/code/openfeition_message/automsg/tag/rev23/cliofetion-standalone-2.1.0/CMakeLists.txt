project(cliofetion-standalone)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(TARNAME cliofetion-standalone)


# program name, version etc
set(OFETION_VERSION_MAJOR "2")
set(OFETION_VERSION_MINER "1")
set(OFETION_VERSION_PATCH "0")

# library API version
set(LIBOFETION_MAJOR_VERSION "1")
set(LIBOFETION_MINOR_VERSION "1")
set(LIBOFETION_PATCH_VERSION "0")

# library ABI version
set(LIBOFETION_SO_CUR_VERSION "1")
set(LIBOFETION_SO_REV_VERSION "0")
set(LIBOFETION_SO_AGE_VERSION "0")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")

set(LIBOFETION_VERSION_STRING
    "${LIBOFETION_MAJOR_VERSION}.${LIBOFETION_MINOR_VERSION}.${LIBOFETION_PATCH_VERSION}"
)
set(LIBOFETION_SO_VERION_STRING
    "${LIBOFETION_SO_CUR_VERSION}.${LIBOFETION_SO_REV_VERSION}.${LIBOFETION_SO_AGE_VERSION}"
)

set(PACKAGE_VERSION "${OFETION_VERSION_MAJOR}.${OFETION_VERSION_MINER}.${OFETION_VERSION_PATCH}")
set(PACKAGE_NAME "openfetion")
set(PACKAGE_TARNAME "${TARNAME}")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "http://code.google.com/p/ofetion/issues/list")

find_package(PkgConfig REQUIRED)

##########################################################################
if(NOT DEFINED LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif(NOT DEFINED LIB_INSTALL_DIR)
if(NOT DEFINED INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/")
endif(NOT DEFINED INCLUDE_INSTALL_DIR)
##########################################################################
if(NOT DEFINED BIN_INSTALL_DIR)
    set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
endif(NOT DEFINED BIN_INSTALL_DIR)
if(NOT DEFINED LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif(NOT DEFINED LIB_INSTALL_DIR)
if(NOT DEFINED DATA_INSTALL_DIR)
    set(DATA_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share")
endif(NOT DEFINED DATA_INSTALL_DIR)
if(NOT DEFINED INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
endif(NOT DEFINED INCLUDE_INSTALL_DIR)

add_definitions(-DRESOURCE_DIR="${DATA_INSTALL_DIR}/openfetion/resource/")
install(
FILES resource/city.xml resource/province.xml
DESTINATION ${DATA_INSTALL_DIR}/openfetion/resource
)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
pkg_check_modules(OPENSSL REQUIRED openssl)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/libdb
    ${LIBXML2_INCLUDE_DIRS} 
    ${OPENSSL_INCLUDE_DIRS} 
    ${SQLITE3_INCLUDE_DIRS}
)

set(cliofetion_SRC
    src/cliofetion.c
    libdb/cdb_c_file.c
    libdb/cdb_u_file.c
    libdb/cdb_d_file.c
    lib/fetion_buddylist.c
    lib/fetion_config.c
    lib/fetion_connection.c
    lib/fetion_contact.c
    lib/fetion_conversation.c
    lib/fetion_debug.c
    lib/fetion_directsms.c
    lib/fetion_group.c
    lib/fetion_history.c
    lib/fetion_list.c
    lib/fetion_login.c
    lib/fetion_message.c
    lib/fetion_share.c
    lib/fetion_sip.c
    lib/fetion_user.c

)

add_executable(cliofetion ${cliofetion_SRC})
target_link_libraries(cliofetion ${LIBXML2_LIBRARIES} ${OPENSSL_LIBRARIES} ${SQLITE3_LIBRARIES})
install(TARGETS cliofetion RUNTIME DESTINATION ${BIN_INSTALL_DIR})

pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)
pkg_check_modules(OPENSSL REQUIRED openssl)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
