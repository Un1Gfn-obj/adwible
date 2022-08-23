#include <gtk/gtk.h>
#include <adwaita.h>

#include "ck3fm7.h"

// GtkWindow AdwWindow
static GObject *w=NULL;

static GResource *g_resources_register2(){

  
  GResource *r=j72qkd_get_resource(); _Static_assert(NULL==G_RESOURCE_ERROR_NOT_FOUND, ""); g_assert_true(r);
  // const char *path="/com/un1gfn/ck3fm7";
  // g_print("%s\n", path);
  // char **cs=g_resource_enumerate_children(r, path, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL); g_assert_true(cs);
  // for(char **c=cs; *c; ++c)
  //   g_print("%s/%s\n", path, *c);
  // g_strfreev(cs); cs=NULL;

  // gsize size=0;
  // guint32 flags=0;
  // g_assert_true(g_resource_get_info(r, "/com/un1gfn/ck3fm7/b7cj8w.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, &size, &flags, NULL));
  // g_assert_true(0==flags);
  // g_print("%zu bytes\n", size);

  // binary dump
  // g_resource_lookup_data()

  g_resources_register(r);
  return r;

}

static void activate_cb(AdwApplication *app){

  // get window
  // segfault if not initialized
  // https://gitlab.gnome.org/GNOME/libadwaita/-/blob/1.1.4/demo/adw-demo-window.c#L118
  // g_type_ensure(...);
  // adw_init();
  GtkBuilder *b=gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/b7cj8w.ui"); g_assert_true(b);
  w=gtk_builder_get_object(b, "tf2fhx"); g_assert_true(w);
  gtk_window_set_default_size(GTK_WINDOW(w), 720/2, 1200/2);

  // render window
  gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(w));
  g_object_unref(G_OBJECT(b)); b=NULL;
  // gtk_window_present(GTK_WINDOW(w));
  gtk_widget_show(GTK_WIDGET(w));

}

int main(const int argc, char *argv[]){

  GResource *r=NULL;
  AdwApplication *app=NULL;
  int exit_code=0;

  // init
  r=g_resources_register2();

  // spawn
  app=adw_application_new("com.un1gfn.adwible", G_APPLICATION_FLAGS_NONE); g_assert_true(app);
  g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

  // run
  exit_code=g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app); app=NULL;

  // end
  g_resources_unregister(r); r=NULL; // g_resource_unref(r); // double free // valgrind "Invalid read of size 4"
  return exit_code;

}
