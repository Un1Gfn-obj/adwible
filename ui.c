// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplicaiton
#include <gio/gio.h> // GResource
#include <sys/utsname.h> // uname

#include "ck3fm7.h" // j72qkd_get_resource

static GResource *r=NULL;

static inline gboolean isMobile(){
  struct utsname name={0};
  g_assert_true(0==uname(&name));
  g_assert_true(0==g_strcmp0("Linux", name.sysname));
  gboolean ret=FALSE;
  if(FALSE)
    g_assert_true(FALSE); // g_assert_not_reached()
  else if( (0==g_strcmp0("xiaomi-wt88047", name.nodename)) && (0==g_strcmp0("aarch64", name.machine)))
    ret=TRUE;
  else if( (0==g_strcmp0("820g3",          name.nodename)) && (0==g_strcmp0("x86_64",  name.machine)))
    ret=FALSE;
  else
    g_assert_true(FALSE); // g_assert_not_reached()
  return ret;
}

void g_resources_register2(){

  g_assert_true(!r);
  r=j72qkd_get_resource();
  _Static_assert(NULL==G_RESOURCE_ERROR_NOT_FOUND, ""); g_assert_true(r);

  // const char *path="/com/un1gfn/ck3fm7";
  // g_print("%s\n", path);
  // char **cs=g_resource_enumerate_children(r, path, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL); g_assert_true(cs);
  // for(char **c=cs; *c; ++c)
  //   g_print("%s/%s\n", path, *c);
  // g_strfreev(cs); cs=NULL;

  // gsize size=0;
  // guint32 flags=0;
  // g_assert_true(g_resource_get_info(r, "/com/un1gfn/ck3fm7/adwible.ui.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, &size, &flags, NULL));
  // g_assert_true(0==flags);
  // g_print("%zu bytes\n", size);

  // binary dump
  // g_resource_lookup_data()

  g_resources_register(r);

}

#define SCALEDOWN 2
//
//        +---------+
//        |         |
//        |         |
//        |         |
#define H             1000
//        |         |
//        |         |
//        |         |
//        +---------+
#define      W 720

void app_activate_cb(AdwApplication *app){
  _Static_assert(0==W%SCALEDOWN, "");
  const int width=W/SCALEDOWN;
  const int height=H/SCALEDOWN;
  // segfault if not initialized
  // https://gitlab.gnome.org/GNOME/libadwaita/-/blob/1.1.4/demo/adw-demo-window.c#L118
  // g_type_ensure(...);
  // adw_init();
  GtkBuilder *x[]={
    gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/adwible.ui"),
    gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/demo.ui"),
  };
  // _Static_assert(sizeof(x)==2*sizeof(void*));
  for(gsize i=0; i<sizeof(x)/sizeof(void*); ++i){
    GtkBuilder *b=x[i];
    g_assert_true(b);
    GObject *const w=gtk_builder_get_object(b, "tf2fhx"); g_assert_true(w);
    if(!isMobile()){
      g_message("not wt88047, resizing...");
      gtk_window_set_default_size(GTK_WINDOW(w), width, height);
    }
    gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(w));
    g_object_unref(G_OBJECT(b)); b=NULL;
    gtk_widget_show(GTK_WIDGET(w)); // gtk_window_present(GTK_WINDOW(w));
  }

  // GtkFlowBox no animation
  // GObject *const fb=gtk_builder_get_object(b, "dx7fws"); g_assert_true(fb);
  // gulong signalID=g_signal_lookup("child-activated", gtk_flow_box_get_type());
  // g_assert_true(1<signalID && signalID<(gulong)(-2));
  // g_assert_true(0==g_signal_handlers_disconnect_matched(fb, G_SIGNAL_MATCH_ID, signalID, 0, NULL, NULL, NULL));

}

void g_resources_unregister2(){
  g_resources_unregister(r); r=NULL;
  // g_resource_unref(r); // double free // valgrind "Invalid read of size 4"
}
