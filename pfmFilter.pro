RC_FILE = pfmFilter.rc
RESOURCES = icons.qrc
contains(QT_CONFIG, opengl): QT += opengl
QT += 
INCLUDEPATH += /c/PFM_ABEv7.0.0_Win64/include
LIBS += -L /c/PFM_ABEv7.0.0_Win64/lib -lpfm -lnvutility -lgdal -lxml2 -lpoppler -liconv -lwsock32
DEFINES += WIN32 NVWIN3X
CONFIG += console
CONFIG += static
QMAKE_LFLAGS += 
######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 14:39:48 2020
######################################################################

TEMPLATE = app
TARGET = pfmFilter
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += filterThread.hpp \
           optionPage.hpp \
           optionPageHelp.hpp \
           pfmFilter.hpp \
           pfmFilterDef.hpp \
           pfmFilterHelp.hpp \
           runPage.hpp \
           startPage.hpp \
           startPageHelp.hpp \
           version.hpp
SOURCES += filterThread.cpp \
           main.cpp \
           optionPage.cpp \
           pfmFilter.cpp \
           runPage.cpp \
           startPage.cpp
RESOURCES += icons.qrc
