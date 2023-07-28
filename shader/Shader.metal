#include <metal_stdlib>
using namespace metal;

struct data
{
    packed_float2 position;
    packed_float4 color;
};

struct vd
{
    float4 position [[position]];
    float4 color;
};

vertex vd basic_vertex(const device data* vertex_array [[buffer(0)]], unsigned int vid [[vertex_id]])
{
    vd outData = {
        float4(vertex_array[vid].position, 0.0, 1.0),
        vertex_array[vid].color
    };

    return outData;
}

fragment float4 basic_fragment(vd in [[stage_in]])
{
    return float4(in.color);
}
