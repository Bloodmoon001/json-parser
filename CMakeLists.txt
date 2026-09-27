cmake_minimum_required(VERSION 3.20)
project(JsonParser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем vcpkg toolchain (на случай, если не задан снаружи)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# --- Библиотека json (наш парсер) ---
add_library(json
    src/Value.cpp
    src/Parser.cpp
    src/Serializer.cpp
)

target_include_directories(json PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# --- Демо-приложение ---
add_executable(json_demo
    apps/main.cpp
)
target_link_libraries(json_demo PRIVATE json)

# --- Тесты (только если включён BUILD_TESTING) ---
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)

    add_executable(json_tests
        tests/test_parser.cpp
    )
    target_link_libraries(json_tests PRIVATE
        json
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(json_tests)
endif()

# Все exe кладём в bin/
set_target_properties(json_demo json_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)