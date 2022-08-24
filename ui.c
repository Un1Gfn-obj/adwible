// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplicaiton
#include <gio/gio.h> // GResource
#include <sys/utsname.h> // uname

#include "ck3fm7.h" // j72qkd_get_resource
#include "bc.h"

#define MAXCHAP 150

static GResource *r=NULL;

static char lb[1+150][3+1]={0};

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

static inline void add_testament(GtkBox *const box, const bc_testament_t *const testament){

  // add book groups to testament
  for(const bc_group_t *g=*testament; 0!=g->n_books; ++g){

    GtkWidget *ag=adw_preferences_group_new(); // AdwPreferencesGroup
    // GValue v=G_VALUE_INIT; g_assert_true(&v==g_value_init(&v, G_TYPE_STRING));
    // g_value_set_static_string(&v, g->title); g_assert_true(G_VALUE_HOLDS_STRING(&v));
    // g_object_set_property(G_OBJECT(ag), "title", &v);
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(ag), g->title);
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(ag), g->description);

    // add books to book group
    for(const bc_book_t *b=g->books; 0!=b->n_chapters; ++b){

      GtkWidget *er=adw_expander_row_new(); // AdwExpanderRow
      adw_preferences_row_set_title(ADW_PREFERENCES_ROW(er), b->title);
      adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(er), b->subtitle);
      // if(expanded_before_last_exit)
      //   adw_expander_row_set_expanded(ADW_EXPANDER_ROW(er), TRUE);
      adw_preferences_group_add(ADW_PREFERENCES_GROUP(ag), er);

      GtkWidget *fb=gtk_flow_box_new(); // GtkFlowBox
      gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(fb), 255);
      g_assert_true(1<=b->n_chapters && b->n_chapters<=MAXCHAP);
      for(int i=1; i<=b->n_chapters; ++i){
        GtkWidget *tb=gtk_toggle_button_new_with_label(lb[i]); // GtkToggleButton
        gtk_widget_set_halign(tb, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(tb, FALSE);
        gtk_flow_box_append(GTK_FLOW_BOX(fb), tb);
      }

      adw_expander_row_add_row(ADW_EXPANDER_ROW(er), fb);

    }

    // add testment to GtkBox in GtkScrolledWindow
    gtk_box_append(box, ag);

  }
}

void ui_init_lb(){
  g_strlcpy(lb[0], "0", 2);
  for(int i=1; i<=MAXCHAP; ++i)
    snprintf(lb[i], 3+1, "%d", i);
}

void ui_app_activate_cb(AdwApplication *app){


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

  #define SCALEDOWN 2

  _Static_assert(0==W%SCALEDOWN, "");
  const int width=W/SCALEDOWN;
  const int height=H/SCALEDOWN;

  // segfault if not initialized
  // https://gitlab.gnome.org/GNOME/libadwaita/-/blob/1.1.4/demo/adw-demo-window.c#L118
  // g_type_ensure(...);
  // adw_init();

  // GtkBuilder *x[]={
  //   gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/adwible.ui"),
  //   gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/demo.ui"),
  // };
  // _Static_assert(sizeof(x)==2*sizeof(void*));
  // for(gsize i=0; i<sizeof(x)/sizeof(void*); ++i){

  GtkBuilder *b=gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/adwible.ui"); g_assert_true(b);

  GObject *const box_tanakh=gtk_builder_get_object(b, "qgnxl8"); g_assert_true(box_tanakh);
  add_testament(GTK_BOX(box_tanakh), &tanakh);

  GObject *const win=gtk_builder_get_object(b, "tf2fhx"); g_assert_true(win);
  {

    GtkCssProvider *css=gtk_css_provider_new(); g_assert_true(css);
    gtk_css_provider_load_from_resource(css, "/com/un1gfn/ck3fm7/adwible.css");

    GtkStyleContext *ct=gtk_widget_get_style_context(GTK_WIDGET(win)); g_assert_true(ct);
    gtk_style_context_add_provider(ct, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);

  }
  if(!isMobile()){
    g_message("not wt88047, resizing...");
    gtk_window_set_default_size(GTK_WINDOW(win), width, height);
  }
  gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(win));
  g_object_unref(G_OBJECT(b)); b=NULL;
  gtk_widget_show(GTK_WIDGET(win)); // gtk_window_present(GTK_WINDOW(win));

  // GtkFlowBox no animation
  // GObject *const fb=gtk_builder_get_object(b, "dx7fws"); g_assert_true(fb);
  // gulong signalID=g_signal_lookup("child-activated", gtk_flow_box_get_type());
  // g_assert_true(1<signalID && signalID<(gulong)(-2));
  // g_assert_true(0==g_signal_handlers_disconnect_matched(fb, G_SIGNAL_MATCH_ID, signalID, 0, NULL, NULL, NULL));

}

void ui_register_gres(){

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

void ui_unregister_gres(){
  g_resources_unregister(r); r=NULL;
  // g_resource_unref(r); // double free // valgrind "Invalid read of size 4"
}
