// #include <gtk/gtk.h>
#include <adwaita.h> // AdwApplicaiton
#include <gio/gio.h> // GResource
#include <sys/utsname.h> // uname

#include "ck3fm7.h" // j72qkd_get_resource
#include "bc.h" // biblical canon
#include "bs.h" // bitset

#define MAXCHAP 150

static char lb[1+150][3+1]={0};
static gboolean dark=FALSE;
static GResource *gres=NULL;
static GtkCssProvider *css_row_dark=NULL;
static GtkCssProvider *css_row_light=NULL;
GtkBuilder *builder=NULL;

static GThread *th_scroll=NULL;

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

static void toggle_cb(/*__attribute__((unused)) */GtkToggleButton* self, gpointer user_data){

  // g_assert_true(user_data);
  // const glong n=*((glong*)user_data); g_assert_true(1<=n);
  const glong n=(glong)user_data; g_assert_true(1<=n && n<=tanakh.n_total_chapters);
  // g_message("GtkToggleButton::toggled [%ld]", n);

  bs_toggle(bs_tanakh, n, gtk_toggle_button_get_active(self));
  // g_message("GtkToggleButton::toggled [%ld] ?->%s", n, gtk_toggle_button_get_active(self)?"O":".");
  // bs_test(bs_tanakh);

}

static inline void add_book(GtkWidget *const flowbox, const bc_book_t *const book, glong *const chapter_counter){
  g_assert_true(1<=book->n_chapters && book->n_chapters<=MAXCHAP);
  for(int i=1; i<=book->n_chapters; ++i){
    GtkWidget *tb=gtk_toggle_button_new_with_label(lb[i]); // GtkToggleButton
    gtk_widget_set_halign(tb, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(tb, FALSE);
    ++(*chapter_counter);
    // the following 3 lines must remain in order
    if(bs_get(bs_tanakh, *chapter_counter)) // 1
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tb), TRUE); // 2
    g_signal_connect(tb, "toggled", G_CALLBACK(toggle_cb),(gpointer)(*chapter_counter)); // 3
    gtk_flow_box_append(GTK_FLOW_BOX(flowbox), tb);
  }
}

static inline void add_testament(GtkBox *const box, const bc_testament_t *const testament){

  _Static_assert(sizeof(gpointer)==sizeof(glong));
  glong cur_chapter=0;

  for(const bc_group_t *g=testament->groups; 0!=g->n_books; ++g){

    // [AdwPreferencesGroup] AdwExpanderRow GtkListBoxRow GtkFlowBox
    GtkWidget *apg=adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(apg), g->title);
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(apg), g->description);

    for(const bc_book_t *b=g->books; 0!=b->n_chapters; ++b){

      // AdwPreferencesGroup [AdwExpanderRow] GtkListBoxRow GtkFlowBox
      GtkWidget *aer=adw_expander_row_new();
      adw_preferences_row_set_title(ADW_PREFERENCES_ROW(aer), b->title);
      if(b->subtitle) adw_expander_row_set_subtitle(ADW_EXPANDER_ROW(aer), b->subtitle);
      adw_preferences_group_add(ADW_PREFERENCES_GROUP(apg), aer);

      // autoexpand
      {
        // GLib.file_get_contents()
        gchar *contents=NULL;
        gsize length=0;
        GError *e=NULL;
        if(g_file_get_contents(testament->autoexpand, &contents, &length, &e)){
          g_assert_true(contents);

          // const gint64 ll=g_ascii_strtoll(contents, NULL, 10);
          // g_assert_true(1<=ll && ll<=testament->n_total_groups);
          // if(ll==g-testament->groups+1)
          //   adw_expander_row_set_expanded(ADW_EXPANDER_ROW(aer), TRUE);

          // if('\n'==contents[length-1])
          //   contents[length-1]='\0';

          g_assert_true(contents==g_strchomp(contents)); // chomp chug strip
          if(0==g_strcmp0(b->title, contents)){
            adw_expander_row_set_expanded(ADW_EXPANDER_ROW(aer), TRUE);
          }

          g_free(contents); contents=NULL; length=0;

        }else{
          g_assert_true(e);
          g_assert_true(G_FILE_ERROR==e->domain);
          g_assert_true(G_FILE_ERROR_NOENT==e->code);
          g_error_free(e); e=NULL;
        }
      }

      // AdwPreferencesGroup AdwExpanderRow [GtkListBoxRow GtkFlowBox]
      GtkWidget *fb=gtk_flow_box_new();
      gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(fb), 255);
      gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(aer), FALSE);
      add_book(fb, b, &cur_chapter);
      adw_expander_row_add_row(ADW_EXPANDER_ROW(aer), fb);
      GtkWidget *const row=gtk_widget_get_parent(fb);
      g_assert_true(g_type_check_instance_is_a((gpointer)row, gtk_list_box_row_get_type()));
      click_keep_bg(row);

    }

    // add a testment to GtkBox in GtkScrolledWindow
    gtk_box_append(box, apg);

  }
}

static gboolean cb_close(GtkWindow *const self, __attribute__((unused)) gpointer user_data){

  g_assert_true(th_scroll);
  // g_assert_true(!g_thread_join(th_scroll));
  // th_scroll=NULL;

  // g_message("closed");
  gtk_window_destroy(self); // is it necessary?
  bs_save(bs_tanakh, tanakh.progress);
  return FALSE;

}

static gpointer show_v_adj(const gpointer data){
  g_assert_true(g_type_check_instance_is_a(data, GTK_TYPE_SCROLLED_WINDOW));

  sleep(1);
  GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(data));
  gtk_adjustment_set_value(gadj, 1169.0f);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(data), gadj);

  // for(;;){
  //   sleep(1);
  //   GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(data));
  //   g_print("%lf\n", gtk_adjustment_get_upper(gadj));
  //   g_print("%lf\n", gtk_adjustment_get_value(gadj));
  //   g_print("%lf\n", gtk_adjustment_get_lower(gadj));
  //   g_print("\n");
  // }

  // g_thread_exit
  return NULL;
}

static void cb_scroll(GtkWidget *const self, const gpointer user_data){
  g_assert_true(!user_data);
  th_scroll=g_thread_new("th_scroll", show_v_adj, self); g_assert_true(th_scroll);
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

  GObject *const scrolledwin_tanakh=gtk_builder_get_object(builder, "qbtw37"); g_assert_true(box_tanakh);
  g_signal_connect(scrolledwin_tanakh, "realize", G_CALLBACK(cb_scroll), NULL);
  // g_signal_connect(scrolledwin_tanakh, "map", G_CALLBACK(cb_scroll), NULL);

  GObject *const win=gtk_builder_get_object(builder, "tf2fhx"); g_assert_true(win);
  g_signal_connect(win, "close-request", G_CALLBACK(cb_close), NULL);

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
