#include "DefaultEngine.h"

#include "DefaultModel.h"
#include "Target.h"
#include "ScopedPtr.h"

namespace herald {

namespace {

/// An implementation of the CPU engine.
class DefaultEngineImpl final : public DefaultEngine {
  /// The target being rendered to.
  Target& target;
  /// The data model for the game.
  ScopedPtr<DefaultModel> model;
public:
  /// Constructs an instance of the CPU engine.
  /// @param t The target to render to.
  DefaultEngineImpl(Target& t) : target(t), model(DefaultModel::make()) { }
  /// Accesses a pointer to the data model.
  Model* get_model() override {
    return model.get();
  }
  /// Moves the game forward in time.
  void advance(std::size_t) override {
  }
};

} // namespace

ScopedPtr<DefaultEngine> DefaultEngine::make(Target& target) {
  return new DefaultEngineImpl(target);
}

} // namespace herald
