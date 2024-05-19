#pragma once

#include <ui/notifier.hpp>

namespace cpl_ui
{
    class main_menu
    {
    public:

        enum class menu_item
        {
            xor_menu_item,
            aes_menu_item
        };

        notifier<menu_item> on_menu_item_clicked;

        /**
         * @brief Render the main menu.
         * 
         */
        void render();
    };
}