#include <gtk/gtk.h>
#include <adwaita.h>

#include "ck3fm7.h"

// gtk_window_destroy() 

static GResource *g_resources_register2(){

  GResource *r=j72qkd_get_resource(); g_assert_true(r);
  _Static_assert(NULL==G_RESOURCE_ERROR_NOT_FOUND, "");
  const char *path="/com/un1gfn/ck3fm7";
  g_print("%s\n", path);
  char **cs=g_resource_enumerate_children(r, path, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL); g_assert_true(cs);
  for(char **c=cs; *c; ++c)
    g_print("%s/%s\n", path, *c);
  g_strfreev(cs); cs=NULL;

  gsize size=0;
  guint32 flags=0;
  g_assert_true(g_resource_get_info(r, "/com/un1gfn/ck3fm7/b7cj8w.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, &size, &flags, NULL));
  g_assert_true(0==flags);
  g_print("%zu bytes\n", size);

  // binary dump
  // g_resource_lookup_data()

  g_resources_register(r);
  return r;

}

static void activate_cb(AdwApplication *app){

  // segfaults
  // https://gitlab.gnome.org/GNOME/libadwaita/-/blob/1.1.4/demo/adw-demo-window.c#L118
  // g_type_ensure(...);
  adw_init();
  GtkBuilder *b=gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/b7cj8w.ui"); g_assert_true(b);

  GObject *dialog = gtk_builder_get_object(b, "dialog1"); g_assert_true(dialog);

  gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(dialog));

  // gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  // adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), label);
  gtk_window_present(GTK_WINDOW(dialog));

}

int main(const int argc, char *argv[]){

  // init
  GResource *r=g_resources_register2();

  // spawn
  g_autoptr(AdwApplication) app = adw_application_new("com.un1gfn.adwible", G_APPLICATION_FLAGS_NONE); g_assert_true(app);
  g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

  // run
  const int exit_code=g_application_run(G_APPLICATION(app), argc, argv);
  g_resources_unregister(r); g_resource_unref(r); r=NULL;
  return exit_code;

}
