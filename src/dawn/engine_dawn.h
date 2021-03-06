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

#ifndef SRC_DAWN_ENGINE_DAWN_H_
#define SRC_DAWN_ENGINE_DAWN_H_

#include "dawn/dawncpp.h"

#include "src/engine.h"

namespace amber {
namespace dawn {

class EngineDawn : public Engine {
 public:
  EngineDawn();
  ~EngineDawn() override;

  // Engine
  // Initialize with a default device.
  Result Initialize() override;
  // Initialize with a given device, specified as a pointer-to-::dawn::Device
  // disguised as a pointer-to-void.
  Result InitializeWithDevice(void* default_device) override;
  Result Shutdown() override;
  Result CreatePipeline(PipelineType) override;
  Result AddRequirement(Feature feature, const Format*) override;
  Result SetShader(ShaderType type, const std::vector<uint32_t>& data) override;
  Result SetBuffer(BufferType type,
                   uint8_t location,
                   const Format& format,
                   const std::vector<Value>& data) override;
  Result DoClearColor(const ClearColorCommand* cmd) override;
  Result DoClearStencil(const ClearStencilCommand* cmd) override;
  Result DoClearDepth(const ClearDepthCommand* cmd) override;
  Result DoClear(const ClearCommand* cmd) override;
  Result DoDrawRect(const DrawRectCommand* cmd) override;
  Result DoDrawArrays(const DrawArraysCommand* cmd) override;
  Result DoCompute(const ComputeCommand* cmd) override;
  Result DoEntryPoint(const EntryPointCommand* cmd) override;
  Result DoPatchParameterVertices(
      const PatchParameterVerticesCommand* cmd) override;
  Result DoProbe(const ProbeCommand* cmd) override;
  Result DoProbeSSBO(const ProbeSSBOCommand* cmd) override;
  Result DoBuffer(const BufferCommand* cmd) override;
  Result DoTolerance(const ToleranceCommand* cmd) override;

 private:
  ::dawn::Device device_;
};

}  // namespace dawn
}  // namespace amber

#endif  // SRC_DAWN_ENGINE_DAWN_H_
