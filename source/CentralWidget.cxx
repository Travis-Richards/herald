#include "CentralWidget.h"

#include "ControlPanel.h"

#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent) {

  control_panel = new ControlPanel(this);

  layout = new QVBoxLayout(this);
  layout->addWidget(control_panel);

  setLayout(layout);

  connect(control_panel, &ControlPanel::delete_requested, this, &CentralWidget::delete_requested);
  connect(control_panel, &ControlPanel::game_selected, this, &CentralWidget::game_selected);
  connect(control_panel, &ControlPanel::open_requested, this, &CentralWidget::open_requested);
  connect(control_panel, &ControlPanel::play_requested, this, &CentralWidget::play_requested);
  connect(control_panel, &ControlPanel::settings_requested, this, &CentralWidget::settings_requested);
}

void CentralWidget::update_game_list(const GameList& game_list) {
  control_panel->update_game_list(game_list);
}
