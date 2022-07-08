#ifndef N3DS
#include "MH4U/MH4U_Test.hpp"
#include "Tools/Folder.hpp"
#include "Tools/File.hpp"
#include "Tools/ARC.hpp"
#include "Tools/MCA.hpp"
#include "MH4U/Crypto.hpp"
#include "MH4U/Quest.hpp"


namespace MH4U {
namespace TEST {
Utils::Collector collector("mh4u_collected_data.txt");

void quest_prints(const std::filesystem::path& path)
{
    CContainer in(path);

    sQuest* quest = reinterpret_cast<sQuest*>(in.data());

    auto aa = quest->get_supply_box_items(ItemBoxID::SUPPLY_BOX);

    quest->print();

    return;
}

void ExtractQuests(const std::filesystem::path& path)
{
    CContainer in, out;
    std::filesystem::path outpath = path;

    outpath.replace_extension(".dec");


    File::file_to_cc(path, in);

    MH4U::Crypto().decode_quest(in, out);

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

        if (arc.IsARC())
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

void ExtractSave(const std::filesystem::__cxx11::path &path)
{
    CContainer in, out;
    std::filesystem::path outpath = path;

    outpath.replace_extension(".dec");


    File::file_to_cc(path, in);

    MH4U::Crypto().decode_ext_data(in, out);

    out.write_To_File(outpath);
}

void mca(const std::filesystem::__cxx11::path &path)
{
    CContainer in, out;
    //std::filesystem::path outpath = path;

    //outpath.replace_extension(".mca");


    File::file_to_cc(path, in);
    MCA mca(in);
    mca.print();
}

void quest_ext_create(const std::filesystem::path &path)
{
    MH4U::Quest quest;

    quest.set_out_path(path);
    quest.create_ext_quests_files(path);
}

}
}
#endif
