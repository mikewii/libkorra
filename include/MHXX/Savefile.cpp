#include "Savefile.hpp"

namespace MHXX {

Savefile::Savefile(const CContainer& container)
{
    if (container.size() == SaveFile_s::VALID_SAVEFILE_SIZE) {
        Utils::copybytes(&this->savefile, container.data(), sizeof(SaveFile_s));
    }
    else return; // notify error
}

void Savefile::print_Flags(void) const
{
}

void SaveSlot::extract(CContainer& container) const
{
    container.resize(SaveSlot::SLOT_SIZE);

    Utils::copybytes(container.data(), this, SaveSlot::SLOT_SIZE);
}

}
