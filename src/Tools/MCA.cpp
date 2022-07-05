#include "Tools/MCA.hpp"

MCA::MCA()
{

}

MCA::MCA(const CContainer &_cc)
{
    if (_cc.at<u32>(0) != this->MAGIC) {
        NotifyError("Not an MCA");
        return;
    }

    this->h = _cc.at<MCA::header>(0);
}

MCA::~MCA()
{

}

void MCA::print() const
{
    printf("\n##### MCA Header #####\n");

    printf("Magic:                  0x%08X\n", this->h.Magic);
    printf("Version:                0x%08X | %d\n", this->h.Version, this->h.Version);

    printf("Channel_count:          0x%04X | %d\n", this->h.Channel_count, this->h.Channel_count);
    printf("Interleave_block_size:  0x%04X | %d\n", this->h.Interleave_block_size, this->h.Interleave_block_size);
    printf("Num_samples:            0x%08X | %d\n", this->h.Num_samples, this->h.Num_samples);
    printf("Sample_rate:            0x%08X | %d\n", this->h.Sample_rate, this->h.Sample_rate);
    printf("Loop_start_sample:      0x%08X | %d\n", this->h.Loop_start_sample, this->h.Loop_start_sample);
    printf("Loop_end_sample:        0x%08X | %d\n", this->h.Loop_end_sample, this->h.Loop_end_sample);

    printf("Header_size:            0x%08X | %d\n", this->h.Header_size, this->h.Header_size);
    printf("Data_size:              0x%08X | %d\n", this->h.Data_size, this->h.Data_size);
    printf("Duration_seconds:       0x%08X | %f\n", *reinterpret_cast<const u32*>( &this->h.Duration_seconds ), this->h.Duration_seconds);

    printf("Coef_shift:             0x%08X | %d\n", this->h.Header_size, this->h.Header_size);
    printf("unk0:                   0x%08X | %d\n", this->h.Data_size, this->h.Data_size);
    printf("Coef_spacing:           0x%08X | %d\n", this->h.Coef_spacing, this->h.Coef_spacing);

}

void MCA::Fix_LoopEndSample(void) {
    if (this->h.Loop_end_sample > this->h.Num_samples)
        this->h.Loop_end_sample = this->h.Num_samples;
}
