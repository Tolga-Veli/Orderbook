cmake_minimum_required(VERSION 3.20)

project(Orderbook
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(OrderBookApp ${SOURCES})

enable_testing()
file(GLOB_RECURSE TEST_SOURCE
    "test/*.hpp"
    "test/*.cpp"
)

add_executable(OrderBookTests ${TEST_SOURCE})
target_link_libraries(OrderBookTests gtest gtest_main)
add_test(NAME OrderbookTests COMMAND OrderBookTests)
