#/**********************************************************\ 
# Auto-generated Windows project definition file for the
# wxWidget_Sample project
#\**********************************************************/

# Windows template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Win/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Win/[^.]*.cpp
    Win/[^.]*.h
    Win/[^.]*.cmake
    )

# use this to add preprocessor definitions
add_definitions(
    /D "_ATL_STATIC_REGISTRY"
)

SOURCE_GROUP(Win FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_windows_plugin(${PROJECT_NAME} SOURCES)


# Add wxWidgets - see http://wiki.wxwidgets.org/FindWxWin.Cmake

#FIND_LIBRARY( WIN32_COMCTRL comctl32.lib PATHS "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib" )
#set( MY_WINDOWS_INSTALL_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib" )
target_link_libraries(${PROJECT_NAME} debug "${MY_WINDOWS_INSTALL_PATH}/comctl32.lib")
target_link_libraries(${PROJECT_NAME} debug "${MY_WINDOWS_INSTALL_PATH}/rpcrt4.lib")
target_link_libraries(${PROJECT_NAME} debug "${MY_WINDOWS_INSTALL_PATH}/winmm.lib")
target_link_libraries(${PROJECT_NAME} debug "${MY_WINDOWS_INSTALL_PATH}/advapi32.lib")
target_link_libraries(${PROJECT_NAME} debug "${MY_WINDOWS_INSTALL_PATH}/wsock32.lib")

target_link_libraries(${PROJECT_NAME} optimized "${MY_WINDOWS_INSTALL_PATH}/comctl32.lib")
target_link_libraries(${PROJECT_NAME} optimized "${MY_WINDOWS_INSTALL_PATH}/rpcrt4.lib")
target_link_libraries(${PROJECT_NAME} optimized "${MY_WINDOWS_INSTALL_PATH}/winmm.lib")
target_link_libraries(${PROJECT_NAME} optimized "${MY_WINDOWS_INSTALL_PATH}/advapi32.lib")
target_link_libraries(${PROJECT_NAME} optimized "${MY_WINDOWS_INSTALL_PATH}/wsock32.lib")


#set( WXWINDOWS_INSTALL_PATH C:/chhenning/wxWidgets/ )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WINDOWS__ -D__WXMSW__ -D__WXDEBUG__ -DWXDEBUG=1 -D__WIN32__ -DWINVER=0x0502 -DSTRICT")

#include paths
set( WXWINDOWS_INCLUDE ${WXWINDOWS_INSTALL_PATH}/include )
set( WXWINDOWS_SETUP_H_INCLUDE ${WXWINDOWS_INSTALL_PATH}/lib/vc_lib/mswud )

include_directories(${WXWINDOWS_INCLUDE})
include_directories(${WXWINDOWS_SETUP_H_INCLUDE})

#library path
set( WXWINDOWS_LIBRARY ${WXWINDOWS_INSTALL_PATH}/lib/vc_lib )

target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxbase29ud.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxmsw29ud_core.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxjpegd.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxpngd.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxtiffd.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxzlibd.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxexpatd.lib")
target_link_libraries(${PROJECT_NAME} debug "${WXWINDOWS_LIBRARY}/wxregexud.lib")

target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxbase29u.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxmsw29u_core.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxjpeg.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxpng.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxtiff.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxzlib.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxexpat.lib")
target_link_libraries(${PROJECT_NAME} optimized "${WXWINDOWS_LIBRARY}/wxregexu.lib")


# This is an example of how to add a build step to sign the plugin DLL before
# the WiX installer builds.  The first filename (certificate.pfx) should be
# the path to your pfx file.  If it requires a passphrase, the passphrase
# should be located inside the second file. If you don't need a passphrase
# then set the second filename to "".  If you don't want signtool to timestamp
# your DLL then make the last parameter "".
#
# Note that this will not attempt to sign if the certificate isn't there --
# that's so that you can have development machines without the cert and it'll
# still work. Your cert should only be on the build machine and shouldn't be in
# source control!
# -- uncomment lines below this to enable signing --
#firebreath_sign_plugin(${PROJECT_NAME}
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll")

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    )

set(WIX_HEAT_FLAGS
    -gg                 # Generate GUIDs
    -srd                # Suppress Root Dir
    -cg PluginDLLGroup  # Set the Component group name
    -dr INSTALLDIR      # Set the directory ID to put the files in
    )

add_wix_installer( ${PLUGIN_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/Win/WiX/wxWidget_SampleInstaller.wxs
    PluginDLLGroup
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/
    ${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${FBSTRING_PluginFileName}.dll
    ${PROJECT_NAME}
    )

# This is an example of how to add a build step to sign the WiX installer
# -- uncomment lines below this to enable signing --
#firebreath_sign_file("${PLUGIN_NAME}_WiXInstall"
#    "${FB_BIN_DIR}/${PLUGIN_NAME}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}.msi"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/certificate.pfx"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sign/passphrase.txt"
#    "http://timestamp.verisign.com/scripts/timestamp.dll")
