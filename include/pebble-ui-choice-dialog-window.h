#pragma once

#include <pebble.h>

#define CHOICE_DIALOG_WINDOW_MARGIN 10

typedef struct UIChoiceDialogWindow {
  // View
  Window* window;
  Layer* background_layer;
  TextLayer* message_layer;
  BitmapLayer* icon_layer;
  ActionBarLayer* action_bar_layer;

  // Model
  bool loaded;
  GColor background_color;
  GColor message_color;
  GColor action_bar_color;
  GFont message_font;

  GBitmap* body_icon;
  GBitmap* accept_icon;
  GBitmap* reject_icon;

  char* message_text;

  // UIChoiceDialogWindowChoiceHandler* accept;
  // UIChoiceDialogWindowChoiceHandler* reject;
} UIChoiceDialogWindow;

UIChoiceDialogWindow* ui_choice_dialog_window_create(char* message, GBitmap* body_icon, GBitmap* accept_icon, GBitmap* reject_icon);
void ui_choice_dialog_window_destroy(UIChoiceDialogWindow* this);

Window* ui_choice_dialog_window_get_window(UIChoiceDialogWindow* this);

// void ui_choice_dialog_window_set_message_text(UIChoiceDialogWindow* this, char* message);
// void ui_choice_dialog_window_set_message_color(UIChoiceDialogWindow* this, GColor message_color);
// void ui_choice_dialog_window_set_message_font(UIChoiceDialogWindow* this, GFont message_font);
// void ui_choice_dialog_window_set_body_icon(UIChoiceDialogWindow* this, GBitmap* body_icon);
// void ui_choice_dialog_window_set_accept_icon(UIChoiceDialogWindow* this, GBitmap* accept_icon);
// void ui_choice_dialog_window_set_reject_icon(UIChoiceDialogWindow* this, GBitmap* reject_icon);
void ui_choice_dialog_window_set_background_color(UIChoiceDialogWindow* this, GColor background_color);
void ui_choice_dialog_window_set_action_bar_color(UIChoiceDialogWindow* this, GColor action_bar_color);
