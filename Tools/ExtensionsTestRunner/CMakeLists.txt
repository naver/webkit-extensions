set(EXTENSIONS_TESTRUNNER_DIR "${EXTENSIONS_TOOLS_DIR}/ExtensionsTestRunner")

set(ExtensionsTestRunner_SOURCES
    ${EXTENSIONS_TESTRUNNER_DIR}/Options.cpp
    ${EXTENSIONS_TESTRUNNER_DIR}/TestController.cpp
)

file(GLOB_RECURSE ExtensionsTestRunner_HEADERS "${EXTENSIONS_TESTRUNNER_DIR}/*.h")

set(ExtensionsTestRunner_LIBRARIES
    ExtensionsInjectedBundle
)

set(ExtensionsTestRunner_INCLUDE_DIRECTORIES
    ${EXTENSIONS_TESTRUNNER_DIR}
    ${WEBKIT_DERIVED_SOURCES_DIR}/WebKit/Interfaces
)

EXTENSIONS_INCLUDE_CONFIG_FILES_IF_EXISTS()

EXTENSIONS_WRAP_SOURCELIST(${ExtensionsTestRunner_SOURCES})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsTestRunner_HEADERS})

include_directories(${ExtensionsTestRunner_INCLUDE_DIRECTORIES})

add_executable(ExtensionsTestRunner ${ExtensionsTestRunner_SOURCES} ${ExtensionsTestRunner_HEADERS})
target_link_libraries(ExtensionsTestRunner ${ExtensionsTestRunner_LIBRARIES})
