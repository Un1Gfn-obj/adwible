// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplicaiton
#include <gio/gio.h> // GResource
#include <sys/utsname.h> // uname

#include "ck3fm7.h" // j72qkd_get_resource
#include "bc.h"

#define MAXCHAP 150

static gboolean dark=FALSE;

static GResource *gres=NULL;
static GtkCssProvider *css_row_dark=NULL;
static GtkCssProvider *css_row_light=NULL;
GtkBuilder *builder=NULL;

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

static inline void click_keep_bg(GtkWidget *const widget){
  GtkStyleContext *const styct=gtk_widget_get_style_context(widget); g_assert_true(styct); g_assert_true(GTK_STATE_FLAG_DIR_LTR==gtk_style_context_get_state(styct));
  // gtk_style_context_lookup_color(styct, "theme_bg_color", &color);
  // gtk_style_context_lookup_color(styct, "theme_selected_bg_color", &color);
  // gtk_style_context_to_string()
  // g_print("rgba(%.3f, %.3f, %.3f, %.3f)\n", color.red, color.green, color.blue, color.alpha);
  if(dark)
    gtk_style_context_add_provider(styct, GTK_STYLE_PROVIDER(css_row_dark), GTK_STYLE_PROVIDER_PRIORITY_USER); // apply css
  else
    gtk_style_context_add_provider(styct, GTK_STYLE_PROVIDER(css_row_light), GTK_STYLE_PROVIDER_PRIORITY_USER); // apply css
}

static inline void add_testament(GtkBox *const box, const bc_testament_t *const testament){

  // add book groups to testament
  for(const bc_group_t *g=*testament; 0!=g->n_books; ++g){

    GtkWidget *ag=adw_preferences_group_new(); // AdwPreferencesGroup
    // GValue v=G_VALUE_INIT;
    // GValue v={0}; g_assert_true(&v==g_value_init(&v, G_TYPE_STRING));
    // g_value_set_static_string(&v, g->title); g_assert_true(G_VALUE_HOLDS_STRING(&v));
    // g_object_set_property(G_OBJECT(ag), "title", &v);
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(ag), g->title);
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(ag), g->description);

    // add books to a book group
    for(const bc_book_t *b=g->books; 0!=b->n_chapters; ++b){

      GtkWidget *er=adw_expander_row_new(); // AdwExpanderRow
      adw_preferences_row_set_title(ADW_PREFERENCES_ROW(er), b->title);
      if(b->subtitle) adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(er), b->subtitle);
      // if(expanded_before_last_exit)
      //   adw_expander_row_set_expanded(ADW_EXPANDER_ROW(er), TRUE);

      // add a book to a book group
      adw_preferences_group_add(ADW_PREFERENCES_GROUP(ag), er);

      GtkWidget *fb=gtk_flow_box_new(); // GtkFlowBox
      gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(fb), 255);
      gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(er), FALSE);
      // gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(er), FALSE);

      // add chapters to a book
      g_assert_true(1<=b->n_chapters && b->n_chapters<=MAXCHAP);
      for(int i=1; i<=b->n_chapters; ++i){
        GtkWidget *tb=gtk_toggle_button_new_with_label(lb[i]); // GtkToggleButton
        gtk_widget_set_halign(tb, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(tb, FALSE);
        // add a chapter to a book
        gtk_flow_box_append(GTK_FLOW_BOX(fb), tb);
      }
      adw_expander_row_add_row(ADW_EXPANDER_ROW(er), fb);
      GtkWidget *const row=gtk_widget_get_parent(fb);
      g_assert_true(g_type_check_instance_is_a((gpointer)row, gtk_list_box_row_get_type()));
      click_keep_bg(row);

    }

    // add a testment to GtkBox in GtkScrolledWindow
    gtk_box_append(box, ag);

  }
}

void ui_init_theme(){
  AdwStyleManager *const mgm=adw_style_manager_get_default();
  g_assert_true(adw_style_manager_get_system_supports_color_schemes(mgm));
  g_assert_true(ADW_COLOR_SCHEME_DEFAULT==adw_style_manager_get_color_scheme(mgm)); // g_print("%d\n", adw_style_manager_get_color_scheme(mgm));
  dark=adw_style_manager_get_dark(mgm);
  // adw_style_manager_set_color_scheme(mgm, ADW_COLOR_SCHEME_FORCE_DARK); g_assert_true(ADW_COLOR_SCHEME_FORCE_DARK==adw_style_manager_get_color_scheme(mgm));
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

  GObject *const box_tanakh=gtk_builder_get_object(builder, "qgnxl8"); g_assert_true(box_tanakh);
  add_testament(GTK_BOX(box_tanakh), &tanakh);

  GObject *const win=gtk_builder_get_object(builder, "tf2fhx"); g_assert_true(win);
  if(!isMobile()){
    g_message("not wt88047, resizing...");
    gtk_window_set_default_size(GTK_WINDOW(win), width, height);
  }
  gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(win));
  gtk_widget_show(GTK_WIDGET(win)); // gtk_window_present(GTK_WINDOW(win));

  // GtkFlowBox no animation
  // GObject *const fb=gtk_builder_get_object(builder, "dx7fws"); g_assert_true(fb);
  // gulong signalID=g_signal_lookup("child-activated", gtk_flow_box_get_type());
  // g_assert_true(1<signalID && signalID<(gulong)(-2));
  // g_assert_true(0==g_signal_handlers_disconnect_matched(fb, G_SIGNAL_MATCH_ID, signalID, 0, NULL, NULL, NULL));

}

void ui_register_gres(){

  g_assert_true(!gres);
  gres=j72qkd_get_resource();
  _Static_assert(NULL==G_RESOURCE_ERROR_NOT_FOUND, ""); g_assert_true(gres);

  // const char *path="/com/un1gfn/ck3fm7";
  // g_print("%s\n", path);
  // char **cs=g_resource_enumerate_children(gres, path, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL); g_assert_true(cs);
  // for(char **c=cs; *c; ++c)
  //   g_print("%s/%s\n", path, *c);
  // g_strfreev(cs); cs=NULL;

  // gsize size=0;
  // guint32 flags=0;
  // g_assert_true(g_resource_get_info(gres, "/com/un1gfn/ck3fm7/adwible.ui.ui", G_RESOURCE_LOOKUP_FLAGS_NONE, &size, &flags, NULL));
  // g_assert_true(0==flags);
  // g_print("%zu bytes\n", size);

  // binary dump
  // g_resource_lookup_data()

  g_resources_register(gres);
  builder=gtk_builder_new_from_resource("/com/un1gfn/ck3fm7/adwible.ui"); g_assert_true(builder);
  css_row_dark=gtk_css_provider_new(); g_assert_true(css_row_dark); gtk_css_provider_load_from_resource(css_row_dark, "/com/un1gfn/ck3fm7/row-dark.css");
  css_row_light=gtk_css_provider_new(); g_assert_true(css_row_light); gtk_css_provider_load_from_resource(css_row_light, "/com/un1gfn/ck3fm7/row-light.css");
  // css_flowbox=gtk_css_provider_new(); g_assert_true(css_flowbox); gtk_css_provider_load_from_resource(css_flowbox, "/com/un1gfn/ck3fm7/flowbox.css");

}

void ui_unregister_gres(){
  g_object_unref(G_OBJECT(css_row_dark)); css_row_dark=NULL;
  g_object_unref(G_OBJECT(css_row_light)); css_row_light=NULL;
  g_object_unref(G_OBJECT(builder)); builder=NULL;
  g_resources_unregister(gres); gres=NULL;
  // g_resource_unref(gres); // double free // valgrind "Invalid read of size 4"
}
