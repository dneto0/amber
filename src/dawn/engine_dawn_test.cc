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
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/dawn/engine_dawn.h"

#include "gtest/gtest.h"

namespace amber {
namespace dawn {

namespace {

using EngineDawnTest = testing::Test;

TEST_F(EngineDawnTest, EntryPointMapEmptyByDefault) {
  EngineDawn e;
  EXPECT_TRUE(e.GetEntryPointMapForTest().empty());
}

TEST_F(EngineDawnTest, EntryPointMapYieldsEmptyStringForUnsetType) {
  EngineDawn e;
  // Need to copy the map because it's const.
  auto map_copy = e.GetEntryPointMapForTest();
  EXPECT_EQ(std::string(), map_copy[ShaderType::kFragment]);
}

TEST_F(EngineDawnTest, DoEntryPointSavesStageName) {
  EngineDawn e;
  {
    EntryPointCommand cmd;
    cmd.SetShaderType(ShaderType::kFragment);
    cmd.SetEntryPointName("sass");
    e.DoEntryPoint(&cmd);
  }
  auto& map = e.GetEntryPointMapForTest();
  EXPECT_EQ(1u, map.size());
  auto where = map.find(ShaderType::kFragment);
  EXPECT_NE(map.end(), where);
  EXPECT_EQ(std::string("sass"), where->second);
}

TEST_F(EngineDawnTest, ShutdownDestroysInternalState) {
  EngineDawn e;
  // Add something to e
  {
    EntryPointCommand cmd;
    cmd.SetShaderType(ShaderType::kFragment);
    cmd.SetEntryPointName("sass");
    e.DoEntryPoint(&cmd);
  }
  auto& map = e.GetEntryPointMapForTest();
  EXPECT_EQ(1u, map.size());
  auto where = map.find(ShaderType::kFragment);
  EXPECT_NE(map.end(), where);
  EXPECT_EQ(std::string("sass"), where->second);

  // Now destroy it.
  e.Shutdown();
  EXPECT_TRUE(e.GetEntryPointMapForTest().empty());
}

}  // namespace
}  // namespace dawn
}  // namespace amber
