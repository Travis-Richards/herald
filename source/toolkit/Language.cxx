#include "Language.h"

#include <herald/ScopedPtr.h>

#include "JavaLanguage.h"
#include "PythonLanguage.h"

#include <QString>

namespace herald {

namespace tk {

namespace {

/// A null language derived class.
/// Used as a placeholder.
class NullLanguage final : public Language {
public:
  /// Does nothing.
  /// @returns Always returns true.
  bool build(ProcessQueue&, SourceTreeModel&) override {
    return true;
  }
  /// Does nothing.
  /// @returns Always returns true.
  bool found_on_system() override {
    return true;
  }
  /// Gets the default extension for a null language.
  /// @returns An extension for a text file.
  QString default_extension() override {
    return ".txt";
  }
};

} // namespace

ScopedPtr<Language> Language::make_null() {
  return new NullLanguage;
}

ScopedPtr<Language> Language::make_from_name(const QString& name) {
  if (name == "Java") {
    return make_java_language();
  } else if (name == "Python") {
    return make_python_language();
  } else {
    return Language::make_null();
  }
}

} // namespace tk

} // namespace herald
