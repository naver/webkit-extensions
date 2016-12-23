set(ExtensionsInjectedBundleLib_SOURCES
    API/APIString.cpp
    API/Array.cpp
    API/Data.cpp
    API/Dictionary.cpp
    API/Function.cpp
    API/Number.cpp
    API/Object.cpp
    API/PageHandle.cpp
    API/ProcessLocalData.cpp
    API/Prototype.cpp

    IPC/ProcessIdentifier.cpp

    InjectedBundle.cpp
    InjectedBundlePage.cpp
    InjectedBundlePageProxy.cpp
    InjectedBundleProcess.cpp
    InjectedBundleProxy.cpp
    InjectedBundleView.cpp
)

file(GLOB_RECURSE ExtensionsInjectedBundleLib_HEADERS "${EXTENSIONS_INJECTEDBUNDLE_DIR}/*.h")

set(ExtensionsInjectedBundleLib_MESSAGES_IN_FILES
    InjectedBundle.messages.in
    InjectedBundleProcess.messages.in
    InjectedBundlePage.messages.in
    InjectedBundlePageProxy.messages.in
    InjectedBundleProxy.messages.in
)

GENERATE_EXTENSIONS_MESSAGE_SOURCES(ExtensionsInjectedBundleLib_SOURCES "${ExtensionsInjectedBundleLib_MESSAGES_IN_FILES}")

set(ExtensionsInjectedBundle_SOURCES
    API/c/WebKitExtension.cpp

    InjectedBundleMain.cpp
    InjectedBundleProxyMain.cpp
)

set(ExtensionsInjectedBundle_LIBRARIES
    ExtensionsInjectedBundleLib
    ExtensionsScriptExtension
    JavaScriptCore
    WTF
    reflect
    ${WIN32_SYSTEM_LIBRARIES}
)

EXTENSIONS_INCLUDE_CONFIG_FILES_IF_EXISTS()

EXTENSIONS_WRAP_SOURCELIST(${ExtensionsInjectedLibBundle_SOURCES})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsInjectedLibBundle_HEADERS})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsInjectedBundleLib_MESSAGES_IN_FILES})

## for sling build.
if (IS_SLING)
    list(APPEND ExtensionsInjectedBundle_LIBRARIES SlingWebKit)
else ()
    list(APPEND ExtensionsInjectedBundle_LIBRARIES WebKit)
endif ()

add_library(ExtensionsInjectedBundleLib STATIC ${ExtensionsInjectedBundleLib_SOURCES} ${ExtensionsInjectedBundleLib_HEADERS})
add_library(ExtensionsInjectedBundle SHARED ${ExtensionsInjectedBundle_SOURCES} ${ExtensionsInjectedBundle_HEADERS})
target_link_libraries(ExtensionsInjectedBundle ${ExtensionsInjectedBundle_LIBRARIES} debug ${WIN32_SYSTEM_LIBRARIES_DEBUG} optimized ${WIN32_SYSTEM_LIBRARIES_RELEASE})