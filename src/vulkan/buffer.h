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

#ifndef SRC_VULKAN_BUFFER_H_
#define SRC_VULKAN_BUFFER_H_

#include "amber/result.h"
#include "src/vulkan/resource.h"
#include "vulkan/vulkan.h"

namespace amber {
namespace vulkan {

class Buffer : public Resource {
 public:
  Buffer(VkDevice device,
         size_t size,
         const VkPhysicalDeviceMemoryProperties& properties);
  ~Buffer() override;

  Result Initialize(const VkBufferUsageFlags usage);
  VkBuffer GetVkBuffer() const { return buffer_; }
  Result CreateVkBufferView(VkFormat format);
  VkBufferView GetVkBufferView() const { return view_; }

  // TODO(jaebaek): Determine copy all or partial data
  void CopyToDevice(VkCommandBuffer command);

  // Resource
  VkDeviceMemory GetHostAccessMemory() const override {
    if (is_buffer_host_accessible_)
      return memory_;

    return Resource::GetHostAccessMemory();
  }

  void Shutdown() override;

 private:
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkBufferView view_ = VK_NULL_HANDLE;
  VkDeviceMemory memory_ = VK_NULL_HANDLE;
  bool is_buffer_host_accessible_ = false;
};

}  // namespace vulkan
}  // namespace amber

#endif  // SRC_VULKAN_BUFFER_H_
