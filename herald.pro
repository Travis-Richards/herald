CONFIG += qt

QT += "widgets"
QT += "3dcore"
QT += "3dextras"

DEFINES += NO_CONFIG=1

RESOURCES += icons.qrc

SOURCES += main.cxx

SOURCES += Api.cxx
SOURCES += CentralWidget.cxx
SOURCES += ControlPanel.cxx
SOURCES += Engine.cxx
SOURCES += GameInfo.cxx
SOURCES += GameList.cxx
SOURCES += GameListView.cxx
SOURCES += LineBuffer.cxx
SOURCES += MainWindow.cxx
SOURCES += MenuBuilder.cxx
SOURCES += ProcessApi.cxx
SOURCES += Scene.cxx
SOURCES += SceneView.cxx
SOURCES += Texture.cxx
SOURCES += TextureList.cxx

HEADERS += Api.h
HEADERS += CentralWidget.h
HEADERS += ControlPanel.h
HEADERS += Engine.h
HEADERS += GameInfo.h
HEADERS += GameList.h
HEADERS += GameListView.h
HEADERS += LineBuffer.h
HEADERS += MainWindow.h
HEADERS += MenuBuilder.h
HEADERS += ProcessApi.h
HEADERS += Scene.h
HEADERS += SceneView.h
HEADERS += Texture.h
HEADERS += TextureList.h

SOURCES += lang/Interpreter.cxx
SOURCES += lang/Lexer.cxx
SOURCES += lang/Parser.cxx
SOURCES += lang/ParseTree.cxx
SOURCES += lang/Writer.cxx

HEADERS += lang/Lexer.h
HEADERS += lang/Interpreter.h
HEADERS += lang/Parser.h
HEADERS += lang/ParseTree.h
HEADERS += lang/Writer.h
