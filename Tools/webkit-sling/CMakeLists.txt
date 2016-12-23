cmake_minimum_required(VERSION 2.8.12)

project(sling-extensions)

set(WEBKIT_ROOT_DIR "${CMAKE_SOURCE_DIR}/../webkit-sling" CACHE PATH "Source directory of webkit-sling")
set(WEBKIT_LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/../WebKitLibraries" CACHE PATH "Path to webkit libraries directory")
set(WEBKIT_BUILD_DIR "${WEBKIT_ROOT_DIR}/WebKitBuild" CACHE PATH "Path to webkit build directory")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" "${WEBKIT_ROOT_DIR}/Source/cmake")
include(WebKitFeatures)
include(OptionsSling)

include(PlatformAll)

set(WEBKIT_SOURCE_DIR "${WEBKIT_ROOT_DIR}/Source")
set(JAVASCRIPTCORE_DIR "${WEBKIT_SOURCE_DIR}/JavaScriptCore")
set(WEBCORE_DIR "${WEBKIT_SOURCE_DIR}/WebCore")
set(WEBKIT2_DIR "${WEBKIT_SOURCE_DIR}/WebKit2")
set(WTF_DIR "${WEBKIT_SOURCE_DIR}/WTF")
set(WEBKIT_DERIVED_SOURCES_DIR "${WEBKIT_BUILD_DIR}/DerivedSources")

set(EXTENSIONS_INJECTEDBUNDLE_DIR "${CMAKE_SOURCE_DIR}/InjectedBundle")
set(EXTENSIONS_SCRIPTEXTENSION_DIR "${CMAKE_SOURCE_DIR}/ScriptExtension")
set(EXTENSIONS_TOOLS_DIR "${CMAKE_SOURCE_DIR}/Tools")

set(DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources")
set(DERIVED_SOURCES_INJECTEDBUNDLE_DIR "${DERIVED_SOURCES_DIR}/InjectedBundle")
set(DERIVED_SOURCES_SCRIPTEXTENSION_DIR "${DERIVED_SOURCES_DIR}/ScriptExtension")

file(MAKE_DIRECTORY ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR})
file(MAKE_DIRECTORY ${DERIVED_SOURCES_SCRIPTEXTENSION_DIR})

set(Extensions_INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/InjectedBundle
    ${EXTENSIONS_INJECTEDBUNDLE_DIR}
    ${EXTENSIONS_INJECTEDBUNDLE_DIR}/API
    ${EXTENSIONS_INJECTEDBUNDLE_DIR}/API/c
    ${EXTENSIONS_INJECTEDBUNDLE_DIR}/IPC
    ${EXTENSIONS_SCRIPTEXTENSION_DIR}
    ${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings
    ${JAVASCRIPTCORE_DIR}
    ${JAVASCRIPTCORE_DIR}/ForwardingHeaders
    ${WEBKIT2_DIR}/Shared/API/c
    ${WEBKIT2_DIR}/UIProcess/API/C
    ${WEBKIT2_DIR}/WebProcess/InjectedBundle/API/c
    ${WTF_DIR}
    ${WEBKIT_DERIVED_SOURCES_DIR}
    ${WEBKIT_DERIVED_SOURCES_DIR}/ForwardingHeaders
    ${WEBKIT_DERIVED_SOURCES_DIR}/ForwardingHeaders/JavaScriptCore
    ${WEBKIT_DERIVED_SOURCES_DIR}/WebKit2/include
    ${CMAKE_BINARY_DIR}
    ${DERIVED_SOURCES_DIR}
    ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}
    ${DERIVED_SOURCES_SCRIPTEXTENSION_DIR}
    ${WEBKIT_BUILD_DIR}
    ${WEBKIT_SOURCE_DIR}
    ${WEBKIT_LIBRARIES_DIR}/include
)

EXTENSIONS_INCLUDE_CONFIG_FILES_IF_EXISTS()

include_directories(${Extensions_INCLUDE_DIRECTORIES})

if (${MSVC_CXX_ARCHITECTURE_ID} STREQUAL "X86")
    link_directories("${CMAKE_BINARY_DIR}/lib32" "${WEBKIT_BUILD_DIR}/lib32")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin32)
else ()
    link_directories("${CMAKE_BINARY_DIR}/lib64" "${WEBKIT_BUILD_DIR}/lib64")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin64)
endif ()

remove_definitions(-DBUILDING_SLING__=1)
add_definitions(-DHAVE_CONFIG_H=1 -DBUILDING_WITH_CMAKE)

add_subdirectory(InjectedBundle)
add_subdirectory(ScriptExtension)
add_subdirectory(Tools)

add_dependencies(ExtensionsScriptExtension ExtensionsInjectedBundleLib)
add_dependencies(ExtensionsInjectedBundle ExtensionsScriptExtension)
add_dependencies(ExtensionsTestRunner ExtensionsInjectedBundle)
