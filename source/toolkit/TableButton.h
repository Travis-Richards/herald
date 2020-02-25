#pragma once

#include <QObject>

#include <cstddef>

class QWidget;
class QPushButton;

namespace herald {

namespace tk {

/// Used to identify a table button.
class TableButtonID final {
  /// If this is a table defined
  /// by default.
  bool predefined;
  /// The ID of the table button.
  std::size_t id;
  /// Constructs a new table button ID.
  /// @param p Whether or not this is a predefined button.
  /// @param i The ID of the button.
  constexpr TableButtonID(bool p, std::size_t i) noexcept
    : predefined(p), id(i) {}
public:
  /// Constructs a new item-specific button.
  /// @param id The ID to assign the button.
  static TableButtonID make(std::size_t id) {
    return TableButtonID(false, id);
  }
  /// Constructs a predefined button.
  /// @param id The ID to assign the button.
  static TableButtonID make_predefined(std::size_t id) noexcept {
    return TableButtonID(true, id);
  }
  /// Indicates whether or not the button has a certain ID.
  /// @param other_id The ID to check for.
  /// @returns True if the IDs match, false otherwise.
  inline bool has_id(std::size_t other_id) const noexcept {
    return id == other_id;
  }
  /// Indicates whether or not this is a predefined button.
  /// @returns True if the button is predefined, false otherwise.
  inline bool is_predefined() const noexcept {
    return predefined;
  }
};

/// A wrapper around a QPushButton widget
/// so that when the button is clicked,
/// it's name is emitted with the signal.
class TableButton final : public QObject {
  Q_OBJECT
public:
  /// Constructs a new instance of the table button.
  /// @param id The ID of the button to make.
  /// @param name The name to give the button.
  /// @param parent A pointer to the parent widget.
  TableButton(const TableButtonID& id, const QString& name, QWidget* parent);
  /// Accesses the widget containing the button.
  /// @returns A pointer to the button widget.
  QWidget* get_widget() noexcept;
signals:
  /// Emitted when the button is clicked.
  /// @param button_id The ID of the button that was clicked.
  void clicked(const TableButtonID& button_id);
protected slots:
  /// Handles the button getting clicked.
  void handle_clicked(bool);
private:
  /// The actual button instance.
  QPushButton* button;
  /// The ID of the button.
  TableButtonID id;
};

} // namespace tk

} // namespace herald
