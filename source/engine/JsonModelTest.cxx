#include <gtest/gtest.h>

#include <herald/JsonModel.h>
#include <herald/ScopedPtr.h>

namespace {

const char* source = R"(
{
  "textures" : [
    {
      "path" : "texture_01.png",
      "name" : "Texture 1"
    },
    {
      "path" : "texture_02.png",
      "name" : "Texture 2"
    }
  ]
}
)";

} // namespace

TEST(JsonModel, Read) {

  auto model = herald::JsonModel::from_source(source);
  ASSERT_EQ(model, true);

  auto* texture_table = model->get_texture_table();
  ASSERT_EQ(!!texture_table, true);
}
