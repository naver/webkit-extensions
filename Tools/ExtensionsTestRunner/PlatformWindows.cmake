list(APPEND ExtensionsTestRunner_SOURCES
    ${EXTENSIONS_TESTRUNNER_DIR}/win/PlatformWebViewWin.cpp
    ${EXTENSIONS_TESTRUNNER_DIR}/win/TestControllerWin.cpp
    ${EXTENSIONS_TESTRUNNER_DIR}/win/main.cpp
)

list(APPEND ExtensionsTestRunner_LIBRARIES
    DbgHelp
    WTF
    JavaScriptCore
    WebCoreTestSupport
    WebKitGUID
    comctl32
    comsupp
    comsuppw
    shlwapi
)
