#pragma once
#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <string.h>


void LoadCss(void){
  GtkCssProvider *css_provider = gtk_css_provider_new();
  GError *error = NULL;
  gtk_css_provider_load_from_file(css_provider, g_file_new_for_path("interface.css"), &error);

  if (error != NULL) {
      g_printerr("Error loading CSS file: %s\n", error->message);
      g_error_free(error);
  } else {
      GdkScreen *screen = gdk_screen_get_default();
      gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  }
  g_object_unref(css_provider);
}

/*typedef struct Createpassword{
  GtkWidget *button, *password_entry, *password_reentry, *stack;
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
    gtk_stack_set_visible_child_name(GTK_STACK(createpassword->stack), "login");
    //free(createpassword);
    return;
  }
  else{
    g_print("%s\n","passwords are different");
  }
}

GtkWidget *CreatePassword (GtkWidget *stack, GtkStyleContext *style){
  CREATEPASSWORD *createpassword = malloc(sizeof(CREATEPASSWORD));

  GtkWidget *grid = gtk_grid_new();
  createpassword->stack = stack;
  
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
  //free(createpassword);
  return grid;
}*/


GtkWidget *WriteSpace(GtkWidget *stack, char *mode, char *filename){
  GtkWidget *grid;
  grid = gtk_grid_new();
  if(strcmp(mode, "new_file") == 0){
    GtkWidget *button;
    button = gtk_button_new_with_label("new file mode");
    gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);
  }
  else if(strcmp(mode, "open_file") == 0){
    GtkWidget *button;

    button = gtk_button_new_with_label("open file mode");
    gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);
  }
  return grid;
}

typedef struct Mainmenu{
  GtkWidget *window, *stack, *entry;
  GtkStyleContext *style;
}MAINMENU;

static void Settings (GtkWidget *parent_window, GtkStyleContext *style){
  GtkWidget *window_popup;
  GtkWidget *button;
  GtkWidget *grid;

  grid = gtk_grid_new();

  window_popup = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_container_set_border_width (GTK_CONTAINER (window_popup), 300);
  gtk_container_add(GTK_CONTAINER(window_popup), grid);
  gtk_window_set_transient_for(GTK_WINDOW(window_popup), GTK_WINDOW(parent_window));
  gtk_window_set_position(GTK_WINDOW(window_popup), GTK_WIN_POS_CENTER_ON_PARENT);

  style = gtk_widget_get_style_context(window_popup);
  gtk_style_context_add_class(style, "window-popup");

  button = gtk_button_new_with_label("test");
  gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);

  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window_popup);

  gtk_widget_show_all(window_popup);
}

static void ButtonClickedMainMenu(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;

  Settings(mainmenu->window, mainmenu->style);
  //free(mainmenu);
}

static void ButtonClickedNewEntry(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;
  gtk_stack_set_visible_child_name(GTK_STACK(mainmenu->stack), "writespace_newfile");
}

static void ButtonClickedOpenEntry(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;
  GtkWidget *dialog;
  gint response;

  dialog = gtk_file_chooser_dialog_new("choose an entry", GTK_WINDOW(mainmenu->window), GTK_FILE_CHOOSER_ACTION_OPEN,
  "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  response = gtk_dialog_run(GTK_DIALOG(dialog));
  char *filename;
  if (response == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);

    g_print("Selected file: %s\n", filename);
  }
  gtk_widget_destroy(dialog);

  GtkWidget *write_space_page_open = WriteSpace(mainmenu->stack, "open_file", filename);
  gtk_stack_add_named(GTK_STACK(mainmenu->stack), write_space_page_open, "writespace_openfile");
  gtk_stack_set_visible_child_name(GTK_STACK(mainmenu->stack), "writespace_openfile");
  g_free(filename);
}

GtkWidget *MainMenu (GtkWidget *stack, GtkWidget *window, GtkStyleContext *style){
  MAINMENU *mainmenu = malloc(sizeof(MAINMENU));
  GtkWidget *image;
  GtkWidget *button;
  GtkWidget *menu;
  GtkWidget *menu_item;
  //GtkWidget *sidebar;

  GtkWidget *grid = gtk_grid_new();
  mainmenu->stack = stack;
  mainmenu->window = window;
  mainmenu->style = style;

  /*sidebar = gtk_stack_sidebar_new();
  gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
  gtk_stack_add_named(GTK_STACK(stack), sidebar, "sidebar");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "sidebar");

  style = gtk_widget_get_style_context(sidebar);
  gtk_style_context_add_class(style, "sidebar");*/

  menu = gtk_menu_new();
  button = gtk_menu_button_new();
  //gtk_widget_set_valign(button, GTK_ALIGN_START);
  gtk_menu_button_set_direction(GTK_MENU_BUTTON(button), GTK_ARROW_RIGHT);
  gtk_grid_attach(GTK_GRID(grid), button, 0,0,4,1);

  menu_item = gtk_menu_item_new_with_label("new entry");
  gtk_menu_attach(GTK_MENU(menu), menu_item, 1,2,1,2);
  gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), menu);

  g_signal_connect(menu_item, "activate" ,G_CALLBACK(ButtonClickedNewEntry), mainmenu);

  menu_item = gtk_menu_item_new_with_label("open entry");
  gtk_menu_attach(GTK_MENU(menu), menu_item, 1,3,2,3);
  gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), menu);

  g_signal_connect(menu_item, "activate" ,G_CALLBACK(ButtonClickedOpenEntry), mainmenu);

  image = gtk_image_new_from_file("assets/gear.png");
  button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), image);
  gtk_grid_attach(GTK_GRID(grid), button, 0,1,4,4);

  gtk_widget_show_all(menu);
  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickedMainMenu), mainmenu);
  //free(mainmenu);
  return grid;
}


/*typedef struct Loginsttruct{
  GtkWidget *button, *password_entry, *stack;
  char password[100];
}LOGIN;

static void ButtonClickedLogin (GtkWidget *widget, gpointer data){
  LOGIN *login = data;
  const gchar *entry = gtk_entry_get_text(GTK_ENTRY(login->password_entry));

  FILE *password_file;
  password_file = fopen("password.txt", "r+");
  fgets(login->password, 100, password_file);
  fclose(password_file);

  const gchar *password = login->password;
  g_print("%s\n", entry);
  g_print("%s\n", password);
  if(strcmp(entry, password) == 0){
    g_print("%s\n","password is correct");
    gtk_stack_set_visible_child_name(GTK_STACK(login->stack), "mainmenu");
    //free(login);
  }
  else{
    g_print("%s\n","password is incorrect");
  }
}

GtkWidget *Login (GtkWidget *stack, GtkStyleContext *style){
  LOGIN *login = malloc(sizeof(LOGIN));

  GtkWidget *grid = gtk_grid_new();
  login->stack = stack;

  login->password_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), login->password_entry, 0,0,1,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(login->password_entry), "Enter password");
  gtk_entry_set_visibility(GTK_ENTRY(login->password_entry), FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(login->password_entry), '*');

  login->button = gtk_button_new_with_label("Enter");
  gtk_grid_attach(GTK_GRID(grid), login->button, 1,0,1,1);

  g_signal_connect(login->button, "clicked", G_CALLBACK(ButtonClickedLogin), login);
  //free(login);
  return grid;
}*/

#endif