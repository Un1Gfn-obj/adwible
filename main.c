// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplication
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

  // init
  adw_init();
  ui_init_theme();
  ui_init_lb();
  ui_register_gres();
  bc_init();

  bs_init();
  bs_tanakh=bs_new(tanakh.n_total_chapters+1);

  // g_print("0x%016lX\n", *locate(bs_tanakh, 32));
  // bs_toggle(bs_tanakh, 32, TRUE);
  // g_print("0x%016lX\n", *locate(bs_tanakh, 32));

  // spawn
  int e=0;
  e=g_application_run2(ARGPASS);

  // cleanup
  ui_unregister_gres();
  bs_free(bs_tanakh); bs_tanakh=NULL;

  // exit
  return e;

}
