#include <gtk/gtk.h>
#include "backend.h"

static void Actions (GtkApplication *app, gpointer user_data){
  GtkWidget *window;
  GtkWidget *grid;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Journal.demo");
  gtk_container_set_border_width (GTK_CONTAINER (window), 100);

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  struct stat buffer;
  if (stat ("password.txt", &buffer) != 0) {
    CreatePassword(grid);
  }
  Login(grid);
  gtk_widget_show_all(window);
}

int main (int argc, char **argv){
  GtkApplication *app;
  int status;

  app = gtk_application_new (NULL, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (Actions), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
