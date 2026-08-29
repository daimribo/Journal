#pragma once
#ifndef BACKEND_H
#define BACKEND_H
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


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


typedef struct{
  GtkWidget *entry, *window_popup, *textview;
  FILE *file;
}SAVE;

static void ButtonClickedSetNewEntryName(GtkWidget *widget, gpointer data){
  SAVE *save = data;
  const gchar *tempfilename = gtk_entry_get_text(GTK_ENTRY(save->entry));
  gchar *filename = g_strdup_printf("entries/%s.txt", tempfilename);
  save->file = fopen(filename, "w+");
  gtk_widget_destroy(save->window_popup);
}

static void SetNewEntryName(GtkStyleContext *style, GtkWidget *window, SAVE *save){
  GtkWidget *button;
  GtkWidget *grid;

  grid = gtk_grid_new();

  save->window_popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (save->window_popup), 100);
  gtk_container_add(GTK_CONTAINER(save->window_popup), grid);
  gtk_window_set_transient_for(GTK_WINDOW(save->window_popup), GTK_WINDOW(window));
  gtk_window_set_position(GTK_WINDOW(save->window_popup), GTK_WIN_POS_CENTER_ON_PARENT);

  style = gtk_widget_get_style_context(save->window_popup);
  gtk_style_context_add_class(style, "window-popup");

  save->entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid),save->entry, 2,0,1,1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(save->entry), "Entry Name");

  button = gtk_button_new_with_label("create");
  gtk_grid_attach(GTK_GRID(grid), button, 2,2,1,1);

  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickedSetNewEntryName), save);

  gtk_widget_show_all(save->window_popup);
}

static void SaveButton(GtkWidget *widget, gpointer data){
  SAVE *save = data;

  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  gchar *text;
  
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(save->textview));
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  fseek(save->file, 0, SEEK_SET);
  ftruncate(fileno(save->file), 0);

  fprintf(save->file, "%s", text);
  fflush(save->file);
}

static void MainMenuButton(GtkWidget *widget, gpointer data){
  GtkWidget *stack = data;

  GtkWidget *openfile_child = gtk_stack_get_child_by_name(GTK_STACK(stack), "writespace_open");
  if(openfile_child != NULL){
    gtk_container_remove(GTK_CONTAINER(stack), openfile_child);
  }
  GtkWidget *newfile_child = gtk_stack_get_child_by_name(GTK_STACK(stack), "writespace_newfile");
  if(newfile_child != NULL){
    gtk_container_remove(GTK_CONTAINER(stack), newfile_child);
  }

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "mainmenu");
}

GtkWidget *WriteSpace(gchar *filename, GtkStyleContext *style, GtkWidget *window, GtkWidget *stack){
  SAVE *save = malloc(sizeof(SAVE));

  GtkWidget *box;
  GtkWidget *header_bar;
  GtkWidget *scrolled_window;
  GtkWidget *button;

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  save->textview = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(save->textview), GTK_WRAP_WORD);
  gtk_widget_set_hexpand(scrolled_window, TRUE);
  gtk_widget_set_vexpand(scrolled_window, TRUE);
  gtk_container_add(GTK_CONTAINER(scrolled_window), save->textview);

  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(save->textview), 30);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(save->textview), 30);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(save->textview), 20);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(save->textview), 20);

  gtk_box_pack_end(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);
  style = gtk_widget_get_style_context(save->textview);
  gtk_style_context_add_class(style, "entry-box");

  if(strcmp(filename, "") == 0){
    SetNewEntryName(style, window, save);
  }
  else{
    GtkTextBuffer *buffer;
    char file_content[100];
    save->file = fopen(filename, "r+");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(save->textview));

    gtk_text_buffer_set_text(buffer, "", -1); //clears the buffer
    while(fgets(file_content, 100, save->file)) {
     gtk_text_buffer_insert_at_cursor(buffer, file_content, -1); 
    }
  }

  header_bar = gtk_header_bar_new();

  button = gtk_button_new_with_label("main menu");
  g_signal_connect(button, "clicked", G_CALLBACK(MainMenuButton), stack);
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), button);
  
  button = gtk_button_new_with_label("save");
  g_signal_connect(button, "clicked", G_CALLBACK(SaveButton), save);
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), button);

  gtk_box_pack_start(GTK_BOX(box), header_bar, FALSE, TRUE, 10);

  gtk_widget_show_all(box);
  //fclose(save->file);
  return box;
}

typedef struct{
  GtkWidget *window, *stack;
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

static void ButtonClickedSettings(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;

  Settings(mainmenu->window, mainmenu->style);
}

static void ButtonClickedNewEntry(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;

  GtkWidget *write_space_page_new = WriteSpace("", mainmenu->style, mainmenu->window, mainmenu->stack);
  gtk_stack_add_named(GTK_STACK(mainmenu->stack), write_space_page_new, "writespace_newfile");
  gtk_widget_show(write_space_page_new);
  gtk_stack_set_visible_child_name(GTK_STACK(mainmenu->stack), "writespace_newfile");
}

static void ButtonClickedOpenEntry(GtkWidget *widget, gpointer data){
  MAINMENU *mainmenu = data;
  GtkWidget *dialog;
  gint response;

  dialog = gtk_file_chooser_dialog_new("choose an entry", GTK_WINDOW(mainmenu->window), GTK_FILE_CHOOSER_ACTION_OPEN,
  "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "entries");

  response = gtk_dialog_run(GTK_DIALOG(dialog));
  char *filename;
  if (response == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);

    g_print("Selected file: %s\n", filename);
  }
  else{
    gtk_widget_destroy(dialog);
    return;
  }
  GtkWidget *write_space_page_open = WriteSpace(filename, mainmenu->style, mainmenu->window, mainmenu->stack);
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
  g_signal_connect(button, "clicked", G_CALLBACK(ButtonClickedSettings), mainmenu);

  return grid;
}

#endif