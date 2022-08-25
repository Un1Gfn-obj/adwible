// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplication
#include <glib.h> // g_assert_true

#include "ui.h"
#include "bc.h"

#define ARGPARA const int argc, char *argv[]
#define ARGPASS argc, argv

int g_application_run2(ARGPARA){
  AdwApplication *app=adw_application_new("com.un1gfn.adwible", G_APPLICATION_FLAGS_NONE); g_assert_true(app);
  g_signal_connect(app, "activate", G_CALLBACK(ui_app_activate_cb), NULL);
  const int r=g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app); app=NULL;
  return r;
}

int main(ARGPARA){

  // init
  adw_init();
  ui_init_theme();
  ui_init_lb();
  bc_validate();
  ui_register_gres();

  // spawn
  const int e=g_application_run2(ARGPASS);

  // cleanup
  ui_unregister_gres();

  // exit
  return e;

}
