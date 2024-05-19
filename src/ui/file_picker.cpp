#include <ui/file_picker.hpp>

namespace cpl_ui
{
    file_picker::file_picker(selection_mode mode) :
    m_mode(mode),
    m_current_directory(std::filesystem::current_path())
    {}

    void file_picker::render()
    {
        ImGui::Begin("File Picker");

        if(m_mode == selection_mode::directory)
        {
            ImGui::Text("Please Select a Directory");
        }
        else
        {
            ImGui::Text("Please Select a File");
        }

        if(m_current_directory.has_parent_path())
        {
            if(ImGui::Button(".."))
            {
                m_current_directory = m_current_directory.parent_path();
            }
        }

        for(auto const entry : std::filesystem::directory_iterator(m_current_directory))
        {
            if(entry.is_directory())
            {
                if(ImGui::Button(entry.path().filename().c_str()))
                {
                    m_current_directory = entry.path();
                }
            }
            else
            {
                if(m_mode == selection_mode::file)
                {
                    if(ImGui::Button(entry.path().filename().c_str()))
                    {
                        on_path_picked.notify(entry.path());
                    }
                }
                else
                {
                    ImGui::Text(entry.path().filename().c_str());
                }
            }
        }

        ImGui::End();
    }
}