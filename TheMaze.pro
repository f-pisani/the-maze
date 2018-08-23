QT  +=  core
QT  -=  gui
TEMPLATE    =   app

TARGET  =   TheMaze

SOURCES +=  main.cpp \
            ../MazeStudio/CMaze.cpp \
            TheMaze.cpp

HEADERS	+=  ../MazeStudio/CMaze.hpp \
            TheMaze.hpp

CONFIG	+=  C++11
CONFIG(release, debug|release): LIBS	+=  -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS  +=	-lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

LIBS	+=  -LY:/IDE/SFML-2.3.2/lib
INCLUDEPATH +=	Y:/IDE/SFML-2.3.2/include
DEPENDPATH  +=	Y:/IDE/SFML-2.3.2/include
