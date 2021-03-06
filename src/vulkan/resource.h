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

#ifndef SRC_VULKAN_RESOURCE_H_
#define SRC_VULKAN_RESOURCE_H_

#include <memory>

#include "amber/result.h"
#include "vulkan/vulkan.h"

namespace amber {
namespace vulkan {

class Resource {
 public:
  virtual ~Resource();

  virtual VkDeviceMemory GetHostAccessMemory() const {
    return host_accessible_memory_;
  }

  virtual void Shutdown();

  void* HostAccessibleMemoryPtr() const { return memory_ptr_; }

 protected:
  Resource(VkDevice device,
           size_t size,
           const VkPhysicalDeviceMemoryProperties& properties);
  Result Initialize();
  Result CreateVkBuffer(VkBuffer* buffer, VkBufferUsageFlags usage);

  VkDevice GetDevice() const { return device_; }
  VkBuffer GetHostAccessibleBuffer() const { return host_accessible_buffer_; }

  size_t GetSize() const { return size_; }

  struct AllocateResult {
    Result r;
    uint32_t memory_type_index;
  };

  AllocateResult AllocateAndBindMemoryToVkBuffer(VkBuffer buffer,
                                                 VkDeviceMemory* memory,
                                                 VkMemoryPropertyFlags flags,
                                                 bool force_flags);
  AllocateResult AllocateAndBindMemoryToVkImage(VkImage image,
                                                VkDeviceMemory* memory,
                                                VkMemoryPropertyFlags flags,
                                                bool force_flags);

  bool CheckMemoryHostAccessible(uint32_t memory_type_index) {
    return (physical_memory_properties_.memoryTypes[memory_type_index]
                .propertyFlags &
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ==
           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
  }

  Result MapMemory(VkDeviceMemory memory);
  void UnMapMemory(VkDeviceMemory memory);

 private:
  uint32_t ChooseMemory(uint32_t memory_type_bits,
                        VkMemoryPropertyFlags flags,
                        bool force_flags);
  Result AllocateMemory(VkDeviceMemory* memory,
                        VkDeviceSize size,
                        uint32_t memory_type_index);

  Result BindMemoryToVkBuffer(VkBuffer buffer, VkDeviceMemory memory);
  const VkMemoryRequirements GetVkBufferMemoryRequirements(
      VkBuffer buffer) const;

  Result BindMemoryToVkImage(VkImage image, VkDeviceMemory memory);
  const VkMemoryRequirements GetVkImageMemoryRequirements(VkImage image) const;

  VkDevice device_ = VK_NULL_HANDLE;
  size_t size_ = 0;
  VkPhysicalDeviceMemoryProperties physical_memory_properties_;

  VkBuffer host_accessible_buffer_ = VK_NULL_HANDLE;
  VkDeviceMemory host_accessible_memory_ = VK_NULL_HANDLE;
  void* memory_ptr_ = nullptr;
};

}  // namespace vulkan
}  // namespace amber

#endif  // SRC_VULKAN_RESOURCE_H_
