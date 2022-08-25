#pragma once

#include <adwaita.h> // AdwApplication*

void ui_theme();
void ui_init_lb();
void ui_register_gres();
void ui_app_activate_cb(AdwApplication*);
void ui_unregister_gres();
