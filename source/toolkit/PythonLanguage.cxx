#include "PythonLanguage.h"

#include "Language.h"
#include "ProcessQueue.h"
#include "SourceManager.h"

#include <herald/ScopedPtr.h>

#include <herald/sysinfo/ProgramFinder.h>

#include <QModelIndex>
#include <QProcess>
#include <QString>

namespace herald {

namespace tk {

namespace {

/// A Python derived @ref Language class.
class PythonLanguage final : public Language {
public:
  /// Builds the project.
  /// @param process_queue The queue to add the build process to.
  /// @param source_manager The source manager to retrieve the source path from.
  /// @returns True on success, false on failure.
  bool build(ProcessQueue& process_queue, SourceManager& source_manager) override {

    auto* process = process_queue.add();

    auto source_root = source_manager.path_of(source_manager.get_root());

    QStringList args;
    args << "-m";
    args << "py_compile";
    args << "__main__.py";

    process->setWorkingDirectory(source_root);
    process->setProgram(find_python("python"));
    process->setArguments(args);
    process->start();

    return true;
  }
  /// Gets the default extension for source files.
  /// @returns The default extension for source files.
  QString default_extension() override {
    return ".py";
  }
  /// Whether or not Java support
  /// was found on the system. This
  /// requires a JDK to be installed.
  bool found_on_system() override {
    return !find_python().isEmpty();
  }
protected:
  /// Attempts to find the python executable.
  /// @param fallback The fallback name to use if it isn't found.
  /// @returns Either the fallback string or the location of the python executable.
  QString find_python(QString fallback = "") {

    auto program_finder = sysinfo::ProgramFinder::make();

    program_finder->add_env_list("PATH");

    if (program_finder->find("python")) {
      return program_finder->get_last_result();
    } else {
      return to_executable_name(fallback);
    }
  }
  /// Converts a basename to an executable name.
  /// This only makes a difference on Windows.
  /// @param basename The basename to convert.
  /// @returns The executable filename.
  QString to_executable_name(const QString& basename) {
#ifdef _WIN32
    return basename + ".exe";
#else
    return basename;
#endif
  }
};

} // namespace

ScopedPtr<Language> make_python_language() {
  return new PythonLanguage;
}

} // namespace tk

} // namespace herald
