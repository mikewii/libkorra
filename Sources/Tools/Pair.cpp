#include "Tools/Pair.hpp"
#include "Tools/Utils.hpp"

void PairInfoKeeper::SetPairInfo(Pair& _pp)
{
    Utils::copybytes(&this->__info.Filename, _pp.info.Filename, FNAME_SIZE);
    this->__info.ResourceHash   = _pp.info.ResourceHash;
    this->__info.XORLock        = _pp.info.XORLock;
    this->__info.DecSize        = _pp.info.DecSize;
    this->__info.isDecompressed = _pp.info.isDecompressed;

    this->__isSet           = true;
}

bool PairInfoKeeper::GetPairInfo(Pair &_pp)
{
    if (_pp.cc.size() <= 0) return false;

    Utils::copybytes(_pp.info.Filename, this->__info.Filename, FNAME_SIZE);
    _pp.info.ResourceHash    = this->__info.ResourceHash;
    _pp.info.XORLock         = this->__info.XORLock;
    _pp.info.DecSize         = this->__info.DecSize;
    _pp.info.isDecompressed  = this->__info.isDecompressed;

    return true;
}

void PairInfo::print(void)
{
    printf("\n##### Pair info #####\n");
    printf("Filename:       %s\n", this->Filename);
    printf("ResourceHash:   0x%08X\n", this->ResourceHash);
    printf("XORLock:        0x%08X\n", this->XORLock);
    printf("DecSize:        %d\n", this->DecSize);
    printf("isDecompressed: %s\n", this->isDecompressed ? "true" : "false");
}
