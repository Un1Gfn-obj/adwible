#include <adwaita.h>
// #include <gtk/gtk.h>
// #include <glib.h>

#include "scroll.h"

// GtkScrolledWindow
GObject *scroll_gsw=NULL;

// AdwPreferencesGroup
GtkWidget *scroll_apg=NULL;

// AdwExpanderRow
GtkWidget *scroll_aer=NULL;

static gboolean f(const gpointer garbage){

  g_assert_true(scroll_gsw);
  g_assert_true(!garbage);

  GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll_gsw));
  // g_print("max %lf\n", gtk_adjustment_get_upper(gadj));

  g_assert_true(scroll_aer);
  // const char *const t=adw_preferences_row_get_title(ADW_PREFERENCES_ROW(scroll_aer));
  // g_assert_true(t); g_print("%s\n", t);

  GtkAllocation a={0}; // GtkAllocation==GdkRectangle===cairo_rectangle_t
  gtk_widget_get_allocation(scroll_aer, &a); // relative to the AdwPreferenceGroup it belongs to
  // g_print("rel ");
  // g_print("x (%d) ", a.x);
  // g_print("y (%d) ", a.y);
  // g_print("width (%d) ", a.width);
  // g_print("height (%d) ", a.height);
  // g_print("\n");

  double dest_x=0.0f;
  double dest_y=0.0f;
  g_assert_true(gtk_widget_translate_coordinates(scroll_apg, GTK_WIDGET(scroll_gsw), (double)a.x, (double)a.y, &dest_x, &dest_y));
  // g_print("abs ");
  // g_print("x (%lf) ", dest_x);
  // g_print("y (%lf) ", dest_y);
  // g_print("\n");

  gtk_adjustment_set_value(gadj, dest_y);
  // gtk_adjustment_set_value(gadj, 1169.0f);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(scroll_gsw), gadj);

  return FALSE; // not called again

}

void scroll_dispatch(){
  /*const guint _=*/g_idle_add(f, NULL);
}
