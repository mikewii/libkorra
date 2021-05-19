#include "Global.hpp"

#include "Tools/Utils.hpp"

void PairInfo::SetPairInfo( Pair& _pp )
{
    Utils::copybytes(&this->__filename, _pp.Filename, ARC_File_s::FNAME_SIZE);
    this->__resourceHash    = _pp.ResourceHash;
    this->__XORLock         = _pp.XORLock;
    this->__decSize         = _pp.DecSize;
    this->__decompressed    = _pp.isDecompressed;

    this->__isSet           = true;
}

bool PairInfo::GetPairInfo( Pair &_pp )
{
    if ( _pp.cc.size() <= 0 ) return false;

    Utils::copybytes(_pp.Filename, this->__filename, ARC_File_s::FNAME_SIZE);
    _pp.ResourceHash    = this->__resourceHash;
    _pp.XORLock         = this->__XORLock;
    _pp.DecSize         = this->__decSize;
    _pp.isDecompressed    = this->__decompressed;

    return true;
}

void Pair::print( void )
{
    printf("\n##### Pair info #####\n");
    printf("Filename:       %s\n", this->Filename);
    printf("ResourceHash:   0x%08X\n", this->ResourceHash);
    printf("XORLock:        0x%08X\n", this->XORLock);
    printf("DecSize:        %d\n", this->DecSize);
    printf("isDecompressed: %s\n", this->isDecompressed ? "true" : "false");
}
