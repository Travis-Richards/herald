CONFIG += qt

QT += "widgets"

DEFINES += NO_CONFIG=1

RESOURCES += icons.qrc

SOURCES += source/main.cxx

SOURCES += source/ActiveGame.cxx
SOURCES += source/ActiveGameList.cxx
SOURCES += source/Api.cxx
SOURCES += source/CentralWidget.cxx
SOURCES += source/ControlPanel.cxx
SOURCES += source/Engine.cxx
SOURCES += source/ErrorLog.cxx
SOURCES += source/GameInfo.cxx
SOURCES += source/GameList.cxx
SOURCES += source/GameListView.cxx
SOURCES += source/LineBuffer.cxx
SOURCES += source/MainWindow.cxx
SOURCES += source/MenuBuilder.cxx
SOURCES += source/ProcessApi.cxx
SOURCES += source/RoomBuilder.cxx
SOURCES += source/Scene.cxx
SOURCES += source/SceneView.cxx
SOURCES += source/Texture.cxx
SOURCES += source/TextureList.cxx

HEADERS += source/ActiveGame.h
HEADERS += source/ActiveGameList.h
HEADERS += source/Api.h
HEADERS += source/CentralWidget.h
HEADERS += source/ControlPanel.h
HEADERS += source/Engine.h
HEADERS += source/ErrorLog.h
HEADERS += source/GameInfo.h
HEADERS += source/GameList.h
HEADERS += source/GameListView.h
HEADERS += source/LineBuffer.h
HEADERS += source/MainWindow.h
HEADERS += source/MenuBuilder.h
HEADERS += source/ProcessApi.h
HEADERS += source/RoomBuilder.h
HEADERS += source/Scene.h
HEADERS += source/SceneView.h
HEADERS += source/Texture.h
HEADERS += source/TextureList.h

SOURCES += source/lang/Interpreter.cxx
SOURCES += source/lang/Lexer.cxx
SOURCES += source/lang/Matrix.cxx
SOURCES += source/lang/Parser.cxx
SOURCES += source/lang/ParseTree.cxx
SOURCES += source/lang/Writer.cxx

HEADERS += source/lang/Lexer.h
HEADERS += source/lang/Interpreter.h
HEADERS += source/lang/Parser.h
HEADERS += source/lang/ParseTree.h
HEADERs += source/lang/Matrix.h
HEADERS += source/lang/Writer.h
