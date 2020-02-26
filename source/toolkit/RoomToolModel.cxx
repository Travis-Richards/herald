#include "RoomToolModel.h"

namespace herald {

namespace tk {

void RoomToolModel::set_tool(RoomToolID id) {
  if (id != tool_id) {
    tool_id = id;
    emit tool_changed(id);
  }
}

} // namespace tk

} // namespace herald
