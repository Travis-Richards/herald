#include "QtKeyController.h"

#include <QKeyEvent>

namespace herald {

void QtKeyController::map(const QKeyEvent* event, bool state) {
  switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
      /* up */
      update_axis(0, state ? 1 : 0);
      break;
    case Qt::Key_Left:
    case Qt::Key_A:
      /* left */
      update_axis(state ? -1 : 0, 0);
      break;
    case Qt::Key_Down:
    case Qt::Key_S:
      /* down */
      update_axis(0, state ? -1 : 0);
      break;
    case Qt::Key_Right:
    case Qt::Key_D:
      /* right */
      update_axis(0, state ? 1 : 0);
      break;
    case Qt::Key_J:
      update_button(Button::A, state);
      break;
    case Qt::Key_K:
      update_button(Button::B, state);
      break;
    case Qt::Key_L:
      update_button(Button::X, state);
      break;
    case Qt::Key_Semicolon:
      update_button(Button::Y, state);
      break;
  }
}

} // namespace herald
