cmake_minimum_required(VERSION 3.20)

project(Orderbook VERSION 1.0 LANGUAGES CXX)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/FTXUI")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(Orderbook ${SOURCES} ${HEADERS})
# Visual Studio compatibility
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Orderbook)
target_compile_features(Orderbook PRIVATE cxx_std_23)

target_include_directories(Orderbook PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES
  ${SOURCES} ${HEADERS}
)

target_link_libraries(Orderbook PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
)

target_compile_options(Orderbook PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /Od /permissive- /W4>
  $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-g3 -O0 -fno-omit-frame-pointer>
)

#enable_testing()
#file(GLOB_RECURSE TEST_SOURCE CONFIGURE_DEPENDS
#  "${ROOT}/test/*.hpp"
#  "${ROOT}/test/*.cpp"
#)
#
#add_executable(OrderBookTests ${TEST_SOURCE})
#target_link_libraries(OrderBookTests PRIVATE gtest gtest_main)
#add_test(NAME OrderbookTests COMMAND OrderBookTests)

