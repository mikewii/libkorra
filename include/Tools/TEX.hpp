#pragma once
#include <types.h>

struct TEX_s {
    static const u32 MAGIC = 0x00584554;

    u32 magic = MAGIC;

    /* 00633F9C
     * Desc
     *
     * width
     * height
     * depth
     *
     * levelCount
     * arrayCount
     *
     * format
     * GPUFormat
     * SRGB
     * miscFlags
     * EncodeMode
     * DecodeMode
     * MSAAMode
     */
};

class TEX {

};
