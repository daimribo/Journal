#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <string.h>

typedef struct Createpassword{
  GtkWidget *button, *password_entry, *password_reentry;
}CREATEPASSWORD;

const void ButtonClickCreatePassword (GtkWidget *widget, gpointer data){
  CREATEPASSWORD *createpassword = data;
  const gchar *entry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_entry));
  const gchar *reentry = gtk_entry_get_text(GTK_ENTRY(createpassword->password_reentry));
  if(strcmp(entry, reentry) == 0){
      
    FILE *password_file;
    password_file = fopen("password.txt", "w+");
    fprintf(password_file, "%s", entry);
    fclose(password_file);
    g_print("%s\n","password created successfully");
    gtk_widget_destroy(createpassword->password_entry);
    gtk_widget_destroy(createpassword->password_reentry);
    gtk_widget_destroy(createpassword->button);
    free(createpassword);
    return;
  }
  else{
    g_print("%s\n","passwords are different");
  }
}

static void CreatePassword (GtkWidget *grid){
  CREATEPASSWORD *createpassword = malloc(sizeof(CREATEPASSWORD));
  
  createpassword->button = gtk_button_new_with_label("Create Password");
  gtk_grid_attach(GTK_GRID(grid), createpassword->button, 1,3,1,1);

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

  g_signal_connect(createpassword->button, "clicked", G_CALLBACK(ButtonClickCreatePassword), createpassword);
  return;
}


typedef struct Mainmenu{
  GtkWidget *button, *grid, *window;
}MAINMENU;

static void Settings (GtkWidget *parent_window){
  g_print("Settings called");
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *grid;

  grid = gtk_grid_new();
  window = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_container_set_border_width (GTK_CONTAINER (window), 100);
  button = gtk_button_new_with_label("test");
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent_window));
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);
  gtk_widget_show_all(window);
  //gtk_window_list_toplevels();
}

static void ButtonClickedMainMenu(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;

  //gtk_widget_destroy(mainmenu->button);
  Settings(mainmenu->window);
}

static void MainMenu (GtkWidget *grid, GtkWidget *window){
  printf("nahhh");
  MAINMENU *mainmenu = malloc(sizeof(MAINMENU));
  GtkWidget *image;

  mainmenu->window = window;
  mainmenu->grid = grid;
  image = gtk_image_new_from_file("assets/gear.png");
  mainmenu->button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(mainmenu->button), image);
  gtk_grid_attach(GTK_GRID(grid), mainmenu->button, 0,0,5,5);
  g_signal_connect(mainmenu->button, "clicked", G_CALLBACK(ButtonClickedMainMenu), mainmenu);
}


typedef struct Loginsttruct{
  GtkWidget *button, *password_entry, *window, *grid;
  char password[100];
  //int *log_in_success;
}LOGIN;

static void ButtonClickedLogin (GtkWidget *widget, gpointer data){
  LOGIN *login = data;
  const gchar *entry = gtk_entry_get_text(GTK_ENTRY(login->password_entry));
  //int *log_in_success = 0;

  FILE *password_file;
  password_file = fopen("password.txt", "r+");
  fgets(login->password, 100, password_file);
  fclose(password_file);

  const gchar *password = login->password;
  g_print("%s\n", entry);
  g_print("%s\n", password);
  if(strcmp(entry, password) == 0){
    g_print("%s\n","password is correct");
    gtk_widget_destroy(login->password_entry);
    gtk_widget_destroy(login->button);
    //MainMenu(login->grid, login->window);
    //*log_in_success = 1;
  }
  else{
    g_print("%s\n","password is incorrect");
  }
}

void Login (GtkWidget *grid, GtkWidget *window){
  LOGIN *login = malloc(sizeof(LOGIN));

  //int *log_in_success;
  //login->log_in_success = log_in_success;
  login->grid = grid;
  login->window = window;
  login->password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), login->password_entry, 0,0,1,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(login->password_entry), "Enter password");
  gtk_entry_set_visibility(GTK_ENTRY(login->password_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(login->password_entry), '*');

  login->button = gtk_button_new_with_label("Enter");
  gtk_grid_attach(GTK_GRID(grid), login->button, 1,0,1,1);

  g_signal_connect(login->button, "clicked", G_CALLBACK(ButtonClickedLogin), login);
  //return login->log_in_success;
}
#endif