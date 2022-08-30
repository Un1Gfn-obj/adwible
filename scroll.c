#include <gtk/gtk.h>
#include <glib.h>

#include "scroll.h"

// GtkScrolledWindow
static GObject *sw=NULL;

static gboolean f(const gpointer garbage){

  g_assert_true(sw);
  g_assert_true(!garbage);

  GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(sw));
  g_print("max %lf\n", gtk_adjustment_get_upper(gadj));
  gtk_adjustment_set_value(gadj, 1169.0f);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(sw), gadj);

  return FALSE; // execute once only

}

void scroll_init(GObject *const o){
  g_assert_true(!sw);
  sw=o;
}

void scroll_dispatch(){
  /*const guint _=*/g_idle_add(f, NULL);
}
