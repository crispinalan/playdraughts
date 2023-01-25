cmake_minimum_required(VERSION 3.22.0)

project(draughts)

set(BUILD_MAJOR "1")
set(BUILD_MINOR "1")
set(BUILD_MICRO "0")

#set package meta data
set(PACKAGE "draughts")
set(PACKAGE_NAME "draughts")
set(PACKAGE_VERSION   "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_MICRO}")
set(PACKAGE_VERSION   "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_MICRO}")
set(PACKAGE_URL       "https://github.com/crispinalan/draughts")

# qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
#add_definitions(-DQT_NO_DEBUG_OUTPUT)

# required qt packages
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Core REQUIRED)
find_package (Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Multimedia REQUIRED)

#header files
set (draughts_HDRS
	boardwidget.h
	)
#source files
set (draughts_SRCS
	main.cpp
	mainwindow.cpp
	boardwidget.cpp
	)
#ui files
set (draughts_UIS
	mainwindow.ui

	)
QT5_WRAP_UI(UIS ${draughts_UIS})

# resource files
set (draughts_RSCS
	icons.qrc		
    )
QT5_ADD_RESOURCES(RSCS ${draughts_RSCS})

#set MOCs
set (draughts_MOCS
	mainwindow.h	
	)
QT5_WRAP_CPP(MOCS ${draughts_MOCS})

# add files defined above
add_executable (draughts ${draughts_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries ( draughts  Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::Xml Qt5::Multimedia)


INSTALL(TARGETS
    draughts
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)


