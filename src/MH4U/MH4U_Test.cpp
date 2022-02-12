#include "MH4U/MH4U_Test.hpp"
#include "Tools/Folder.hpp"
#include "Tools/File.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

namespace MH4U {
namespace TEST {
Utils::Collector collector("mh4u_collected_data.txt");


void runTests(void)
{
    collector.Set_Path(Utils::Get_User_Home().append("test"));
    std::string                 test_folder = "test/MH4U";
    std::vector<std::string>    selected_files =
    {
        "Diva_eng.lmd",
    };

    Folder folder(Utils::Get_User_Home().append(test_folder));

    const auto files = folder.Get_ListOfFiles(false);

    for (const auto& entry : files)
    {
        if (!Utils::Filter::Is_InVector(entry, selected_files))
            continue;

        std::vector<Pair>   vector;
        CContainer          container(entry.string());

        ARC arc(container, &vector);

        if (arc.isARC())
        {
            arc.ExtractAll();
            MH4U::TEST::Extentions(vector, entry);
        }
        else
        {
            MH4U::LMD::cLMD lmd(container);

            lmd.print_Data0();
            lmd.print_counts();
        }
    }

    collector.Show();
}

void Extentions(const std::vector<Pair> &vector, const std::filesystem::path& path)
{
    for (const auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        default:break;
        case MH4U::LMD::RESOURCE_HASH:{
            MH4U::LMD::cLMD lmd(pair);

            for (const auto& info : lmd.__Get_U16String_info_vector())
            {
                if (info.str_size != info.str_size_copy)
                {
                    printf("%s\n", path.filename().string().c_str());
                    pair.info.print_Filename();
                }
            }
            break;
        }
        }
    }

}

}
}
