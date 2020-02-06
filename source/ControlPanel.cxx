#include "ControlPanel.h"

#include "GameListView.h"

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>

namespace {

/// Creates a new button for the control panel.
/// @param icon_path The path to the button icon.
/// @param label The text label to assign the button.
/// @param parent A pointer to the parent widget.
QPushButton* make_button(const QString& icon_path,
                         const QString& label,
                         QWidget* parent) {

  auto* button = new QPushButton(parent);
  button->setIcon(QIcon(icon_path));
  button->setText(label);
  return button;
}

} // namespace

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {

  game_list_view = new GameListView(parent);

  open_button = make_button(":/icons/open.svg", tr("Open"), this);

  refresh_button = make_button(":/icons/refresh.svg", tr("Refresh"), this);

  play_button = make_button(":/icons/play.svg", tr("Play"), this);

  pause_button = make_button(":/icons/pause.svg", tr("Pause"), this);

  save_button = make_button(":/icons/save.svg", tr("Save"), this);

  trash_button = make_button(":/icons/trash.svg", tr("Delete"), this);

  connect(open_button,    &QPushButton::clicked, this, &ControlPanel::open_requested);
  connect(refresh_button, &QPushButton::clicked, this, &ControlPanel::refresh_requested);
  connect(play_button,    &QPushButton::clicked, this, &ControlPanel::play_requested);
  connect(pause_button,   &QPushButton::clicked, this, &ControlPanel::pause_requested);
  connect(save_button,    &QPushButton::clicked, this, &ControlPanel::save_requested);
  connect(trash_button,   &QPushButton::clicked, this, &ControlPanel::delete_requested);

  connect(game_list_view, &GameListView::game_selected, this, &ControlPanel::game_selected);
  connect(game_list_view, &GameListView::game_play_requested, this, &ControlPanel::on_game_play_request);

  layout = new QGridLayout(this);
  layout->addWidget(open_button,    0, 0, 1, 1);
  layout->addWidget(refresh_button, 0, 1, 1, 1);
  layout->addWidget(play_button,    0, 2, 1, 1);
  layout->addWidget(pause_button,   0, 3, 1, 1);
  layout->addWidget(save_button,    0, 4, 1, 1);
  layout->addWidget(trash_button,   0, 5, 1, 1);
  layout->addWidget(game_list_view, 1, 0, 4, 6);

  setLayout(layout);
}

void ControlPanel::update_game_list(const GameList& game_list) {
  game_list_view->update(game_list);
}

void ControlPanel::on_game_play_request(int index) {
  emit game_selected(index);
  emit play_requested();
}
