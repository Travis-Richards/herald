#include "JavaLanguage.h"

#include "Language.h"

#include <herald/ScopedPtr.h>

#include <QString>

namespace herald {

namespace tk {

namespace {

/// A Java derived @ref Language class.
class JavaLanguage final : public Language {
public:
  /// Builds the project.
  /// @returns True on success, false on failure.
  bool build(SourceManager*) override {
    return true;
  }
  /// Gets the default extension for source files.
  /// @returns The default extension for source files.
  QString default_extension() override {
    return ".java";
  }
  /// Whether or not Java support
  /// was found on the system. This
  /// requires a JDK to be installed.
  bool found_on_system() override {
    return true;
  }
};

} // namespace

ScopedPtr<Language> make_java_language() {
  return new JavaLanguage;
}

} // namespace tk

} // namespace herald
