#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

static void print_from_text (GtkEntry *text_entry, gpointer user_data){
  g_print("%s\n",gtk_entry_get_text(text_entry));
}

typedef struct Loginsttruct{
  GtkWidget* password_entry, *grid;
  char password[100];
}LOGIN;

static void ButtonClickedLogin(GtkWidget *widget, gpointer data){
  LOGIN *login = data;
  const gchar *entry = gtk_entry_get_text(GTK_ENTRY(login->password_entry));
  const gchar *password = login->password;
  g_print("%s\n", entry);
  g_print("%s\n", password);
  if(strcmp(entry, password) == 0){
    g_print("%s\n","password is correct");
    gtk_widget_destroy(login->grid);
  }
  else{
    g_print("%s\n","password is incorrect");
  }
}

static void Login (GtkWidget* window){
  g_print("login called\n");
  GtkWidget *button;

  LOGIN *login = malloc(sizeof(LOGIN));

  login->grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), login->grid);

  login->password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(login->grid), login->password_entry, 0,0,1,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(login->password_entry), "Enter password");
  gtk_entry_set_visibility(GTK_ENTRY(login->password_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(login->password_entry), '*');

  button = gtk_button_new_with_label("Enter");
  gtk_grid_attach(GTK_GRID(login->grid), button, 1,0,1,1);

  FILE *password_file;
  password_file = fopen("password.txt", "r");
  fgets(login->password, 100, password_file);

  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickedLogin), login);
}

typedef struct Createpassword{
  GtkWidget *grid ,*password_entry, *password_reentry;
}CREATEPASSWORD;

const void ButtonClickCreatePassword(GtkWidget *widget, gpointer data){
  CREATEPASSWORD *createpassword = data;
  const gchar * entry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_entry));
  const gchar * reentry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_reentry));
  if(strcmp(entry, reentry) == 0){
    FILE *password_file;
    password_file = fopen("password.txt", "w");
    fprintf(password_file, entry);
    fclose(password_file);
    g_print("%s\n","password created successfully");
    gtk_widget_destroy(createpassword->grid);
    return;
  }
  else{
    g_print("%s\n","passwords are different");
  }
}

static void CreatePassword (GtkWidget *window){
  GtkWidget *button;

  CREATEPASSWORD *createpassword = malloc(sizeof(CREATEPASSWORD));

  createpassword->grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), createpassword->grid);
  
  button = gtk_button_new_with_label("Create Password");
  gtk_grid_attach(GTK_GRID(createpassword->grid), button, 1,3,1,1);

  createpassword->password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(createpassword->grid), createpassword->password_entry, 0,0,3,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(createpassword->password_entry), "New password");
  gtk_entry_set_visibility(GTK_ENTRY(createpassword->password_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(createpassword->password_entry), '*');

  createpassword->password_reentry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(createpassword->grid), createpassword->password_reentry, 0,2,3,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(createpassword->password_reentry), "Reenter password");
  gtk_entry_set_visibility(GTK_ENTRY(createpassword->password_reentry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(createpassword->password_reentry), '*');

  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickCreatePassword), createpassword);
  return;
}

#endif