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

int main(){

  GResource *r=g_resources_register2();

  // segfaults
  // https://gitlab.gnome.org/GNOME/libadwaita/-/blob/1.1.4/demo/adw-demo-window.c#L118
  // g_type_ensure(GTK_TYPE_DIALOG);
  // g_type_ensure(GTK_TYPE_BOX);
  // g_type_ensure(GTK_TYPE_BUTTON);
  // g_type_ensure(GTK_TYPE_LABEL);
  // g_type_ensure(GTK_TYPE_BOX);
  // g_type_ensure(GTK_TYPE_HEADER_BAR);
  // g_type_ensure(GTK_TYPE_APPLICATION_WINDOW);
  // g_type_ensure(GTK_TYPE_WINDOW);
  adw_init();
  GtkBuilder *b=gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/b7cj8w.ui"); g_assert_true(b);

  g_resources_unregister(r); g_resource_unref(r); r=NULL;

  return 0;

}
