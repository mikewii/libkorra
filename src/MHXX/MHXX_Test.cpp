#include "MHXX/MHXX_Test.hpp"
#include "MHXX/MHXX.hpp"
#include "Tools/Test.hpp"

namespace MHXX {
namespace TEST {

void Extentions(std::vector<Pair>& vector, const char* filename, Utils::Collector& col)
{
    bool once = true;
    std::string quest_name;
    //printf("\n%s ", filename);
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        case MHXX::GMD::RESOURCE_HASH:{
            test<MHXX::GMD::cGMD>(pair);

            if (once)
            {
                MHXX::GMD::cGMD gmd(pair);
//                gmd.print_AllItems();
                quest_name = gmd.Get_ItemStr(0);
                once = false;
            }

            //printf("%s \n", gmd.get_ItemStr(0).c_str());
            //gmd.print_AllItems();
            break;
        }
        case MHXX::QDP::RESOURCE_HASH:{
            test<MHXX::QDP::cQuestPlus>(pair);
            break;
        }
        case MHXX::SEM::RESOURCE_HASH:{
            test<MHXX::SEM::cSetEmMain>(pair);

            MHXX::SEM::cSetEmMain sem(pair);
            //sem.print();
            break;
        }
        case MHXX::REM::RESOURCE_HASH:{
            test<MHXX::REM::cRewardEm>(pair);

            MHXX::REM::cRewardEm rem(pair);
            //rem.print();
            break;
        }
        case MHXX::SUP::RESOURCE_HASH:{
            test<MHXX::SUP::cSupply>(pair);

            MHXX::SUP::cSupply sup(pair);
            //sup.print(true);
            break;
        }
        case MHXX::QDL::RESOURCE_HASH:{
            test<MHXX::QDL::cQuestDataLink>(pair);

            MHXX::QDL::cQuestDataLink qdl(pair);
            //qdl.print();
            break;
        }
        case MHXX::ESL::RESOURCE_HASH:{ // broken for MHGU
            //MHXX::ESL::cEmSetList esl(pair);
            //esl.print();
            break;
        }
        case MHXX::EXT::RESOURCE_HASH:{
            MHXX::EXT::cEXT ext(pair);

            const auto& header0 = ext.GetHeader0();
            const auto& header1 = ext.GetHeader1();

            //col.Disable();
            if (col.IsActive())
            {
                col.Set_Value(8);
                col.Set_Operator(Utils::Collector::Op::Unique);
                //if (header0.Em[0].EmSetTargetID.idSub == 8)
                col.Add
                ({
                    header0.questID,
                    quest_name,
                    header0.questLv,

                    header0.acEquipSetNo
                });
                //out.push_back(pair);
            }
            else
            {
                //if (header0.Em[0].EmSetTargetID.idSub == 8)
                {
                    printf("q%07d %-3s %s\n", header0.questID, MHXX::QuestLv::GetStr(header0.questLv), quest_name.c_str());
                    printf("0: %d %d\n", header0.Em[0].EmSetTargetID.id, header0.Em[0].EmSetTargetID.idSub);
                    printf("1: %d %d\n", header0.Em[1].EmSetTargetID.id, header0.Em[1].EmSetTargetID.idSub);
//                    ext.print_Em0();
//                    ext.print_Em0();
                }
//                ext.print_Supply0();
//                ext.print_Supply1();
            }
            //pair.cc.writeToFile((fpath + ".ext").c_str());
            break;
        }
        } // switch
    }
}

}
}
