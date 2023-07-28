#include "example.hpp"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#include <stdexcept>

typedef struct
{
    float position[2];
    float color[4];
} MyVertex;

static const MyVertex vertices[] =
{
    { {  -0.5, -0.5 }, { 1, 0, 0, 1 } },
    { {  +0.5, -0.5 }, { 0, 1, 0, 1 } },
    { {  +0.0, +0.5 }, { 0, 0, 1, 1 } },
};

void example::metal()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(250, 250, "Metal Example", nullptr, nullptr);
    if (window == nullptr) std::runtime_error("CRASH: Window creation failed!");

    id <MTLDevice> device = MTLCreateSystemDefaultDevice();
    id <MTLCommandQueue> commandQueue = [device newCommandQueue];

    NSWindow *nWindow = glfwGetCocoaWindow(window);
    CAMetalLayer *metalLayer = [CAMetalLayer layer];
    metalLayer.device = device;
    metalLayer.pixelFormat = MTLPixelFormatRGBA8Unorm;
    metalLayer.framebufferOnly = YES;
    nWindow.contentView.layer = metalLayer;
    nWindow.contentView.wantsLayer = YES;

    id <MTLBuffer> vertexBuffer;
    NSUInteger dataSize = sizeof(vertices);
    vertexBuffer = [device newBufferWithBytes: vertices length: dataSize options: MTLResourceStorageModeShared];

    id <MTLRenderPipelineState> pipelineState;

    id defaultLibrary = [device newDefaultLibrary];
    id vertexProgram = [defaultLibrary newFunctionWithName: @"basic_vertex"];
    id fragmentProgram = [defaultLibrary newFunctionWithName: @"basic_fragment"];

    MTLVertexDescriptor *vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];

    // POSITION
    vertexDescriptor.attributes[0].offset = 0;
    vertexDescriptor.attributes[0].format = MTLVertexFormatFloat2;
    vertexDescriptor.attributes[0].bufferIndex = 0;

    // COLOR
    vertexDescriptor.attributes[1].offset = sizeof MyVertex().position;
    vertexDescriptor.attributes[1].format = MTLVertexFormatFloat4;
    vertexDescriptor.attributes[1].bufferIndex = 0;

    // LAYOUT
    vertexDescriptor.layouts[0].stepRate = 1;
    vertexDescriptor.layouts[0].stride = sizeof(MyVertex);
    vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
    pipelineStateDescriptor.vertexFunction = vertexProgram;
    pipelineStateDescriptor.fragmentFunction = fragmentProgram;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
    pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;
    [pipelineStateDescriptor setVertexDescriptor: vertexDescriptor];

    pipelineState = [device newRenderPipelineStateWithDescriptor: pipelineStateDescriptor error: nil];


    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor new];

    while (!glfwWindowShouldClose(window))
    {
        @autoreleasepool
        {
            glfwPollEvents();

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            metalLayer.drawableSize = CGSizeMake(width, height);
            id<CAMetalDrawable> drawable = [metalLayer nextDrawable];

            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
            renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
            renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
            renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;


            id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];

            [renderEncoder setRenderPipelineState: pipelineState];
            [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
            [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: 3];

            [renderEncoder pushDebugGroup:@"Debug Group"];

            [renderEncoder popDebugGroup];
            [renderEncoder endEncoding];

            [commandBuffer presentDrawable:drawable];
            [commandBuffer commit];
        }
    }
}
