#pragma once
#include "Global.hpp"

/*
 * sResource::TypeInfo
 * {
 *      mAttr
 *      mExt
 *      mName
 *      mSuper
 *      mSize
 * }
*/
class MCA // ADPCM 00E62E7C
{
public:
    struct header {
        u32     Magic;
        u32     Version;

        u16     Channel_count;
        u16     Interleave_block_size;
        u32     Num_samples;
        u32     Sample_rate;
        u32     Loop_start_sample;
        u32     Loop_end_sample;

        u32     Header_size;
        u32     Data_size;
        float   Duration_seconds;

        u32     Coef_shift;
        u32     unk0;
        u32     Coef_spacing;

        // format
        // marker num
    };


    MCA();
    MCA(const CContainer& _cc);
    ~MCA();

    void print(void) const;

    void Set_channel_count(const u16 count) { this->h.Channel_count = count; }
    void Set_channel_count_stereo(void) { this->h.Channel_count = 2; }
    void Set_channel_count_mono(void) { this->h.Channel_count = 1; }

private:
    header h;

    static const u32 MAGIC = 0x5044414D; // MADP

    void Fix_LoopEndSample(void);
};
