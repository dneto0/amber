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

#ifndef SRC_VKSCRIPT_NODES_H_
#define SRC_VKSCRIPT_NODES_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "src/buffer.h"
#include "src/command.h"
#include "src/feature.h"
#include "src/format.h"
#include "src/tokenizer.h"
#include "src/vkscript/section_parser.h"

namespace amber {
namespace vkscript {

class RequireNode;

class Node {
 public:
  virtual ~Node();

  bool IsRequire() const { return node_type_ == NodeType::kRequire; }

  RequireNode* AsRequire();

 protected:
  explicit Node(NodeType type);

 private:
  NodeType node_type_;
};

class RequireNode : public Node {
 public:
  class Requirement {
   public:
    explicit Requirement(Feature feature);
    Requirement(Feature feature, std::unique_ptr<Format> format);
    Requirement(Requirement&&);
    ~Requirement();

    Feature GetFeature() const { return feature_; }
    const Format* GetFormat() const { return format_.get(); }

   private:
    Feature feature_;
    std::unique_ptr<Format> format_;
  };

  RequireNode();
  ~RequireNode() override;

  void AddRequirement(Feature feature, std::unique_ptr<Format> format);

  const std::vector<Requirement>& Requirements() const { return requirements_; }

 private:
  std::vector<Requirement> requirements_;
};

}  // namespace vkscript
}  // namespace amber

#endif  // SRC_VKSCRIPT_NODES_H_
