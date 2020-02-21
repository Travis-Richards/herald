#pragma once

#include <cstddef>

class QSettings;
class QTableWidget;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class GameInfo;

/// The interface for a game list.
class GameList {
public:
  /// Creates a new game list.
  /// @returns A new game list.
  static ScopedPtr<GameList> make();
  /// Opens the game list from settings.
  /// @param remove_invalid Whether or not to
  /// remove paths from the game list that no longer exist.
  /// @returns A new game list instance.
  static ScopedPtr<GameList> open(QSettings& settings, bool remove_invalid = true);
  /// Just a stub.
  virtual ~GameList() {}
  /// Gets game information from a certain index.
  /// @param index The index to get the game info from.
  /// @returns On success, a pointer to a game information instance.
  /// If the index is out of bounds, then a null pointer instead.
  virtual const GameInfo* at(std::size_t index) const = 0;
  /// Fills a table widget with the information.
  /// @param table_widget The table widget to fill.
  virtual void fill_table(QTableWidget* table_widget) = 0;
  /// Loads game information from a specified file.
  /// @param filename The name of the file to load.
  /// @returns True on success, false on failure.
  virtual bool open(const char* filename) = 0;
};

} // namespace tk

} // namespace herald
