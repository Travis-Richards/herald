#pragma once

class QString;

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

class ProcessQueue;
class SourceManager;

/// The base of any supported language.
class Language {
public:
  /// Creates a null language instance.
  /// @returns A null language instance.
  static ScopedPtr<Language> make_null();
  /// Creates a language based on the API name.
  /// @param name The name of the language to make.
  static ScopedPtr<Language> make_from_name(const QString& name);
  /// Just a stub.
  virtual ~Language() {}
  /// Builds the sources from the source tree.
  /// @param process_queue The queue to add build jobs to.
  /// @param source_manager The source manager containing the source tree.
  /// @returns True on success, false on failure.
  virtual bool build(ProcessQueue& process_queue,
                     SourceManager& source_manager) = 0;
  /// The default extension used when creating files.
  /// @returns The extension used when creating files.
  virtual QString default_extension() = 0;
  /// Whether or not language support is detected on the system.
  /// @returns True if it was detected, false otherwise.
  virtual bool found_on_system() = 0;
};

} // namespace tk

} // namespace herald
