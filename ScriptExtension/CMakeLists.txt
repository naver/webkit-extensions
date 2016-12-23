set(ExtensionsScriptExtension_SOURCES
    Bindings/chrome/BrowserAction.cpp
    Bindings/chrome/BrowserActionHost.cpp
    Bindings/chrome/Chrome.cpp
    Bindings/chrome/ChromeHost.cpp
    Bindings/chrome/ChromeMain.cpp
    Bindings/chrome/ChromeStrings.cpp
    Bindings/chrome/ChromeWindows.cpp
    Bindings/chrome/ChromeWindowsHost.cpp
    Bindings/chrome/ContextMenus.cpp
    Bindings/chrome/ContextMenusHost.cpp 
    Bindings/chrome/Event.cpp
    Bindings/chrome/EventFactory.cpp
    Bindings/chrome/EventProxyFactory.cpp
    Bindings/chrome/EventFilter.cpp
    Bindings/chrome/EventProxy.cpp
    Bindings/chrome/I18n.cpp
    Bindings/chrome/I18nHost.cpp
    Bindings/chrome/Listener.cpp
    Bindings/chrome/MessageSender.cpp
    Bindings/chrome/PageResourceLoaderClient.cpp
    Bindings/chrome/Port.cpp
    Bindings/chrome/PortConnections.cpp
    Bindings/chrome/PortHost.cpp
    Bindings/chrome/RequestFilter.cpp
    Bindings/chrome/Runtime.cpp
    Bindings/chrome/RuntimeHost.cpp
    Bindings/chrome/Storage.cpp
    Bindings/chrome/StorageArea.cpp
    Bindings/chrome/StorageAreaHost.cpp
    Bindings/chrome/Tab.cpp
    Bindings/chrome/Tabs.cpp
    Bindings/chrome/TabsHost.cpp
    Bindings/chrome/UrlFilter.cpp
    Bindings/chrome/WebRequest.cpp
    Bindings/chrome/WebRequestEventProxy.cpp
    Bindings/chrome/WebRequestHost.cpp

    Bindings/test/TestUserExtension.cpp
    Bindings/test/TestExtension.cpp
    Bindings/test/TestExtensionMain.cpp
    Bindings/test/TestExtensionHost.cpp

    ScriptPrototype.cpp
    JSBinding.cpp
    JSValue.cpp
    JSWrappable.cpp
    ScriptExtension.cpp
    ScriptExtensionHost.cpp
)

file(GLOB_RECURSE ExtensionsScriptExtension_HEADERS "${EXTENSIONS_SCRIPTEXTENSION_DIR}/*.h")

set(ExtensionsScriptExtension_MESSAGES_IN_FILES
    Bindings/chrome/BrowserActionHost.messages.in
    Bindings/chrome/ChromeHost.messages.in
    Bindings/chrome/ChromeWindowsHost.messages.in
    Bindings/chrome/ContextMenusHost.messages.in
    Bindings/chrome/EventProxyFactory.messages.in
    Bindings/chrome/I18nHost.messages.in
    Bindings/chrome/Port.messages.in
    Bindings/chrome/PortHost.messages.in
    Bindings/chrome/Runtime.messages.in
    Bindings/chrome/RuntimeHost.messages.in
    Bindings/chrome/StorageAreaHost.messages.in
    Bindings/chrome/TabsHost.messages.in

    Bindings/test/TestExtensionHost.messages.in

    ScriptExtension.messages.in
    ScriptExtensionHost.messages.in
)

set(ExtensionsScriptExtension_IDL_FILES
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/BrowserAction.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Chrome.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/ChromeWindows.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/ContextMenus.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Event.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/I18n.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/MessageSender.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Port.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Runtime.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Storage.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/StorageArea.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Tab.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/Tabs.idl"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome/WebRequest.idl"

    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/test/TestExtension.idl"
)

GENERATE_BINDINGS(ExtensionsScriptExtension_SOURCES
    "${ExtensionsScriptExtension_IDL_FILES}"
    "${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings"
    "--include=${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings"
    "${FEATURE_DEFINES_WITH_SPACE_SEPARATOR}"
    ${DERIVED_SOURCES_SCRIPTEXTENSION_DIR} JS ScriptExtension cpp
    ${WEBCORE_DIR}/bindings/scripts/IDLAttributes.txt)

GENERATE_EXTENSIONS_MESSAGE_SOURCES(ExtensionsScriptExtension_SOURCES "${ExtensionsScriptExtension_MESSAGES_IN_FILES}")

set(ExtensionsScriptExtension_INCLUDE_DIRECTORIES
    ${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/test
    ${EXTENSIONS_SCRIPTEXTENSION_DIR}/Bindings/chrome
)

EXTENSIONS_INCLUDE_CONFIG_FILES_IF_EXISTS()

EXTENSIONS_WRAP_SOURCELIST(${ExtensionsScriptExtension_SOURCES})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsScriptExtension_HEADERS})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsScriptExtension_MESSAGES_IN_FILES})
EXTENSIONS_WRAP_SOURCELIST(${ExtensionsScriptExtension_IDL_FILES})

include_directories(${ExtensionsScriptExtension_INCLUDE_DIRECTORIES})

add_library(ExtensionsScriptExtension STATIC ${ExtensionsScriptExtension_SOURCES} ${ExtensionsScriptExtension_HEADERS})
