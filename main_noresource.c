#include <adwaita.h>

static void activate_cb(AdwApplication *app){

  GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app)); g_assert_true(window);

  GtkWidget *label = gtk_label_new ("Hello World"); g_assert_true(label);

  gtk_window_set_title(GTK_WINDOW(window), "Hello");
  // gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), label);
  gtk_window_present(GTK_WINDOW(window));

}

int main(const int argc, char *argv[]){

  g_autoptr(AdwApplication) app = adw_application_new("com.un1gfn.adwible", G_APPLICATION_FLAGS_NONE); g_assert_true(app);

  g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

  return g_application_run(G_APPLICATION(app), argc, argv);

}
