#pragma once

#include <herald/Model.h>

namespace herald {

template <typename T>
class ScopedPtr;

/// A model that's read from a JSON file.
class JsonModel : public Model {
public:
  /// Opens up a model from a JSON file.
  /// @param filename The name of the file to open.
  /// @returns A pointer to the model on success, a null pointer on failure.
  static ScopedPtr<JsonModel> open(const char* filename);
  /// Reads a JSON model from a string.
  /// @param source The JSON source to read from.
  /// @returns A new JSON model instance.
  static ScopedPtr<JsonModel> from_source(const char* source);
  /// Just a stub.
  virtual ~JsonModel() {}
};

} // namespace herald
