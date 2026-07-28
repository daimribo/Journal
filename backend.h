#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

static void print_from_text (GtkEntry *text_entry, gpointer user_data){
  g_print("%s\n",gtk_entry_get_text(text_entry));
}
typedef struct Createpassword{
  GtkWidget * password_entry, *password_reentry;
}CREATEPASSWORD;

const void ButtonClickCreatePassword(GtkWidget *widget, gpointer data){
  CREATEPASSWORD *createpassword = data;
  const gchar * entry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_entry));
  const gchar * reentry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_reentry));
  g_print("%s\n", entry);
  g_print("%s\n", reentry);
  if(strcmp(entry, reentry) == 0){
    g_print("%s\n","password is the same");
  }
  else{
    g_print("%s\n","passwords are different");
  }
}

static void CreatePassword (GtkWidget *window){
  GtkWidget *password_entry;
  GtkWidget *password_reentry;
  GtkWidget *grid;
  GtkWidget *button;

  CREATEPASSWORD *createpassword = malloc(sizeof(CREATEPASSWORD));

  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);
  
  button = gtk_button_new_with_label("Create Password");
  gtk_grid_attach(GTK_GRID(grid), button, 1,3,1,1);

  createpassword->password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), createpassword->password_entry, 0,0,3,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(createpassword->password_entry), "New password");
  gtk_entry_set_visibility(GTK_ENTRY(createpassword->password_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(createpassword->password_entry), '*');

  createpassword->password_reentry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), createpassword->password_reentry, 0,2,3,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(createpassword->password_reentry), "Reenter password");
  gtk_entry_set_visibility(GTK_ENTRY(createpassword->password_reentry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(createpassword->password_reentry), '*');

  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickCreatePassword), createpassword);
}
static void Actions (GtkApplication *app, gpointer user_data){
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Journal.demo");
  gtk_container_set_border_width (GTK_CONTAINER (window), 100);

  struct stat buffer;
  if (stat ("password.txt", &buffer) == 0) {
    printf("password exists");
  } else {
    printf("password doesnt exist");
    CreatePassword(window);
  }
  /*text_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), text_entry, 0,0,1,5);
  gtk_entry_set_visibility(GTK_ENTRY(text_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(text_entry), '*');*/

  /*button = gtk_button_new_with_label("enter");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_from_text), text_entry);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);*/
  gtk_widget_show_all(window);
}


#endif