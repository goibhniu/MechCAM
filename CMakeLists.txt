project(MechCAM)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(MechCAM_SRCS
    src/line.cpp
    src/point.cpp
    src/caddrawing.cpp
    src/MechCAM.cpp
    src/main.cpp
    src/toolpath.cpp
    src/contourtoolpath.cpp
    src/operationsmanager.cpp
    src/machine.cpp
    src/systemconfiguration.cpp
)
set(MechCAM_RCS MechCAM.qrc)
QT4_ADD_RESOURCES( MechCAM_RC_SRCS ${MechCAM_RCS} )
qt4_automoc(${MechCAM_SRCS})
add_executable(MechCAM ${MechCAM_SRCS} ${MechCAM_RC_SRCS})
target_link_libraries(MechCAM ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} QtOpenGL)
