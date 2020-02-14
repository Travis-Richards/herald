#include "QtEngine.h"

#include "ScopedPtr.h"
#include "QtModel.h"
#include "QtTarget.h"

#include <QGraphicsView>

#include <utility>

namespace herald {

namespace {

/// An implementation of the Qt engine.
class QtEngineImpl final : public QtEngine {
  /// The taregt to render to.
  ScopedPtr<QtTarget> target;
  /// The Qt data model.
  ScopedPtr<QtModel> model;
public:
  /// Constructs an instance of the Qt engine.
  /// @param t The target to render to.
  QtEngineImpl(ScopedPtr<QtTarget>&& t) : target(std::move(t)), model(QtModel::make()) {

    target->get_graphics_view()->setScene(model->get_scene());

    model->resize(target->get_graphics_view()->size());

    target->connect_model(model.get());
  }
  /// Accesses a pointer to the model.
  /// @returns A pointer to the model.
  Model* get_model() override {
    return model.get();
  }
  /// Moves the game forward in time.
  void advance(std::size_t value) override {
    model->advance(value);
  }
};

} // namespace

ScopedPtr<QtEngine> QtEngine::make(ScopedPtr<QtTarget>&& target) {
  return new QtEngineImpl(std::move(target));
}

} // namespace herald
