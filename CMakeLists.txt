project( TPad )
cmake_minimum_required( VERSION 2.8.11 )

include( CheckCXXCompilerFlag )
check_cxx_compiler_flag( "-std=c++11"   COMPILER_SUPPORTS_CXX11 )
check_cxx_compiler_flag( "-std=c++0x"   COMPILER_SUPPORTS_CXX0X )
if( COMPILER_SUPPORTS_CXX11 )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11" )
    else()
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
    endif()
elseif( COMPILER_SUPPORTS_CXX0X)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
else()

endif()


set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_DIR} )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED
    Core
    Gui
    Widgets 
    PrintSupport
)

qt5_wrap_ui( UI_HEADERS src/mainwindow.ui src/about.ui src/credits.ui)
qt5_add_resources( QRCS src/icons.qrc )
add_executable( ${PROJECT_NAME} src/main.cpp src/mainwindow.cpp src/about.cpp src/translation.cpp src/credits.cpp src/save_load_files.cpp src/highlighter.cpp ${QRCS} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets Qt5::PrintSupport)
