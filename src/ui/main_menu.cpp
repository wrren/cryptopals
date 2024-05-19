#include <ui/main_menu.hpp>
#include <imgui.h>

namespace cpl_ui
{
    void main_menu::render()
    {
        ImGui::Begin("Main Menu");

        ImGui::Text("Please Select a Task");

        if (ImGui::Button("AES"))    
        {
            on_menu_item_clicked.notify(menu_item::aes_menu_item);
        }
        ImGui::SameLine();
        if (ImGui::Button("XOR"))    
        {
            on_menu_item_clicked.notify(menu_item::xor_menu_item);
        }

        ImGui::End();
    }
}