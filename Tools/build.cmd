@echo off

echo %WEBKIT_ROOT%
if "%WEBKIT_ROOT%" == "" (
    echo ERROR : Set your WEBKIT_ROOT.
    goto exit
)

if not exist %WEBKIT_ROOT% (
    echo ERROR : %WEBKIT_ROOT% not exists.
    goto exit
)

set TOOLS_DIR=..\Tools
set BUILD_DIR=..\Build

if /I "%1" == "release" (
    set CONFIG=Release
    goto switchToWebKit
) else if /I "%1" == "debug" (
    set CONFIG=Debug
    goto switchToWebKit
) else if /I "%1" == "sling-release" (
    set CONFIG=Release
    goto switchToSling
) else if /I "%1" == "sling-debug" (
    set CONFIG=Debug
    goto switchToSling
) else if /I "%1" == "clean" (
    goto clean
) else (
    echo "%1" config is not found.
)

mkdir %BUILD_DIR%

:switchToWebKit
echo * switch to webkit-sling configuration...
xcopy cmakeconfig.h %BUILD_DIR%\ /e /h /k /y
xcopy %TOOLS_DIR%\webkit-sling\*.* .. /e /h /k /y
del /f ..\OptionsSlingWindows.cmake

if not %ERRORLEVEL%==0 (
    echo ERROR : switch to webkit-sling failed.
    goto exit
)

:switchToSling
echo * switch to sling configuration...
xcopy /s /f /y cmakeconfig.h %BUILD_DIR%\
xcopy %TOOLS_DIR%\sling\*.* .. /e /h /k /y

if not %ERRORLEVEL%==0 (
    echo ERROR : switch to sling failed.
    goto exit
)

if /I "%2" == "build" (
    goto build
)

cd %BUILD_DIR%

echo.
echo * generating ms projects ...
cmake -DWEBKIT_ROOT_DIR=%WEBKIT_ROOT% -DWEBKIT_LIBRARIES_DIR=%WEBKIT_ROOT%\WebKitLibraries -DWEBKIT_BUILD_DIR=%WEBKIT_ROOT%\WebKitBuild ..

if not %ERRORLEVEL%==0 (
    echo ERROR : ms projects generation failed.
    goto exit
)

:build
echo.
echo * building ... %CONFIG%
cmake --build . --config %CONFIG%

if not %ERRORLEVEL%==0 (
    echo ERROR : build failed.
    goto exit
)

echo.
echo * copying built files to WebKitBuild ..
copy bin32\%CONFIG%\* %WEBKIT_ROOT%\WebKitBuild\%CONFIG%\bin32
copy lib32\%CONFIG%\ExtensionsInjectedBundle.lib %WEBKIT_ROOT%\WebKitBuild\%CONFIG%\lib32
xcopy /y /d /f ..\InjectedBundle\API\c\WebKitExtension.h %WEBKIT_ROOT%\WebKitBuild\%CONFIG%\include\WebKitExtensions\
cd %TOOLS_DIR%

if not %ERRORLEVEL% == 0 (
    echo ERROR : copying built files to WebKitBuild..failed.
    goto exit
)

goto exit

:clean
echo.
echo * cleaning ... sling-extensions
rmdir /s /q %BUILD_DIR%

echo * switch to webkit-sling configuration...
xcopy %TOOLS_DIR%\webkit-sling\*.* .. /e /h /k /y
del /f ..\OptionsSlingWindows.cmake

if not %ERRORLEVEL% == 0 (
    echo ERROR : clean failed.
    goto exit
)

:exit
cd %TOOLS_DIR%
EXIT /b
