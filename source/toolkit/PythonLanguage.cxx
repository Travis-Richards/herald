#include "PythonLanguage.h"

#include "Language.h"

#include <herald/ScopedPtr.h>

#include <QString>

namespace herald {

namespace tk {

namespace {

/// A Python derived @ref Language class.
class PythonLanguage final : public Language {
public:
  /// Builds the project.
  /// @returns True on success, false on failure.
  bool build(SourceManager*) override {
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
    return true;
  }
};

} // namespace

ScopedPtr<Language> make_python_language() {
  return new PythonLanguage;
}

} // namespace tk

} // namespace herald
