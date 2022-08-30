#include <gtk/gtk.h>

// GtkScrolledWindow
GObject *sw=NULL;

gboolean f(gpointer user_data){

  g_assert_true(sw);
  GtkAdjustment *const gadj=gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(sw));
  g_print("%lf\n", gtk_adjustment_get_upper(gadj));
  // gtk_adjustment_set_value(gadj, 1169.0f);
  // gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(scrolledwin_tanakh), gadj);

  return FALSE; // not called again

}

int main(){

  gtk_init();

  GtkBuilder *builder=NULL; // GtkBuilder
  GObject *w=NULL; // GtkWindow

  builder=gtk_builder_new_from_file("/home/darren/adwible/demo.ui"); g_assert_true(builder);

  sw=gtk_builder_get_object(builder, "c32p5s");

  w=gtk_builder_get_object(builder, "z9vmdc");
  gtk_window_set_default_size(GTK_WINDOW(w), 360, 600);
  gtk_window_present(GTK_WINDOW(w));

  /*guint _=*/g_idle_add(f, NULL);

  // for(;;){}
  while (g_list_model_get_n_items (gtk_window_get_toplevels ()) > 0)
      g_main_context_iteration (NULL, TRUE);

  return 0;

}
