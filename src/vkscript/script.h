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

#ifndef SRC_VKSCRIPT_SCRIPT_H_
#define SRC_VKSCRIPT_SCRIPT_H_

#include <memory>
#include <vector>

#include "src/command.h"
#include "src/make_unique.h"
#include "src/script.h"
#include "src/vkscript/section_parser.h"

namespace amber {
namespace vkscript {

class Node;
class RequireNode;
class VertexDataNode;

class Script : public amber::Script {
 public:
  Script();
  ~Script() override;

  void AddRequireNode(std::unique_ptr<RequireNode> node);
  void AddShader(ShaderType, std::vector<uint32_t>);
  void AddIndices(const std::vector<uint16_t>& indices);
  void AddVertexData(std::unique_ptr<VertexDataNode> node);
  void SetTestCommands(std::vector<std::unique_ptr<Command>> commands);

  const std::vector<std::unique_ptr<Node>>& Nodes() const {
    return test_nodes_;
  }

 private:
  std::vector<std::unique_ptr<Node>> test_nodes_;
};

}  // namespace vkscript

const vkscript::Script* ToVkScript(const amber::Script* s);

}  // namespace amber

#endif  // SRC_VKSCRIPT_SCRIPT_H_
