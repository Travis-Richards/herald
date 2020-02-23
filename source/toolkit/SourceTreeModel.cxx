#include "SourceTreeModel.h"

#include <herald/ScopedPtr.h>

namespace herald {

namespace tk {

namespace {

/// The implementation of the source tree model.
class SourceTreeModelImpl final : public SourceTreeModel {
public:
  int columnCount(const QModelIndex& parent) const override {
    (void)parent;
    return 2;
  }
  QVariant data(const QModelIndex& index, int role) const override {
    (void)index;
    (void)role;
    return QVariant();
  }
  QModelIndex index(int row, int column, const QModelIndex& parent) const override {
    (void)row;
    (void)column;
    (void)parent;
    return QModelIndex();
  }
  QModelIndex parent(const QModelIndex& index) const override {
    (void)index;
    return QModelIndex();
  }
  int rowCount(const QModelIndex& index) const override {
    (void)index;
    return 0;
  }
};

} // namespace

ScopedPtr<SourceTreeModel> SourceTreeModel::make() {
  return new SourceTreeModelImpl();
}

} // namespace tk

} // namespace herald
