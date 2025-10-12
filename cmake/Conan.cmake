macro(run_conan PATH_TO_CONANFILE)
    find_program(CONAN_CMD conan)
    if (NOT CONAN_CMD)
        message(FATAL_ERROR "Conan not found! Please install it first: pip install conan")
    endif ()

    # Determine architecture based on CMAKE_SIZEOF_VOID_P
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CONAN_ARCH "x86_64")
    else ()
        set(CONAN_ARCH "x86")
    endif ()

    # Determine the correct output directory
    set(CONAN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/conan")
    file(MAKE_DIRECTORY ${CONAN_OUTPUT_DIR})

    # Prepare generator-specific settings
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        # Visual Studio generators are multi-config
        set(CONAN_GENERATOR_SETTINGS
                -c tools.cmake.cmaketoolchain:generator=${CMAKE_GENERATOR}
        )
    else ()
        # Single-config generators (like NMake)
        set(CONAN_GENERATOR_SETTINGS
                -c tools.cmake.cmaketoolchain:generator=${CMAKE_GENERATOR})
    endif ()

    # Capitalize the first letter
    string(SUBSTRING ${CMAKE_BUILD_TYPE} 0 1 first_letter)
    string(TOUPPER ${first_letter} first_letter)
    string(SUBSTRING ${CMAKE_BUILD_TYPE} 1 -1 rest_of_string)
    set(CAPITAL_BUILD_TYPE "${first_letter}${rest_of_string}")

    # Run Conan install
    execute_process(
            COMMAND ${CONAN_CMD} install ${PATH_TO_CONANFILE}
            --output-folder=${CONAN_OUTPUT_DIR}
            --build=missing
            --settings=build_type=${CAPITAL_BUILD_TYPE}
            --settings=arch=${CONAN_ARCH}
            -c tools.cmake.cmaketoolchain:user_presets=None
            ${CONAN_GENERATOR_SETTINGS}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            RESULT_VARIABLE conan_result
            OUTPUT_VARIABLE conan_output
            ERROR_VARIABLE conan_error
    )

    if (NOT "${conan_result}" STREQUAL "0")
        message(FATAL_ERROR "Conan install failed:\n${conan_error}")
    endif ()

    # Find the generated toolchain file
    set(TOOLCHAIN_CANDIDATES
            "${CONAN_OUTPUT_DIR}/conan_toolchain.cmake"
            "${CONAN_OUTPUT_DIR}/generators/conan_toolchain.cmake"
            "${CONAN_OUTPUT_DIR}/build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake"
    )

    set(FOUND_TOOLCHAIN "")
    foreach (CANDIDATE ${TOOLCHAIN_CANDIDATES})
        if (EXISTS ${CANDIDATE})
            set(FOUND_TOOLCHAIN ${CANDIDATE})
            break()
        endif ()
    endforeach ()

    if (NOT FOUND_TOOLCHAIN)
        message(FATAL_ERROR "Could not find conan_toolchain.cmake in:\n${TOOLCHAIN_CANDIDATES}")
    endif ()

    message(STATUS "Using Conan toolchain: ${FOUND_TOOLCHAIN}")
    include(${FOUND_TOOLCHAIN})
endmacro()