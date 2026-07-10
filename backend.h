#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>

static void print_from_text (GtkEntry *text_entry, gpointer user_data){
  g_print("%s\n",gtk_entry_get_text(text_entry));
}
static void actions (GtkApplication *app, gpointer user_data){
  GtkWidget *window;
  GtkWidget *text_entry;
  GtkWidget *grid;
  GtkWidget *button;

  grid = gtk_grid_new();
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Journal.demo");
  gtk_window_set_default_size(GTK_WINDOW (window), 500,500);
  gtk_container_add(GTK_CONTAINER(window), grid);

  text_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), text_entry, 0,0,5,5);

  button = gtk_button_new_with_label("enter");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_from_text), text_entry);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 0, 1, 1);
  gtk_widget_show_all(window);
}


#endif