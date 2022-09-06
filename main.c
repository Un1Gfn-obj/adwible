// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplication
#include <errno.h>
#include <glib.h> // g_assert_true

#include "ui.h"
#include "bc.h" // biblical canon
#include "bs.h" // bitset

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

  // backend
  bs_chdir();
  bs_init();
  bc_init(&bc_tanakh);
  bc_init(&bc_newtestament);

  // gui
  adw_init();
  ui_theme();
  ui_init_lb();
  ui_register_gres();

  // spawn
  int e=0;
  e=g_application_run2(ARGPASS);

  // cleanup
  ui_unregister_gres();
  bs_free(bc_tanakh.dyn->bs); bc_tanakh.dyn->bs=NULL;
  bs_free(bc_newtestament.dyn->bs); bc_newtestament.dyn->bs=NULL;

  // exit
  return e;

}
