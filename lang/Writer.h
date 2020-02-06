#pragma once

class QByteArray;

/// Used for writing strings that may be
/// sent to a game as a command.
class Writer {
public:
  /// Creates a build room request.
  static QByteArray build_room();
  /// Creates an exit request.
  static QByteArray exit();
};
