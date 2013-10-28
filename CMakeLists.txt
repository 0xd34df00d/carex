project(carex)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(carex_SRCS carex.cpp main.cpp)
qt4_automoc(${carex_SRCS})
add_executable(carex ${carex_SRCS})
target_link_libraries(carex ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS carex RUNTIME DESTINATION bin)
