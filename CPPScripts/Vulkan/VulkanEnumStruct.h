#pragma once
#include "../pubh.h"
// 用GLFW的话这里就不要自己去include Vulkan的头文件，用这个宏定义，让GLFW自己去处理，不然有些接口有问题
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vk_mem_alloc.h"

#define ShaderModuleSet map<VkShaderStageFlagBits, VkShaderModule>

namespace ZXEngine
{
    // 在GPU渲染画面的时候，CPU可以处理的帧数
    const uint32_t MAX_FRAMES_IN_FLIGHT = 1;

    // 需要的验证层
    const vector<const char*> validationLayers =
    {
        // Vulkan SDK通过请求VK_LAYER_KHRONOS_validation层，来隐式的开启有所关于诊断的layers，从而避免明确的指定所有的明确的诊断层
        "VK_LAYER_KHRONOS_validation"
    };

    // 需要用到的扩展
    const vector<const char*> deviceExtensions =
    {
        // 交换链扩展名，这个的支持也就代表了是否支持将图像绘制到显示器上(不是所有GPU都可以拿来绘图)
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    enum class RenderPassType
    {
        Present, // For swap chain
        Normal,
        Color,
        ShadowCubeMap,
        MAX,
    };

    struct QueueFamilyIndices
    {
        uint32_t present = UINT32_MAX;
        uint32_t graphics = UINT32_MAX;

        bool isComplete() { return present != UINT32_MAX && graphics != UINT32_MAX; }
    };

    // 交换链的三大类属性设置
    struct SwapChainSupportDetails
    {
        // 基本的surface功能属性(min/max number of images in swap chain, min/max width and height of images)
        VkSurfaceCapabilitiesKHR capabilities = {};
        // Surface格式(pixel format, color space)
        vector<VkSurfaceFormatKHR> formats;
        // 有效的presentation模式
        vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanBuffer
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    struct UniformBuffer
    {
        uint32_t binding = 0;
        VkDeviceSize size = 0;
        VulkanBuffer buffer;
        void* mappedAddress;
    };

    struct VulkanImage
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    struct VulkanTexture
    {
        VulkanImage image;
        VkImageView imageView = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        bool inUse = false;
    };

    struct VulkanFBO
    {
        vector<VkFramebuffer> frameBuffers;
        uint32_t colorAttachmentIdx = UINT32_MAX;
        uint32_t depthAttachmentIdx = UINT32_MAX;
        FrameBufferType bufferType = FrameBufferType::Normal;
        RenderPassType renderPassType = RenderPassType::Normal;
        ClearInfo clearInfo = {};
        bool inUse = false;
    };

    struct VulkanAttachmentBuffer
    {
        vector<uint32_t> attachmentBuffers;
        bool inUse = false;
    };

    struct VulkanDrawIndex
    {
        uint32_t VAO = 0;
        uint32_t pipelineID = 0;
        uint32_t materialDataID = 0;
    };

    struct VulkanCommand
    {
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        vector<VkSemaphore> signalSemaphores;
    };

    struct VulkanDrawCommand
    {
        CommandType commandType = CommandType::NotCare;
        vector<VulkanCommand> drawCommands;
        bool inUse = false;
    };

    struct VulkanVAO
    {
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VmaAllocation indexBufferAlloc = VK_NULL_HANDLE;
        void* indexBufferAddress = nullptr; // Only for dynamic mesh
        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation vertexBufferAlloc = VK_NULL_HANDLE;
        void* vertexBufferAddress = nullptr; // Only for dynamic mesh
        uint32_t size = 0;
        bool inUse = false;
    };

    struct VulkanPipeline
    {
        string name; // For debug
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
        bool inUse = false;
    };

    struct VulkanMaterialData
    {
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        vector<VkDescriptorSet> descriptorSets;
        vector<UniformBuffer> vertUniformBuffers;
        vector<UniformBuffer> geomUniformBuffers;
        vector<UniformBuffer> fragUniformBuffers;
        bool inUse = false;
    };
}