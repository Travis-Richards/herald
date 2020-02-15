#include <QApplication>

#include "ScopedPtr.h"
#include "StartupDialog.h"

int main(int argc, char** argv) {

  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("Taylor Holberton");
  QCoreApplication::setApplicationName("Herald Toolkit");

  auto startup_dialog = herald::tk::StartupDialog::make();

  startup_dialog->show();

  return app.exec();
}
