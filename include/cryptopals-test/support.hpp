#include <string>
#include <filesystem>
#include <optional>

namespace cpl_test
{
    /**
     * @brief Get the full path to an input file
     * 
     * @param filename Input file name
     * @return std::optional<std::filesystem::path> Full path to input file.
     */
    std::optional<std::filesystem::path> get_input(const std::string& filename);

    /**
     * @brief Set the root directory for the test run.
     * 
     * @param root Root directory.
     */
    void set_root_directory(const std::filesystem::path& root);
}