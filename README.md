# webkit-extensions
Modern browsers have many third party extensions run with them. WebKitExtensions is a open-source project that provides Web Extension APIs to WebKit rendering engine.
The project is still in very early stage and we have a long way to go. We'd appreciate if you could join us on the project. Let's have fun!

## Prerequisite
* CMake - 3.4 preferred.
* Visual Studio 2015 - On Windows.
* WebKit - Requires built WebKit2 binaries and API headers.

## Getting Started
### Windows
With CMake installed, do like this:
```
mkdir build
cd build
cmake -G "Visual Studio 14 2015" -D WEBKIT_LIBRARIES_DIR=..\..\..\webkit\WebKitLibraries\win ..
```
Then open webkit-extensions.sln, Hit "Build Solution". or type `cmake --build .`

### Other Platforms
Not yet supported.

## Resources(Under Construction)
* Architectural Overview
* API Documentation
* External Links

## Contribution
There are a lot of extensions API we have to write. We would be really happy if you could contribute your code.

## Code Examples
Refer to ExtensionsTestRunner.
