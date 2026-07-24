cmake_minimum_required(VERSION 4.2.0)

project(Kolekto C)


# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable
add_executable(Kolekto src/main.c)
target_link_directories(Kolekto PRIVATE src/)

# ── Cppcheck ──────────────────────────────────────────────────────────────────
find_program(CPPCHECK cppcheck REQUIRED)
set(CMAKE_C_CPPCHECK
    ${CPPCHECK}
    --enable=all
    --check-level=exhaustive
    --suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt
    --error-exitcode=1
    --inline-suppr
    --template=gcc
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/test
)

# ── Compiler and Linker ───────────────────────────────────────────────────────
# Strict and pedantic compiler flags (applied to everything)
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wformat=2
    -Wconversion
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wfloat-equal
    -Wundef
    -Wlogical-op
    -Wduplicated-cond
    -Wduplicated-branches
    --coverage
)

# ── Tests ─────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/lib)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG        v2.6.1
    )
    set(UNITY_NO_OUTPUT  TRUE)
    set(UNITY_NO_EXTRA   TRUE)
    FetchContent_MakeAvailable(unity)

    # Unity's own code doesn't pass -Wfloat-equal cleanly
    target_compile_options(unity PRIVATE -Wno-float-equal)

    add_subdirectory(test)

    # ── Coverage ──────────────────────────────────────────────────────────────
    find_program(GCOVR gcovr REQUIRED)
    add_custom_target(coverage
        COMMAND ${GCOVR} 
            --exclude 'build/*' 
            --exclude 'lib/*' 
            --lcov ./build/coverage.info
        COMMAND ${GCOVR} 
            --exclude 'build/*' 
            --exclude 'lib/*' 
            --html-details 
            -o ./build/coverage.html
        COMMENT "Generating code coverage report"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

endif()
