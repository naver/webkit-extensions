# This file is meant to be read from each project under cmake root.
#  - to refer other project.
#  - to share common variables and macros.

# -----------------------------------------------------------------------------
# Find common packages (used by all platforms)
# -----------------------------------------------------------------------------
find_package(BISON 2.1 REQUIRED)
if (!APPLE)
    find_package(FLEX 2.5.34 REQUIRED)
endif ()

# TODO Enforce version requirement for perl
find_package(Perl 5.10.0 REQUIRED)

find_package(PythonInterp 2.7.0 REQUIRED)

# We cannot check for RUBY_FOUND because it is set only when the full package is installed and
# the only thing we need is the interpreter. Unlike Python, cmake does not provide a macro
# for finding only the Ruby interpreter.
find_package(Ruby 1.9)
if (NOT RUBY_EXECUTABLE OR RUBY_VERSION VERSION_LESS 1.9)
    message(FATAL_ERROR "Ruby 1.9 or higher is required.")
endif ()

# CODE_GENERATOR_PREPROCESSOR_WITH_LINEMARKERS only matters with GCC >= 4.7.0.  Since this
# version, -P does not output empty lines, which currently breaks make_names.pl in
# WebCore. Investigating whether make_names.pl should be changed instead is left as an exercise to
# the reader.
if (MSVC)
    # FIXME: Some codegenerators don't support paths with spaces. So use the executable name only.
    get_filename_component(CODE_GENERATOR_PREPROCESSOR_EXECUTABLE ${CMAKE_CXX_COMPILER} ABSOLUTE)
    set(CODE_GENERATOR_PREPROCESSOR "\"${CODE_GENERATOR_PREPROCESSOR_EXECUTABLE}\" /nologo /EP")
    set(CODE_GENERATOR_PREPROCESSOR_WITH_LINEMARKERS "${CODE_GENERATOR_PREPROCESSOR}")
else ()
    set(CODE_GENERATOR_PREPROCESSOR "${CMAKE_CXX_COMPILER} -E -P -x c++")
    set(CODE_GENERATOR_PREPROCESSOR_WITH_LINEMARKERS "${CMAKE_CXX_COMPILER} -E -x c++")
endif ()

macro(EXTENSIONS_INCLUDE_CONFIG_FILES_IF_EXISTS)
    set(_file ${CMAKE_CURRENT_SOURCE_DIR}/Platform${CMAKE_SYSTEM_NAME}.cmake)
    if (EXISTS ${_file})
        message(STATUS "Using platform-specific CMakeLists: ${_file}")
        include(${_file})
    else ()
        message(STATUS "Platform-specific CMakeLists not found: ${_file}")
    endif ()
endmacro()

# Helper macro which wraps generate-bindings.pl script.
#   _output_source is a list name which will contain generated sources.(eg. WebCore_SOURCES)
#   _input_files are IDL files to generate.
#   _base_dir is base directory where script is called.
#   _idl_includes is value of --include argument. (eg. --include=${WEBCORE_DIR}/bindings/js)
#   _features is a value of --defines argument.
#   _destination is a value of --outputDir argument.
#   _prefix is a prefix of output files. (eg. JS - it makes JSXXX.cpp JSXXX.h from XXX.idl)
#   _generator is a value of --generator argument.
#   _supplemental_dependency_file is a value of --supplementalDependencyFile. (optional)
macro(GENERATE_BINDINGS _output_source _input_files _base_dir _idl_includes _features _destination _prefix _generator _extension _idl_attributes_file)
    set(BINDING_GENERATOR ${WEBCORE_DIR}/bindings/scripts/generate-bindings.pl)
    set(_args ${ARGN})
    list(LENGTH _args _argCount)
    if (_argCount GREATER 0)
        list(GET _args 0 _supplemental_dependency_file)
        if (_supplemental_dependency_file)
            set(_supplemental_dependency --supplementalDependencyFile ${_supplemental_dependency_file})
        endif ()
        list(GET _args 1 _additional_dependencies)
    endif ()

    set(COMMON_GENERATOR_DEPENDENCIES
        ${BINDING_GENERATOR}
        ${WEBCORE_DIR}/bindings/scripts/CodeGenerator.pm
        ${SCRIPTS_BINDINGS}
        ${_supplemental_dependency_file}
        ${_idl_attributes_file}
    )
    list(APPEND COMMON_GENERATOR_DEPENDENCIES ${_additional_dependencies})

    if (EXISTS ${WEBCORE_DIR}/bindings/scripts/CodeGenerator${_generator}.pm)
        list(APPEND COMMON_GENERATOR_DEPENDENCIES ${WEBCORE_DIR}/bindings/scripts/CodeGenerator${_generator}.pm)
    endif ()

    foreach (_file ${_input_files})
        get_filename_component(_name ${_file} NAME_WE)

        # Not all ObjC bindings generate a .mm file, and not all .mm files generated should be compiled.
        if (${_generator} STREQUAL "ObjC")
            list(FIND ObjC_BINDINGS_NO_MM ${_name} _no_mm_index)
            if (${_no_mm_index} EQUAL -1)
                set(_no_mm 0)
            else ()
                set(_no_mm 1)
            endif ()
        else ()
            set(_no_mm 0)
        endif ()

        if (${_no_mm})
            add_custom_command(
                OUTPUT ${_destination}/${_prefix}${_name}.h
                MAIN_DEPENDENCY ${_file}
                DEPENDS ${COMMON_GENERATOR_DEPENDENCIES}
                COMMAND ${PERL_EXECUTABLE} -I${WEBCORE_DIR}/bindings/scripts ${BINDING_GENERATOR} --defines "${_features}" --generator ${_generator} ${_idl_includes} --outputDir "${_destination}" --preprocessor "${CODE_GENERATOR_PREPROCESSOR}" --idlAttributesFile ${_idl_attributes_file} ${_supplemental_dependency} ${_file}
                WORKING_DIRECTORY ${_base_dir}
                VERBATIM)

            list(APPEND ${_output_source} ${_destination}/${_prefix}${_name}.h)
        else ()
            add_custom_command(
                OUTPUT ${_destination}/${_prefix}${_name}.${_extension} ${_destination}/${_prefix}${_name}.h
                MAIN_DEPENDENCY ${_file}
                DEPENDS ${COMMON_GENERATOR_DEPENDENCIES}
                COMMAND ${PERL_EXECUTABLE} -I${WEBCORE_DIR}/bindings/scripts ${BINDING_GENERATOR} --defines "${_features}" --generator ${_generator} ${_idl_includes} --outputDir "${_destination}" --preprocessor "${CODE_GENERATOR_PREPROCESSOR}" --idlAttributesFile ${_idl_attributes_file} ${_supplemental_dependency} ${_file}
                WORKING_DIRECTORY ${_base_dir}
                VERBATIM)
            list(APPEND ${_output_source} ${_destination}/${_prefix}${_name}.${_extension})
        endif ()
    endforeach ()
endmacro()

# Helper macro which wraps generate-message-receiver.py and generate-message-header.py scripts
#   _output_source is a list name which will contain generated sources.(eg. WebKit2_SOURCES)
#   _input_files are messages.in files to generate.
macro(GENERATE_EXTENSIONS_MESSAGE_SOURCES _output_source _input_files)
    foreach (_file ${_input_files})
        get_filename_component(_name ${_file} NAME_WE)
        get_filename_component(_path ${_file} ABSOLUTE)
        add_custom_command(
            OUTPUT ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}MessageReceiver.cpp ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}Messages.h
            MAIN_DEPENDENCY ${_path}
            DEPENDS ${WEBKIT2_DIR}/Scripts/webkit/__init__.py
                    ${WEBKIT2_DIR}/Scripts/webkit/messages.py
                    ${WEBKIT2_DIR}/Scripts/webkit/model.py
                    ${WEBKIT2_DIR}/Scripts/webkit/parser.py
            COMMAND ${PYTHON_EXECUTABLE} ${WEBKIT2_DIR}/Scripts/generate-message-receiver.py ${_path} > ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}MessageReceiver.cpp
            COMMAND ${PYTHON_EXECUTABLE} ${WEBKIT2_DIR}/Scripts/generate-messages-header.py ${_path} > ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}Messages.h
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/wrapup-message-receiver.py ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}MessageReceiver.cpp ${_path}
            WORKING_DIRECTORY ${WEBKIT2_DIR}
            VERBATIM)

        list(APPEND ${_output_source} ${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}/${_name}MessageReceiver.cpp)
    endforeach ()
endmacro()

macro(EXTENSIONS_WRAP_SOURCELIST)
    foreach (_file ${ARGN})
        get_filename_component(_name ${_file} NAME)
        get_filename_component(_path ${_file} ABSOLUTE)
        string(REPLACE "${_name}" "" _path "${_path}")
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _path "${_path}")

        if (NOT _file MATCHES "${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}")
            string(REGEX REPLACE "/" "\\\\\\\\" _sourcegroup "${_path}")
            source_group("${_sourcegroup}" FILES ${_file})
        endif ()
    endforeach ()

    source_group("DerivedSources" REGULAR_EXPRESSION "${DERIVED_SOURCES_INJECTEDBUNDLE_DIR}")
endmacro()
