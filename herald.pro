CONFIG += qt

QT += "widgets"

DEFINES += NO_CONFIG=1

RESOURCES += icons.qrc

SOURCES += source/main.cxx

SOURCES += source/ActiveGame.cxx
SOURCES += source/ActiveGameList.cxx
SOURCES += source/BackgroundModifier.cxx
SOURCES += source/BackgroundView.cxx
SOURCES += source/CentralWidget.cxx
SOURCES += source/ControlPanel.cxx
SOURCES += source/Engine.cxx
SOURCES += source/ErrorLog.cxx
SOURCES += source/GameInfo.cxx
SOURCES += source/GameList.cxx
SOURCES += source/GameListView.cxx
SOURCES += source/Interpreter.cxx
SOURCES += source/KeyController.cxx
SOURCES += source/LineBuffer.cxx
SOURCES += source/MainWindow.cxx
SOURCES += source/Matrix.cxx
SOURCES += source/MenuBuilder.cxx
SOURCES += source/ObjectMap.cxx
SOURCES += source/ObjectBuilder.cxx
SOURCES += source/ObjectMapView.cxx
SOURCES += source/PathSetting.cxx
SOURCES += source/ProcessApi.cxx
SOURCES += source/Room.cxx
SOURCES += source/RoomView.cxx
SOURCES += source/RoomBuilder.cxx
SOURCES += source/Scene.cxx
SOURCES += source/SceneView.cxx
SOURCES += source/SettingsDialog.cxx
SOURCES += source/Texture.cxx
SOURCES += source/TextureAnimation.cxx
SOURCES += source/TextureAnimationGroup.cxx
SOURCES += source/Writer.cxx

HEADERS += source/ActiveGame.h
HEADERS += source/ActiveGameList.h
HEADERS += source/Background.h
HEADERS += source/BackgroundModifier.h
HEADERS += source/BackgroundView.h
HEADERS += source/Api.h
HEADERS += source/CentralWidget.h
HEADERS += source/ControlPanel.h
HEADERS += source/Controller.h
HEADERS += source/Engine.h
HEADERS += source/ErrorLog.h
HEADERS += source/GameInfo.h
HEADERS += source/GameList.h
HEADERS += source/GameListView.h
HEADERS += source/Interpreter.h
HEADERS += source/KeyController.h
HEADERS += source/LineBuffer.h
HEADERS += source/MainWindow.h
HEADERs += source/Matrix.h
HEADERS += source/MenuBuilder.h
HEADERS += source/Object.h
HEADERS += source/ObjectMap.h
HEADERS += source/ObjectMapBuilder.h
HEADERS += source/ObjectMapView.h
HEADERS += source/PathSetting.h
HEADERS += source/ProcessApi.h
HEADERS += source/Room.h
HEADERS += source/RoomView.h
HEADERS += source/RoomBuilder.h
HEADERS += source/Scene.h
HEADERS += source/SceneView.h
HEADERS += source/SettingsDialog.h
HEADERS += source/Texture.h
HEADERS += source/TextureAnimation.h
HEADERS += source/TextureAnimationGroup.h
HEADERS += source/Tile.h
HEADERS += source/Writer.h

SOURCES += source/lang/Lexer.cxx
SOURCES += source/lang/Parser.cxx
SOURCES += source/lang/ParseTree.cxx
SOURCES += source/lang/SyntaxChecker.cxx
SOURCES += source/lang/Token.cxx

HEADERS += source/lang/Lexer.h
HEADERS += source/lang/Parser.h
HEADERS += source/lang/ParseTree.h
HEADERS += source/lang/SyntaxChecker.h
HEADERS += source/lang/Token.h
