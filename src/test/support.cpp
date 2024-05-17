#include <cryptopals-test/support.hpp>

namespace cpl_test
{
    static std::filesystem::path _project_root;

    std::optional<std::filesystem::path> get_input(const std::string& filename)
    {
        if(std::filesystem::exists(_project_root / "inputs" / filename))
        {
            return _project_root / "inputs" / filename;
        }

        return std::nullopt;
    }

    void set_root_directory(const std::filesystem::path& root)
    {
        _project_root = root;
    }
}