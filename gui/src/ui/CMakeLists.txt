add_library(zappy_gui_ui
        UIManager.cpp
        components/UIText.cpp
        components/UIPanel.cpp
        components/UIGamePanel.cpp
        components/UIGridManager.cpp
        components/UIButton.cpp
        components/UIImage.cpp
        components/UISlider.cpp
        components/UITextbox.cpp
        menus/PauseMenu.cpp
        menus/PlayerInspectorUI.cpp
        menus/AInspectorUI.cpp
        menus/TileInspectorUI.cpp
        hud/GameHUD.cpp
)
target_sources(zappy_gui_ui PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        IUIComponent.hpp
        UIManager.hpp
        components/UIText.hpp
        components/UIPanel.hpp
        components/UIGamePanel.hpp
        components/UIGridManager.hpp
        components/UIButton.hpp
        components/UIImage.hpp
        components/UISlider.hpp
        components/UITextbox.hpp
        menus/PauseMenu.hpp
        menus/PlayerInspectorUI.hpp
        menus/AInspectorUI.hpp
        menus/TileInspectorUI.hpp
        hud/GameHUD.hpp
)
target_include_directories(zappy_gui_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/gui/raylib
)
target_link_libraries(zappy_gui_ui PUBLIC
        zappy_gui_raylib
        zappy_shared
        zappy_gui_events
)
