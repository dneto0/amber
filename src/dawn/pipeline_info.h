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

#ifndef SRC_DAWN_PIPELINE_INFO_H_
#define SRC_DAWN_PIPELINE_INFO_H_

#include "amber/result.h"
#include "dawn/dawncpp.h"

namespace amber {
namespace dawn {

class RenderPipelineInfo {
 public:
  RenderPipelineInfo() {}
  RenderPipelineInfo(::dawn::ShaderModule vert, ::dawn::ShaderModule frag)
      : vertex_shader_(vert), fragment_shader_(frag) {}

  // Returns true if this render pipeline is configured at all.
  operator bool() { return vertex_shader_ && fragment_shader_; }

 private:
  ::dawn::ShaderModule vertex_shader_;
  ::dawn::ShaderModule fragment_shader_;
  // TODO(dneto): Create a default framebuffer colour target when needed.
  // TODO(dneto): Record stencil info?
  // TODO(dneto): Record index data
  // TODO(dneto): Record buffer data
};

class ComputePipelineInfo {
 public:
  ComputePipelineInfo() {}
  explicit ComputePipelineInfo(::dawn::ShaderModule comp)
      : compute_shader_(comp) {}

  // Returns true if this render pipeline is configured at all.
  operator bool() { return bool(compute_shader_); }

 private:
  ::dawn::ShaderModule compute_shader_;
};

}  // namespace dawn
}  // namespace amber

#endif  // SRC_DAWN_PIPELINE_INFO_H_
