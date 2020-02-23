#pragma once

#include <QAbstractItemModel>

namespace herald {

template <typename T>
class ScopedPtr;

namespace tk {

/// Used for implementing an abstract item model
/// to be used for the source tree view. Tracks
/// source files and whether or not they have been modified.
class SourceTreeModel : public QAbstractItemModel {
public:
  /// Creates a new instance of the model.
  /// @returns A pointer to a new source tree model.
  static ScopedPtr<SourceTreeModel> make();
  /// Just a stub.
  virtual ~SourceTreeModel() {}
};

} // namespace tk

} // namespace herald
