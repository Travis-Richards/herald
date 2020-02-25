#include <QApplication>

#include <herald/ScopedPtr.h>

#include "Manager.h"
#include "StartupDialog.h"

int main(int argc, char** argv) {

  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald Toolkit");

  Q_INIT_RESOURCE(icons);

  auto manager = herald::tk::Manager::make();

  auto startup_dialog = herald::tk::StartupDialog::make(manager.get());

  startup_dialog->show();

  return app.exec();
}
