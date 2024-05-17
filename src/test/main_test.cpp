#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include <cryptopals-test/support.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    std::filesystem::path bin_path(argv[0]);

    auto has_xmake_config = [](const std::filesystem::path& directory) -> bool
    {
        return std::filesystem::exists(directory / "xmake.lua");
    };

    auto current_path = bin_path;
    bool found_root_directory = false;

    while(current_path.has_parent_path())
    {
        current_path = current_path.parent_path();

        if(has_xmake_config(current_path))
        {
            cpl_test::set_root_directory(current_path);
            found_root_directory = true;
            break;
        }
    }

    if(!found_root_directory)
    {
        std::cerr << "Failed to locate root project directory." << std::endl;
        return EXIT_FAILURE;
    }

    auto input = cpl_test::get_input("aes-ecb.txt");
    if(!input)
    {
        std::cerr << "Failed to locate example project input file." << std::endl;
        return EXIT_FAILURE;
    }

    return Catch::Session().run(argc, argv);
}