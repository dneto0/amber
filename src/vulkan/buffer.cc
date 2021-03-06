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

#include "src/vulkan/buffer.h"

namespace amber {
namespace vulkan {

Buffer::Buffer(VkDevice device,
               size_t size,
               const VkPhysicalDeviceMemoryProperties& properties)
    : Resource(device, size, properties) {}

Buffer::~Buffer() = default;

Result Buffer::Initialize(const VkBufferUsageFlags usage) {
  Result r = CreateVkBuffer(&buffer_, usage);
  if (!r.IsSuccess())
    return r;

  AllocateResult allocate_result = AllocateAndBindMemoryToVkBuffer(
      buffer_, &memory_, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, false);
  if (!allocate_result.r.IsSuccess())
    return allocate_result.r;

  if (CheckMemoryHostAccessible(allocate_result.memory_type_index)) {
    is_buffer_host_accessible_ = true;
    return MapMemory(memory_);
  }

  is_buffer_host_accessible_ = false;
  return Resource::Initialize();
}

Result Buffer::CreateVkBufferView(VkFormat format) {
  VkBufferViewCreateInfo buffer_view_info = {};
  buffer_view_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
  buffer_view_info.buffer = buffer_;
  buffer_view_info.format = format;
  buffer_view_info.offset = 0;
  buffer_view_info.range = VK_WHOLE_SIZE;
  if (vkCreateBufferView(GetDevice(), &buffer_view_info, nullptr, &view_) !=
      VK_SUCCESS) {
    return Result("Vulkan::Calling vkCreateBufferView Fail");
  }

  return {};
}

void Buffer::CopyToDevice(VkCommandBuffer command) {
  if (is_buffer_host_accessible_)
    return;

  VkBufferCopy region = {};
  region.srcOffset = 0;
  region.dstOffset = 0;
  region.size = GetSize();

  vkCmdCopyBuffer(command, GetHostAccessibleBuffer(), buffer_, 1, &region);
}

void Buffer::Shutdown() {
  // TODO(jaebaek): Doublecheck what happens if |view_| is VK_NULL_HANDLE on
  //                Android and Windows.
  if (view_ != VK_NULL_HANDLE) {
    vkDestroyBufferView(GetDevice(), view_, nullptr);
    view_ = VK_NULL_HANDLE;
  }

  if (buffer_ != VK_NULL_HANDLE) {
    vkDestroyBuffer(GetDevice(), buffer_, nullptr);
    buffer_ = VK_NULL_HANDLE;
  }

  if (memory_ != VK_NULL_HANDLE) {
    vkFreeMemory(GetDevice(), memory_, nullptr);
    memory_ = VK_NULL_HANDLE;
  }
}

}  // namespace vulkan
}  // namespace amber
