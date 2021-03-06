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

#include "src/vulkan/frame_buffer.h"

#include <limits>

#include "src/make_unique.h"

namespace amber {
namespace vulkan {

FrameBuffer::FrameBuffer(VkDevice device, uint32_t width, uint32_t height)
    : device_(device), width_(width), height_(height) {}

FrameBuffer::~FrameBuffer() = default;

Result FrameBuffer::Initialize(
    VkRenderPass render_pass,
    VkFormat color_format,
    VkFormat depth_format,
    const VkPhysicalDeviceMemoryProperties& properties) {
  std::vector<VkImageView> attachments;

  if (color_format != VK_FORMAT_UNDEFINED) {
    color_image_ = MakeUnique<Image>(device_, color_format, width_, height_,
                                     depth_, properties);
    Result r = color_image_->Initialize(VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    if (!r.IsSuccess())
      return r;
    attachments.push_back(color_image_->GetVkImageView());
  }

  if (depth_format != VK_FORMAT_UNDEFINED) {
    depth_image_ = MakeUnique<Image>(device_, depth_format, width_, height_,
                                     depth_, properties);
    Result r =
        depth_image_->Initialize(VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    if (!r.IsSuccess())
      return r;
    attachments.push_back(depth_image_->GetVkImageView());
  }

  VkFramebufferCreateInfo frame_buffer_info = {};
  frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  frame_buffer_info.renderPass = render_pass;
  frame_buffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
  frame_buffer_info.pAttachments = attachments.data();
  frame_buffer_info.width = width_;
  frame_buffer_info.height = height_;
  frame_buffer_info.layers = 1;

  if (vkCreateFramebuffer(device_, &frame_buffer_info, nullptr, &frame_) !=
      VK_SUCCESS) {
    return Result("Vulkan::Calling vkCreateFramebuffer Fail");
  }

  return {};
}

void FrameBuffer::Shutdown() {
  vkDestroyFramebuffer(device_, frame_, nullptr);
  if (color_image_)
    color_image_->Shutdown();
  if (depth_image_)
    depth_image_->Shutdown();
}

}  // namespace vulkan
}  // namespace amber
