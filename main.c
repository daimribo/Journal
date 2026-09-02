#include "backend.h"
#include <gtk/gtk.h>

static void Actions(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *stack;
  GtkStyleContext *style;

  LoadCss();

  if (g_mkdir_with_parents("entries", 0755) != 0) {
    perror("Could not create entries directory");
  }

  stack = gtk_stack_new();

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Journal.demo");
  gtk_window_maximize(GTK_WINDOW(window));

  gtk_container_add(GTK_CONTAINER(window), stack);

  GtkWidget *main_menu_page = MainMenu(stack, window, style);
  gtk_stack_add_named(GTK_STACK(stack), main_menu_page, "mainmenu");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "mainmenu");

  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(Actions), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
