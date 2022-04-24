#include "MH4U/MH4U_Test.hpp"
#include "Tools/Folder.hpp"
#include "Tools/File.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

namespace MH4U {
namespace TEST {
Utils::Collector collector("mh4u_collected_data.txt");

void ExtractQuests(const std::filesystem::path& path)
{
    CContainer in, out;
    std::filesystem::path outpath = path;

    outpath.replace_extension(".dec");


    File::File_To_CC(path, in);

    Decode(in, out);

    out.write_To_File(outpath);
}

void runTests(void)
{
    collector.Set_Path(Utils::Get_User_Home().append("test"));
    std::string                 test_folder = "test/MH4U";
    std::vector<std::string>    selected_files =
    {
    };

    Folder folder(Utils::Get_User_Home().append(test_folder));

    const auto files = folder.Get_ListOfFiles(false);

    for (const auto& entry : files)
    {
        if (!Utils::Filter::Is_InVector(entry, selected_files))
            continue;

        std::vector<Pair>   vector;
        CContainer          container(entry.string());

        ARC arc(container, vector);

        if (arc.Is_ARC())
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

        CContainer container_out;
        ARC arc_out;

        arc_out.MakeARC(container_out, vector);

        container_out.write_To_File(Utils::Get_User_Home().append("test/MH4U/arc.arc"));
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
            auto pp = std::filesystem::path(pair.info.Filename);

            MH4U::LMD::cLMD lmd(pair);

            lmd.print_Strings();
        }
        }
    }
}

void Replace(std::vector<Pair> &vector, const std::filesystem::__cxx11::path &path)
{
    for (auto& pair : vector)
    {
        switch(pair.info.ResourceHash){
        default:break;
        case MH4U::LMD::RESOURCE_HASH:{
            MH4U::LMD::cLMD lmd(pair);

            for (u32 i = 0; i < lmd.Get_Strings_Count(); i++)
                lmd.replace_String("ass", i);

            lmd.write(pair);
        }
        }
    }
}

}
}
