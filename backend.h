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

GtkWidget *WriteSpace(char *filename){
  g_print("WriteSpace Called!!");
  
  GtkWidget *grid;
  grid = gtk_grid_new();
  GtkWidget *button;
  button = gtk_button_new_with_label("new file mode");
  gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);
  gtk_widget_show_all(grid);
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
  GtkStack *stack = GTK_STACK(data);

  GtkWidget *write_space_page_new = WriteSpace(NULL);
  gtk_stack_add_named(GTK_STACK(stack), write_space_page_new, "writespace_newfile");
  gtk_widget_show(write_space_page_new);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "writespace_newfile");
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
  GtkWidget *write_space_page_open = WriteSpace(filename);
  gtk_stack_add_named(GTK_STACK(mainmenu->stack), write_space_page_open, "writespace_open");
  gtk_widget_show(write_space_page_open);
  gtk_stack_set_visible_child(GTK_STACK(mainmenu->stack), write_space_page_open);

  gtk_widget_destroy(dialog);
}

GtkWidget *MainMenu (GtkWidget *stack, GtkWidget *window, GtkStyleContext *style){
  MAINMENU *mainmenu = malloc(sizeof(MAINMENU));
  GtkWidget *image;
  GtkWidget *button;
  GtkWidget *menu;
  GtkWidget *menu_item;

  GtkWidget *grid = gtk_grid_new();
  mainmenu->stack = stack;
  mainmenu->window = window;
  mainmenu->style = style;

  menu = gtk_menu_new();
  button = gtk_menu_button_new();
  //gtk_widget_set_valign(button, GTK_ALIGN_START);
  gtk_menu_button_set_direction(GTK_MENU_BUTTON(button), GTK_ARROW_RIGHT);
  gtk_grid_attach(GTK_GRID(grid), button, 0,0,4,1);

  menu_item = gtk_menu_item_new_with_label("new entry");
  gtk_menu_attach(GTK_MENU(menu), menu_item, 1,2,1,2);
  gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), menu);

  g_signal_connect(menu_item, "activate" ,G_CALLBACK(ButtonClickedNewEntry), stack);

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

  return grid;
}

#endif