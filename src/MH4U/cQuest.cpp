#include "MH4U/Quest.hpp"

namespace MH4U {

void QuestEditor::initialize(void)
{
    this->m_p_header = reinterpret_cast<sQuest*>(this->m_data.data());
}

void QuestEditor::read_header(void)
{
    this->m_header = *this->m_p_header;

}

}
