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
  bc_init();
  bs_init();

  g_assert_true(0==chdir(g_get_user_config_dir()));
  gchar *curd=g_get_current_dir(); // g_canonicalize_filename
  g_assert_true(curd);
  g_assert_true(0==g_strcmp0("/home/user/.config", curd) || 0==g_strcmp0("/home/darren/.config", curd));
  g_free(curd); curd=NULL;

  g_assert_true(0==g_mkdir_with_parents("./adwible", 0755));
  g_assert_true(0==chdir("./adwible")); 

  bs_tanakh=bs_new(tanakh.n_total_chapters+1);
  bs_load(bs_tanakh, BIN_TANAKH);
  // bs_test(bs_tanakh);

  // const int r_md=g_mkdir_with_parents("./adwible", 0755);
  // g_assert_true(0==chdir("./adwible"));
  // g_message("r_md=%d", r_md);
  // if(-1==r_md){
  //   g_assert_true(EEXIST==errno);
  //   bs_load(bs_tanakh, BIN_TANAKH);
  // }else{
  //   g_assert_true(0==r_md);
  // }

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
  bs_free(bs_tanakh); bs_tanakh=NULL;

  // exit
  return e;

}
