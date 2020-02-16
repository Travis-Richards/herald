#include <QApplication>

#include "Manager.h"
#include "ScopedPtr.h"
#include "StartupDialog.h"

int main(int argc, char** argv) {

  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald Toolkit");

  auto manager = herald::tk::Manager::make();

  auto startup_dialog = herald::tk::StartupDialog::make(manager.get());

  startup_dialog->show();

  return app.exec();
}
