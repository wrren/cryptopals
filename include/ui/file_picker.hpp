#pragma once

#include <imgui.h>
#include <filesystem>
#include <ui/notifier.hpp>

namespace cpl_ui
{
    class file_picker
    {
    public:

        enum class selection_mode
        {
            file,
            directory
        };

        /**
         * @brief Construct a new file picker object using the specified selection mode.
         * 
         * @param mode Selection mode, determines whether the user is picking a file or directory
         */
        file_picker(selection_mode mode = selection_mode::file);

        /// @brief Notifier for path picked events.
        notifier<const std::filesystem::path&> on_path_picked;

        /**
         * @brief Renders the file picker.
         * 
         */
        void render();

    private:

        /// @brief Selection mode
        selection_mode          m_mode;
        /// @brief Current directory
        std::filesystem::path   m_current_directory;
    };
}