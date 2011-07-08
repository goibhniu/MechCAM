project(MechCAM)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(BISON)
find_package(FLEX)


include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} /usr/include/opencascade)

set(MechCAM_SRCS
    src/circle.cpp
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
    src/stocksetup.cpp
    src/fileprocessor.cpp
    src/variable.cpp
    src/variabledialog.cpp
    src/lua_integration.cpp
    src/STEPNC_Write/ncfunc.cxx
    src/STEPNC_Write/partno.cxx
    src/STEPNC_Write/property.cxx
    src/STEPNC_Write/speed.cxx
    src/STEPNC_Write/coolant.cxx
    src/STEPNC_Write/cutter.cxx
    src/STEPNC_Write/goto.cxx
    src/STEPNC_Write/idlist.cxx
    src/STEPNC_Write/measures.cxx
    src/STEPNC_Write/units.cxx
    src/STEPNC_Write/STEPNCWrite.cxx
    src/APT_Write/APTWrite.cpp
    src/APT_Read/APTRead.cpp
    src/tinyxml/tinyxml.cpp
    src/tinyxml/tinyxmlerror.cpp
    src/tinyxml/tinyxmlparser.cpp
    src/tinyxml/tinystr.cpp
    src/Heeks_Read/heeks_reader.cpp
    src/MechCAM_Read/mechcam_read.cpp
    src/MechCAM_Write/mechcam_write.cpp


)

qt4_automoc(${MechCAM_SRCS})
add_executable(MechCAM ${MechCAM_SRCS}
    ${FLEX_MyScanner_OUTPUTS}
    ${BISON_MyParser_OUTPUTS}

)
target_link_libraries(MechCAM ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} QtOpenGL GLU fl lua TKernel)
