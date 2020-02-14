#include "DefaultBackground.h"

namespace herald {

namespace {

class DefaultBackgroundImpl final : public DefaultBackground {
public:
};

} // namespace

ScopedPtr<DefaultBackground> DefaultBackground::make() {
  return new DefaultBackgroundImpl();
}

} // namespace herald
