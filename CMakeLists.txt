cmake_minimum_required(VERSION 4.2.0)

project(Kolekto C)

# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable
add_executable(Kolekto src/main.c)

target_link_directories(Kolekto PRIVATE src/)

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
)

# ── Tests ──────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)
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
endif()
