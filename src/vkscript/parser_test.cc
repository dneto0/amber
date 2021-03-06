// Copyright 2018 The Amber Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or parseried.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/vkscript/parser.h"
#include "gtest/gtest.h"
#include "src/feature.h"
#include "src/format.h"
#include "src/vkscript/nodes.h"

namespace amber {
namespace vkscript {

using VkScriptParserTest = testing::Test;

TEST_F(VkScriptParserTest, EmptyRequireBlock) {
  std::string block = "";

  Parser parser;
  Result r = parser.ProcessRequireBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess());

  auto script = parser.GetScript();
  ASSERT_TRUE(script->IsVkScript());
  EXPECT_TRUE(ToVkScript(script)->Nodes().empty());
}

TEST_F(VkScriptParserTest, RequireBlockNoArgumentFeatures) {
  struct {
    const char* name;
    Feature feature;
  } features[] = {
      {"robustBufferAccess", Feature::kRobustBufferAccess},
      {"fullDrawIndexUint32", Feature::kFullDrawIndexUint32},
      {"imageCubeArray", Feature::kImageCubeArray},
      {"independentBlend", Feature::kIndependentBlend},
      {"geometryShader", Feature::kGeometryShader},
      {"tessellationShader", Feature::kTessellationShader},
      {"sampleRateShading", Feature::kSampleRateShading},
      {"dualSrcBlend", Feature::kDualSrcBlend},
      {"logicOp", Feature::kLogicOp},
      {"multiDrawIndirect", Feature::kMultiDrawIndirect},
      {"drawIndirectFirstInstance", Feature::kDrawIndirectFirstInstance},
      {"depthClamp", Feature::kDepthClamp},
      {"depthBiasClamp", Feature::kDepthBiasClamp},
      {"fillModeNonSolid", Feature::kFillModeNonSolid},
      {"depthBounds", Feature::kDepthBounds},
      {"wideLines", Feature::kWideLines},
      {"largePoints", Feature::kLargePoints},
      {"alphaToOne", Feature::kAlphaToOne},
      {"multiViewport", Feature::kMultiViewport},
      {"samplerAnisotropy", Feature::kSamplerAnisotropy},
      {"textureCompressionETC2", Feature::kTextureCompressionETC2},
      {"textureCompressionASTC_LDR", Feature::kTextureCompressionASTC_LDR},
      {"textureCompressionBC", Feature::kTextureCompressionBC},
      {"occlusionQueryPrecise", Feature::kOcclusionQueryPrecise},
      {"pipelineStatisticsQuery", Feature::kPipelineStatisticsQuery},
      {"vertexPipelineStoresAndAtomics",
       Feature::kVertexPipelineStoresAndAtomics},
      {"fragmentStoresAndAtomics", Feature::kFragmentStoresAndAtomics},
      {"shaderTessellationAndGeometryPointSize",
       Feature::kShaderTessellationAndGeometryPointSize},
      {"shaderImageGatherExtended", Feature::kShaderImageGatherExtended},
      {"shaderStorageImageExtendedFormats",
       Feature::kShaderStorageImageExtendedFormats},
      {"shaderStorageImageMultisample",
       Feature::kShaderStorageImageMultisample},
      {"shaderStorageImageReadWithoutFormat",
       Feature::kShaderStorageImageReadWithoutFormat},
      {"shaderStorageImageWriteWithoutFormat",
       Feature::kShaderStorageImageWriteWithoutFormat},
      {"shaderUniformBufferArrayDynamicIndexing",
       Feature::kShaderUniformBufferArrayDynamicIndexing},
      {"shaderSampledImageArrayDynamicIndexing",
       Feature::kShaderSampledImageArrayDynamicIndexing},
      {"shaderStorageBufferArrayDynamicIndexing",
       Feature::kShaderStorageBufferArrayDynamicIndexing},
      {"shaderStorageImageArrayDynamicIndexing",
       Feature::kShaderStorageImageArrayDynamicIndexing},
      {"shaderClipDistance", Feature::kShaderClipDistance},
      {"shaderCullDistance", Feature::kShaderCullDistance},
      {"shaderFloat64", Feature::kShaderFloat64},
      {"shaderInt64", Feature::kShaderInt64},
      {"shaderInt16", Feature::kShaderInt16},
      {"shaderResourceResidency", Feature::kShaderResourceResidency},
      {"shaderResourceMinLod", Feature::kShaderResourceMinLod},
      {"sparseBinding", Feature::kSparseBinding},
      {"sparseResidencyBuffer", Feature::kSparseResidencyBuffer},
      {"sparseResidencyImage2D", Feature::kSparseResidencyImage2D},
      {"sparseResidencyImage3D", Feature::kSparseResidencyImage3D},
      {"sparseResidency2Samples", Feature::kSparseResidency2Samples},
      {"sparseResidency4Samples", Feature::kSparseResidency4Samples},
      {"sparseResidency8Samples", Feature::kSparseResidency8Samples},
      {"sparseResidency16Samples", Feature::kSparseResidency16Samples},
      {"sparseResidencyAliased", Feature::kSparseResidencyAliased},
      {"variableMultisampleRate", Feature::kVariableMultisampleRate},
      {"inheritedQueries", Feature::kInheritedQueries},
  };

  for (const auto& feature : features) {
    Parser parser;
    Result r = parser.ProcessRequireBlockForTesting(feature.name);
    ASSERT_TRUE(r.IsSuccess()) << r.Error();

    auto& nodes = ToVkScript(parser.GetScript())->Nodes();
    ASSERT_EQ(1U, nodes.size());
    ASSERT_TRUE(nodes[0]->IsRequire());

    auto req = nodes[0]->AsRequire();
    ASSERT_EQ(1U, req->Requirements().size());
    EXPECT_EQ(feature.feature, req->Requirements()[0].GetFeature());
  }
}

TEST_F(VkScriptParserTest, RequireBlockExtensions) {
  std::string block = R"(VK_KHR_storage_buffer_storage_class
VK_KHR_variable_pointers)";

  Parser parser;
  Result r = parser.ProcessRequireBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsRequire());

  auto req = nodes[0]->AsRequire();
  ASSERT_EQ(2U, req->Extensions().size());

  const auto& exts = req->Extensions();
  EXPECT_EQ("VK_KHR_storage_buffer_storage_class", exts[0]);
  EXPECT_EQ("VK_KHR_variable_pointers", exts[1]);
}

TEST_F(VkScriptParserTest, RequireBlockFramebuffer) {
  std::string block = "framebuffer R32G32B32A32_SFLOAT";

  Parser parser;
  Result r = parser.ProcessRequireBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess());

  auto script = ToVkScript(parser.GetScript());
  EXPECT_EQ(1U, script->Nodes().size());

  auto& nodes = script->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsRequire());

  auto* req = nodes[0]->AsRequire();
  ASSERT_EQ(1U, req->Requirements().size());
  EXPECT_EQ(Feature::kFramebuffer, req->Requirements()[0].GetFeature());

  auto format = req->Requirements()[0].GetFormat();
  EXPECT_EQ(FormatType::kR32G32B32A32_SFLOAT, format->GetFormatType());
}

TEST_F(VkScriptParserTest, RequireBlockDepthStencil) {
  std::string block = "depthstencil D24_UNORM_S8_UINT";

  Parser parser;
  Result r = parser.ProcessRequireBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto script = ToVkScript(parser.GetScript());
  EXPECT_EQ(1U, script->Nodes().size());

  auto& nodes = script->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsRequire());

  auto* req = nodes[0]->AsRequire();
  ASSERT_EQ(1U, req->Requirements().size());
  EXPECT_EQ(Feature::kDepthStencil, req->Requirements()[0].GetFeature());

  auto format = req->Requirements()[0].GetFormat();
  EXPECT_EQ(FormatType::kD24_UNORM_S8_UINT, format->GetFormatType());
}

TEST_F(VkScriptParserTest, RequireBlockMultipleLines) {
  std::string block = R"(
# Requirements block stuff.
depthstencil D24_UNORM_S8_UINT
sparseResidency4Samples
framebuffer R32G32B32A32_SFLOAT
# More comments
inheritedQueries # line comment
)";

  Parser parser;
  Result r = parser.ProcessRequireBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto script = ToVkScript(parser.GetScript());
  EXPECT_EQ(1U, script->Nodes().size());

  auto& nodes = script->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsRequire());

  auto* req = nodes[0]->AsRequire();
  ASSERT_EQ(4U, req->Requirements().size());
  EXPECT_EQ(Feature::kDepthStencil, req->Requirements()[0].GetFeature());
  auto format = req->Requirements()[0].GetFormat();
  EXPECT_EQ(FormatType::kD24_UNORM_S8_UINT, format->GetFormatType());

  EXPECT_EQ(Feature::kSparseResidency4Samples,
            req->Requirements()[1].GetFeature());

  EXPECT_EQ(Feature::kFramebuffer, req->Requirements()[2].GetFeature());
  format = req->Requirements()[2].GetFormat();
  EXPECT_EQ(FormatType::kR32G32B32A32_SFLOAT, format->GetFormatType());

  EXPECT_EQ(Feature::kInheritedQueries, req->Requirements()[3].GetFeature());
}

TEST_F(VkScriptParserTest, IndicesBlock) {
  std::string block = "1 2 3";

  Parser parser;
  Result r = parser.ProcessIndicesBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsIndices());

  auto& indices = nodes[0]->AsIndices()->Indices();
  ASSERT_EQ(3U, indices.size());

  EXPECT_EQ(1, indices[0]);
  EXPECT_EQ(2, indices[1]);
  EXPECT_EQ(3, indices[2]);
}

TEST_F(VkScriptParserTest, IndicesBlockMultipleLines) {
  std::string block = R"(
# comment line
1 2 3   4 5 6
# another comment
7 8 9  10 11 12
)";

  std::vector<uint16_t> results = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  Parser parser;
  Result r = parser.ProcessIndicesBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsIndices());

  auto& indices = nodes[0]->AsIndices()->Indices();
  ASSERT_EQ(results.size(), indices.size());
  for (size_t i = 0; i < results.size(); ++i) {
    EXPECT_EQ(results[i], indices[i]);
  }
}

TEST_F(VkScriptParserTest, IndicesBlockBadValue) {
  std::string block = "1 a 3";

  Parser parser;
  Result r = parser.ProcessIndicesBlockForTesting(block);
  ASSERT_FALSE(r.IsSuccess());
  EXPECT_EQ("Invalid value in indices block", r.Error());
}

TEST_F(VkScriptParserTest, IndicesBlockValueTooLarge) {
  std::string block = "100000000000 3";

  Parser parser;
  Result r = parser.ProcessIndicesBlockForTesting(block);
  ASSERT_FALSE(r.IsSuccess());
  EXPECT_EQ("Value too large in indices block", r.Error());
}

TEST_F(VkScriptParserTest, VertexDataEmpty) {
  std::string block = "\n#comment\n";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess());

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  EXPECT_TRUE(nodes.empty());
}

TEST_F(VkScriptParserTest, VertexDataHeaderFormatString) {
  std::string block = "0/R32G32_SFLOAT 1/A8B8G8R8_UNORM_PACK32";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsVertexData());

  auto* data = nodes[0]->AsVertexData();
  EXPECT_TRUE(data->GetRows().empty());

  auto& headers = data->GetHeaders();

  ASSERT_EQ(2U, headers.size());
  EXPECT_EQ(static_cast<size_t>(0U), headers[0].location);
  EXPECT_EQ(FormatType::kR32G32_SFLOAT, headers[0].format->GetFormatType());

  EXPECT_EQ(1U, headers[1].location);
  EXPECT_EQ(FormatType::kA8B8G8R8_UNORM_PACK32,
            headers[1].format->GetFormatType());
}

TEST_F(VkScriptParserTest, VertexDataHeaderGlslString) {
  std::string block = "0/float/vec2 1/int/vec3";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsVertexData());

  auto* data = nodes[0]->AsVertexData();
  EXPECT_TRUE(data->GetRows().empty());

  auto& headers = data->GetHeaders();

  ASSERT_EQ(2U, headers.size());
  EXPECT_EQ(static_cast<size_t>(0U), headers[0].location);
  EXPECT_EQ(FormatType::kR32G32_SFLOAT, headers[0].format->GetFormatType());

  auto& comps1 = headers[0].format->GetComponents();
  ASSERT_EQ(2U, comps1.size());
  EXPECT_EQ(FormatMode::kSFloat, comps1[0].mode);
  EXPECT_EQ(FormatMode::kSFloat, comps1[1].mode);

  EXPECT_EQ(1U, headers[1].location);
  EXPECT_EQ(FormatType::kR32G32B32_SINT, headers[1].format->GetFormatType());

  auto& comps2 = headers[1].format->GetComponents();
  ASSERT_EQ(3U, comps2.size());
  EXPECT_EQ(FormatMode::kSInt, comps2[0].mode);
  EXPECT_EQ(FormatMode::kSInt, comps2[1].mode);
  EXPECT_EQ(FormatMode::kSInt, comps2[2].mode);
}

TEST_F(VkScriptParserTest, TestBlock) {
  std::string block = R"(clear color 255 255 255 0
clear depth 10
clear stencil 2
clear)";

  Parser parser;
  Result r = parser.ProcessTestBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsTest());

  const auto& cmds = nodes[0]->AsTest()->GetCommands();
  ASSERT_EQ(4U, cmds.size());

  ASSERT_TRUE(cmds[0]->IsClearColor());
  auto* color_cmd = cmds[0]->AsClearColor();
  EXPECT_FLOAT_EQ(255.f, color_cmd->GetR());
  EXPECT_FLOAT_EQ(255.f, color_cmd->GetG());
  EXPECT_FLOAT_EQ(255.f, color_cmd->GetB());
  EXPECT_FLOAT_EQ(0.0f, color_cmd->GetA());

  ASSERT_TRUE(cmds[1]->IsClearDepth());
  EXPECT_EQ(10U, cmds[1]->AsClearDepth()->GetValue());

  ASSERT_TRUE(cmds[2]->IsClearStencil());
  EXPECT_FLOAT_EQ(2, cmds[2]->AsClearStencil()->GetValue());

  EXPECT_TRUE(cmds[3]->IsClear());
}

TEST_F(VkScriptParserTest, VertexDataRows) {
  std::string block = R"(
# Vertex data
0/R32G32B32_SFLOAT  1/R8G8B8_UNORM
-1    -1 0.25       255 0 0  # ending comment
# Another Row
0.25  -1 0.25       255 0 255
)";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsVertexData());

  auto* data = nodes[0]->AsVertexData();
  auto& headers = data->GetHeaders();
  ASSERT_EQ(2U, headers.size());

  // Rows is a vector of vector of cells
  auto& rows = data->GetRows();
  ASSERT_EQ(2U, rows.size());

  // A row is a vector of Cells
  const auto& row1 = rows[0];
  ASSERT_EQ(2U, row1.size());

  const auto& row1_cell1 = row1[0];
  ASSERT_EQ(3U, row1_cell1.size());

  ASSERT_TRUE(row1_cell1.GetValue(0).IsFloat());
  EXPECT_FLOAT_EQ(-1, row1_cell1.GetValue(0).AsFloat());
  ASSERT_TRUE(row1_cell1.GetValue(1).IsFloat());
  EXPECT_FLOAT_EQ(-1, row1_cell1.GetValue(1).AsFloat());
  ASSERT_TRUE(row1_cell1.GetValue(2).IsFloat());
  EXPECT_FLOAT_EQ(0.25, row1_cell1.GetValue(2).AsFloat());

  const auto& row1_cell2 = row1[1];
  ASSERT_EQ(3U, row1_cell2.size());

  ASSERT_TRUE(row1_cell2.GetValue(0).IsInteger());
  EXPECT_FLOAT_EQ(255, row1_cell2.GetValue(0).AsUint8());
  ASSERT_TRUE(row1_cell2.GetValue(1).IsInteger());
  EXPECT_FLOAT_EQ(0, row1_cell2.GetValue(1).AsUint8());
  ASSERT_TRUE(row1_cell2.GetValue(2).IsInteger());
  EXPECT_FLOAT_EQ(0, row1_cell2.GetValue(2).AsUint8());

  // Second row.
  const auto& row2 = rows[1];
  ASSERT_EQ(2U, row2.size());

  const auto& row2_cell1 = row2[0];
  ASSERT_EQ(3U, row2_cell1.size());

  ASSERT_TRUE(row2_cell1.GetValue(0).IsFloat());
  EXPECT_FLOAT_EQ(0.25, row2_cell1.GetValue(0).AsFloat());
  ASSERT_TRUE(row2_cell1.GetValue(1).IsFloat());
  EXPECT_FLOAT_EQ(-1, row2_cell1.GetValue(1).AsFloat());
  ASSERT_TRUE(row2_cell1.GetValue(2).IsFloat());
  EXPECT_FLOAT_EQ(0.25, row2_cell1.GetValue(2).AsFloat());

  const auto& row2_cell2 = row2[1];
  ASSERT_EQ(3U, row2_cell2.size());

  ASSERT_TRUE(row2_cell2.GetValue(0).IsInteger());
  EXPECT_FLOAT_EQ(255, row2_cell2.GetValue(0).AsUint8());
  ASSERT_TRUE(row2_cell2.GetValue(1).IsInteger());
  EXPECT_FLOAT_EQ(0, row2_cell2.GetValue(1).AsUint8());
  ASSERT_TRUE(row2_cell2.GetValue(2).IsInteger());
  EXPECT_FLOAT_EQ(255, row2_cell2.GetValue(2).AsUint8());
}

TEST_F(VkScriptParserTest, VertexDataShortRow) {
  std::string block = R"(
0/R32G32B32_SFLOAT  1/R8G8B8_UNORM
-1    -1 0.25       255 0 0
0.25  -1 0.25       255 0
)";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_FALSE(r.IsSuccess());
  EXPECT_EQ("Too few cells in given vertex data row", r.Error());
}

TEST_F(VkScriptParserTest, VertexDataIncorrectValue) {
  std::string block = R"(
0/R32G32B32_SFLOAT  1/R8G8B8_UNORM
-1    -1 0.25       255 StringValue 0
0.25  -1 0.25       255 0 0
)";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_FALSE(r.IsSuccess());
  EXPECT_EQ("Invalid vertex data value", r.Error());
}

TEST_F(VkScriptParserTest, VertexDataRowsWithHex) {
  std::string block = R"(
0/A8B8G8R8_UNORM_PACK32
0xff0000ff
0xffff0000
)";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_TRUE(r.IsSuccess()) << r.Error();

  auto& nodes = ToVkScript(parser.GetScript())->Nodes();
  ASSERT_EQ(1U, nodes.size());
  ASSERT_TRUE(nodes[0]->IsVertexData());

  auto* data = nodes[0]->AsVertexData();
  auto& headers = data->GetHeaders();
  ASSERT_EQ(1U, headers.size());

  auto& rows = data->GetRows();
  ASSERT_EQ(2U, rows.size());

  // Each row has 1 cell.
  auto& row1 = rows[0];
  ASSERT_EQ(1U, row1.size());

  auto& row1_cell1 = row1[0];
  ASSERT_EQ(1U, row1_cell1.size());

  ASSERT_TRUE(row1_cell1.GetValue(0).IsInteger());
  EXPECT_EQ(0xff0000ff, row1_cell1.GetValue(0).AsUint32());

  auto& row2 = rows[1];
  ASSERT_EQ(1U, row1.size());

  const auto& row2_cell1 = row2[0];
  ASSERT_EQ(1U, row2_cell1.size());

  ASSERT_TRUE(row2_cell1.GetValue(0).IsInteger());
  EXPECT_EQ(0xffff0000, row2_cell1.GetValue(0).AsUint32());
}

TEST_F(VkScriptParserTest, VertexDataRowsWithHexWrongColumn) {
  std::string block = R"(
0/R32G32B32_SFLOAT  1/R8G8B8_UNORM
-1    -1 0.25       0xffff0000
0.25  -1 0.25       255 0
)";

  Parser parser;
  Result r = parser.ProcessVertexDataBlockForTesting(block);
  ASSERT_FALSE(r.IsSuccess());
  EXPECT_EQ("Invalid vertex data value", r.Error());
}

}  // namespace vkscript
}  // namespace amber
