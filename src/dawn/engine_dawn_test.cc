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

TEST_F(EngineDawnTest, PendingWorkDefaultIsEmpty) {
  EngineDawn e;
  auto& pending = e.GetPendingWorkForTest();
  EXPECT_TRUE(pending.empty());
}

TEST_F(EngineDawnTest, DoClearAppendsToPendingWork) {
  EngineDawn e;
  {
    ClearCommand cc;
    e.DoClear(&cc);
  }
  auto& pending = e.GetPendingWorkForTest();
  EXPECT_EQ(1u, pending.size());
  EXPECT_TRUE(pending[0].IsClear());
}

TEST_F(EngineDawnTest, DoDrawRectAppendsToPendingWork) {
  EngineDawn e;
  {
    PipelineData pd;
    DrawRectCommand cmd(pd);
    e.DoDrawRect(&cmd);
  }
  auto& pending = e.GetPendingWorkForTest();
  ASSERT_EQ(1, pending.size());
  EXPECT_TRUE(pending[0].IsDrawRect());
}

TEST_F(EngineDawnTest, DoDrawArraysAppendsToPendingWork) {
  EngineDawn e;
  {
    PipelineData pd;
    DrawArraysCommand cmd(pd);
    e.DoDrawArrays(&cmd);
  }
  auto& pending = e.GetPendingWorkForTest();
  ASSERT_EQ(1, pending.size());
  EXPECT_TRUE(pending[0].IsDrawArrays());
}

TEST_F(EngineDawnTest, DoComputeAppendsToPendingWork) {
  EngineDawn e;
  {
    PipelineData pd;
    ComputeCommand cmd(pd);
    e.DoCompute(&cmd);
  }
  auto& pending = e.GetPendingWorkForTest();
  ASSERT_EQ(1, pending.size());
  EXPECT_TRUE(pending[0].IsCompute());
}

TEST_F(EngineDawnTest, MixedWorkloadFaithfullyRecorded) {
  EngineDawn e;
  {
    ClearCommand clear;
    PipelineData pd;
    ComputeCommand compute(pd);
    DrawRectCommand draw_rect(pd);
    DrawArraysCommand draw_arrays(pd);

    e.DoDrawArrays(&draw_arrays);
    e.DoCompute(&compute);
    e.DoCompute(&compute);
    e.DoDrawRect(&draw_rect);
  }
  auto& pending = e.GetPendingWorkForTest();
  ASSERT_EQ(4, pending.size());
  EXPECT_TRUE(pending[0].IsDrawArrays());
  EXPECT_TRUE(pending[1].IsCompute());
  EXPECT_TRUE(pending[2].IsCompute());
  EXPECT_TRUE(pending[3].IsDrawRect());
}
}  // namespace
}  // namespace dawn
}  // namespace amber
