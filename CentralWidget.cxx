#include "CentralWidget.h"

#include "ControlPanel.h"
#include "PanicWidget.h"
#include "Scene.h"

#include <QGraphicsView>

namespace {

/// Contains the indices for
/// each of the sub widgets in
/// the stacked central widget.
class Indices final {
public:
  static constexpr int control_panel() noexcept {
    return 0;
  }
  static constexpr int graphics_view() noexcept {
    return 1;
  }
  static constexpr int panic_widget() noexcept {
    return 2;
  }
};

} // namespace

CentralWidget::CentralWidget(QWidget* parent) : QStackedWidget(parent) {

  panic_widget = new PanicWidget(this);

  control_panel = new ControlPanel(this);

  addWidget(control_panel);
  addWidget(panic_widget);

  connect(control_panel, &ControlPanel::open_requested, this, &CentralWidget::open_requested);
  connect(control_panel, &ControlPanel::play_requested, this, &CentralWidget::play_requested);
  connect(control_panel, &ControlPanel::game_selected, this, &CentralWidget::game_selected);
}

void CentralWidget::panic(const QString& message) {

  panic_widget->setText(message);

  setCurrentIndex(Indices::panic_widget());
}

void CentralWidget::update_game_list(const GameList& game_list) {
  control_panel->update_game_list(game_list);
}
