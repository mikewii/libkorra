#include "MHXX/MHXX_Test.hpp"
#include "MHXX/MHXX.hpp"
#include "Tools/Test.hpp"
#include "Tools/Folder.hpp"
#include "Tools/ARC.hpp"


namespace MHXX {
namespace TEST {
static Utils::Collector collector(0);

void run_tests()
{
    collector.Set_Path(Utils::Get_User_Home().append("test"));
    std::string                 test_folder = "test/MHXX_CQs/from_MHGU";
    std::vector<std::string>    selected_files =
    {
        //"q0000105.arc",
//            "q0110101.arc",
//            "q0040505.arc",
//            "q0000633.arc",
//            "q0000712.arc",
//            "q0011424.arc",
//            "q0000914.arc",
//            "q0011213.arc",
//            "q0000234.arc",
//            "q0000223.arc",
//            "q0010661.arc",
    };

    Folder folder(Utils::Get_User_Home().append(test_folder));

    const auto files = folder.Get_ListOfFiles();

    for (const auto& entry : files)
    {
        if (!selected_files.empty())
        {
            bool found = false;

            for (auto& str : selected_files)
                if (str.compare(entry.filename().string()) == 0)
                {
                    found = true;
                    break;
                }

            if (!found) continue;
        }

        if (entry.filename().string().size() != 12) // quest filename size
            continue;

        std::vector<Pair>   vector;
        CContainer          arc(entry.string());

        ARC(arc, &vector).ExtractAll();

        MHXX::TEST::Extentions(vector, entry);
    }

    collector.Show();
}

void Extentions(const std::vector<Pair>& vector, const std::string& filename)
{
    bool once = true;
    std::string quest_name;
    //printf("\n%s ", filename.c_str());
    for (const auto& pair : vector)
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

            //collector.Disable();
            if (collector.IsActive())
            {
                collector.Set_Value(8);
                collector.Set_Operator(Utils::Collector::Op::Unique);
                //if (header0.Em[0].EmSetTargetID.idSub == 8)
                collector.Add
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
