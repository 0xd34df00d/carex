project(carex)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++0x")
set(CMAKE_AUTOMOC TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Qwt REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${QWT_INCLUDE_DIRS})

set(SRCS
	main.cpp
	carex.cpp
	profiler.cpp
	)
set(FORMS
	carex.ui
	)
QT4_WRAP_UI(UIS_H ${FORMS})
add_executable(carex ${SRCS} ${UIS_H})
target_link_libraries(carex ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QWT_LIBRARIES})
install(TARGETS carex RUNTIME DESTINATION bin)
