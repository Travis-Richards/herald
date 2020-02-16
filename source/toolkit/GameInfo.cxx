#include "GameInfo.h"

#include "ScopedPtr.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace herald {

namespace tk {

namespace {

/// The implementation of a game information interface.
class GameInfoImpl final : public GameInfo {
  /// The root JSON object of the info.
  QJsonObject root_object;
public:
  /// Constructs a game information instance from a file.
  /// @param filename The name of the file to open.
  GameInfoImpl(const char* filename) {

    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      root_object = QJsonDocument::fromJson(file.readAll()).object();
    }
  }
  /// Accesses the API name.
  QString api() const {
    return root_object["api"].toString();
  }
  /// Accesses the author name.
  QString author() const {
    return root_object["author"].toString();
  }
  /// Accesses the title of the game.
  QString title() const override {
    return root_object["title"].toString();
  }
};

} // namespace

ScopedPtr<GameInfo> GameInfo::open(const char* filename) {
  return new GameInfoImpl(filename);
}

} // namespace tk

} // namespace herald
