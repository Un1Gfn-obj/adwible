// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplicaiton
#include <gio/gio.h> // GResource
#include <sys/utsname.h> // uname

#include "scroll.h"
#include "ck3fm7.h" // j72qkd_get_resource
#include "bc.h" // biblical canon
#include "bs.h" // bitset

#define MAXCHAP 150

static char lb[1+150][3+1]={0};
static gboolean dark=FALSE;
static GResource *gres=NULL;
static GtkCssProvider *css_row_dark=NULL;
static GtkCssProvider *css_row_light=NULL;
static GtkBuilder *builder=NULL;

static const char *autoexpand=NULL;
static gboolean do_scroll=FALSE;

typedef struct {
  glong n; // current chapter
  const char *t; // current book group
} cb_toggle_t;

static void cb_togglebutton(GtkToggleButton* self, gpointer data){
  cb_toggle_t *p=(cb_toggle_t*)data;
  // g_print("%ld\n", p->n);
  g_assert_true(1<=(p->n) && (p->n)<=tanakh.n_total_chapters);
  bs_toggle(bs_tanakh, p->n, gtk_toggle_button_get_active(self));
  // g_print("%s\n", p->t);
  g_assert_true(p->t); autoexpand=p->t;
  // bs_test(bs_tanakh);
}

// try g_signal_connect_closure?
static void cb_togglebutton_destroy(__attribute__((unused)) GtkToggleButton* self, gpointer data){
  g_free(data); data=NULL;
}

// bind to a floating button in GtkOverlay
// show an AdwToast on success
void cb_sync(__attribute__((unused)) GtkButton *self, gpointer _){
  g_assert_true(!_);
  bs_save(bs_tanakh, tanakh.progress);
  if(autoexpand){
    g_message("'%s' saved", tanakh.autoexpand);
    g_assert_true(g_file_set_contents_full(tanakh.autoexpand, autoexpand, -1, G_FILE_SET_CONTENTS_CONSISTENT, 0644, NULL));
  }
}

static gboolean cb_close(GtkWindow *const self, gpointer _){
  g_assert_true(!_);
  gtk_window_destroy(self); // is it necessary?
  cb_sync(NULL, NULL);
  return FALSE;
}

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

  glong cur_chapter=0; _Static_assert(sizeof(gpointer)==sizeof(glong), "");

  GtkWidget *apg=NULL;
  GtkWidget *aer=NULL;

  gchar *contents=NULL;
  gsize length=0;
  GError *e=NULL;

  // GLib.file_get_contents()
  if(g_file_get_contents(testament->autoexpand, &contents, &length, &e)){
    g_assert_true(contents);
    g_assert_true(contents==g_strchomp(contents)); // chomp chug strip
    g_message("'%s' loaded", testament->autoexpand);
  }else{
    g_assert_true(e);
    g_assert_true(G_FILE_ERROR==e->domain);
    g_assert_true(G_FILE_ERROR_NOENT==e->code);
    g_error_free(e); e=NULL;
  }

  for(const bc_group_t *g=testament->groups; 0!=g->n_books; ++g){ // BEGIN LOOP groups in a testament

    // [AdwPreferencesGroup] AdwExpanderRow GtkListBoxRow GtkFlowBox
    apg=adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(apg), g->title);
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(apg), g->description);

    for(const bc_book_t *b=g->books; 0!=b->n_chapters; ++b){ // BEGIN LOOP books in a group

      // AdwPreferencesGroup [AdwExpanderRow] GtkListBoxRow GtkFlowBox
      aer=adw_expander_row_new();
      adw_preferences_row_set_title(ADW_PREFERENCES_ROW(aer), b->title);
      if(b->subtitle)
        adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(aer), b->subtitle);
      adw_preferences_group_add(ADW_PREFERENCES_GROUP(apg), aer);

      // autoexpand
      if(contents){
        // g_message("A.%s.%s.B", contents, b->title);
        if(0==g_strcmp0(b->title, contents)){
          g_message("'%s' expanded", contents);
          adw_expander_row_set_expanded(ADW_EXPANDER_ROW(aer), TRUE);
          scroll_aer=aer;
          scroll_apg=apg;
          do_scroll=TRUE;
        }
      }

      // AdwPreferencesGroup AdwExpanderRow [GtkListBoxRow GtkFlowBox]
      GtkWidget *fb=gtk_flow_box_new();
      gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(fb), 255);
      gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(aer), FALSE);
      { // add_book()
        g_assert_true(1<=b->n_chapters && b->n_chapters<=MAXCHAP);
        for(int i=1; i<=b->n_chapters; ++i){
          GtkWidget *tb=gtk_toggle_button_new_with_label(lb[i]); // GtkToggleButton
          gtk_widget_set_halign(tb, GTK_ALIGN_FILL);
          gtk_widget_set_hexpand(tb, FALSE);
          ++cur_chapter;
          if(bs_get(bs_tanakh, cur_chapter)) // 1/3
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb), TRUE); // 2/3
          cb_toggle_t *const cb_data=g_new0(cb_toggle_t, 1);
          *cb_data=(cb_toggle_t){.n=cur_chapter, .t=b->title};
          g_signal_connect(tb, "toggled", G_CALLBACK(cb_togglebutton), cb_data); // 3/3
          g_signal_connect(tb, "destroy", G_CALLBACK(cb_togglebutton_destroy), cb_data); // 3/3
          gtk_flow_box_append(GTK_FLOW_BOX(fb), tb);
        }
      }
      adw_expander_row_add_row(ADW_EXPANDER_ROW(aer), fb);
      GtkWidget *const row=gtk_widget_get_parent(fb);
      g_assert_true(g_type_check_instance_is_a((gpointer)row, gtk_list_box_row_get_type()));
      click_keep_bg(row);

    } // END LOOP books in a group


    // add a testment to GtkBox in GtkScrolledWindow
    gtk_box_append(box, apg);

  } // END LOOP groups in a testament

  g_free(contents); contents=NULL; length=0;

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
  GObject *const box_tanakh=gtk_builder_get_object(builder, "qgnxl8"); g_assert_true(box_tanakh);
  add_testament(GTK_BOX(box_tanakh), &tanakh);

  GObject *sw=gtk_builder_get_object(builder, "qbtw37"); g_assert_true(sw); scroll_gsw=sw; // GtkScrolledWindow

  GObject *const win=gtk_builder_get_object(builder, "tf2fhx"); g_assert_true(win);
  g_signal_connect(win, "close-request", G_CALLBACK(cb_close), NULL);

  if(!isMobile()){
    g_message("not wt88047, resizing...");
    gtk_window_set_default_size(GTK_WINDOW(win), width, height);
  }
  gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(win));

  gtk_widget_show(GTK_WIDGET(win)); // gtk_window_present(GTK_WINDOW(win));
  if(do_scroll)
    scroll_dispatch();

}

void ui_theme(){
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

// GValue v=G_VALUE_INIT;
// GValue v={0}; g_assert_true(&v==g_value_init(&v, G_TYPE_STRING));
// g_value_set_static_string(&v, g->title); g_assert_true(G_VALUE_HOLDS_STRING(&v));
// g_object_set_property(G_OBJECT(apg), "title", &v);

// static gpointer show_v_adj(const gpointer data){
//   g_assert_true(g_type_check_instance_is_a(data, GTK_TYPE_SCROLLED_WINDOW));

//   sleep(1);
//   GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(data));
//   gtk_adjustment_set_value(gadj, 1169.0f);
//   gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(data), gadj);

//   // for(;;){
//   //   sleep(1);
//   //   GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(data));
//   //   g_print("%lf\n", gtk_adjustment_get_upper(gadj));
//   //   g_print("%lf\n", gtk_adjustment_get_value(gadj));
//   //   g_print("%lf\n", gtk_adjustment_get_lower(gadj));
//   //   g_print("\n");
//   // }

//   // g_thread_exit
//   return NULL;
// }

// const gint64 ll=g_ascii_strtoll(contents, NULL, 10);
// g_assert_true(1<=ll && ll<=testament->n_total_groups);
// if(ll==g-testament->groups+1)
//   adw_expander_row_set_expanded(ADW_EXPANDER_ROW(aer), TRUE);

// if('\n'==contents[length-1])
//   contents[length-1]='\0';
