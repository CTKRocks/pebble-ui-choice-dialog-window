#include <pebble.h>
#include "./ui-choice-dialog-window.h"

// Private Methods
static void prv_choice_dialog_window_update_ui(UIChoiceDialogWindow* this) {
  if (!this->loaded) return;

  Layer* window_layer = window_get_root_layer(this->window);
  const GRect window_bounds = layer_get_bounds(window_layer);

  const GRect icon_bounds = gbitmap_get_bounds(this->body_icon);

  text_layer_set_text(this->message_layer, this->message_text);
  text_layer_set_text_color(this->message_layer, this->message_color);
  text_layer_set_font(this->message_layer, this->message_font);

  bitmap_layer_set_bitmap(this->icon_layer, this->body_icon);

  // Configure Action Bar
  action_bar_layer_set_background_color(this->action_bar_layer, this->action_bar_color);
  action_bar_layer_set_icon(this->action_bar_layer, BUTTON_ID_UP, this->accept_icon);
  action_bar_layer_set_icon(this->action_bar_layer, BUTTON_ID_DOWN, this->reject_icon);

  // Put everything in the correct place
  int height =  gbitmap_get_bounds(this->body_icon).size.h +
                CHOICE_DIALOG_WINDOW_MARGIN +
                text_layer_get_content_size(this->message_layer).h;

  layer_set_frame(bitmap_layer_get_layer(this->icon_layer), GRect(
    (window_bounds.size.w - icon_bounds.size.w - PBL_IF_ROUND_ELSE(0, ACTION_BAR_WIDTH)) / 2,
    (window_bounds.size.h - height) / 3,
    icon_bounds.size.w,
    icon_bounds.size.h
  ));

  layer_set_frame(text_layer_get_layer(this->message_layer), GRect(
    ACTION_BAR_WIDTH,
    ((window_bounds.size.h - height) / 3) + icon_bounds.size.h,
    window_bounds.size.w - (2 * ACTION_BAR_WIDTH),
    text_layer_get_content_size(this->message_layer).h
  ));

}

// Event Handlers
static void prv_on_background_layer_update(Layer* layer, GContext* ctx) {
  UIChoiceDialogWindow* this = (UIChoiceDialogWindow*) window_get_user_data(layer_get_window(layer));

  graphics_context_set_fill_color(ctx, this->background_color);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, 0);

  prv_choice_dialog_window_update_ui(this);
}

// Window Handlers
static void prv_window_load(Window* window) {
  UIChoiceDialogWindow* this = (UIChoiceDialogWindow*) window_get_user_data(window);

  Layer* window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  this->background_layer = layer_create(window_bounds);
  layer_set_update_proc(this->background_layer, prv_on_background_layer_update);
  layer_add_child(window_layer, this->background_layer);

  // Create the action_bar_layer
  this->action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(this->action_bar_layer, this->window);

  // Create the message_layer
  const GEdgeInsets message_inset = { .right = ACTION_BAR_WIDTH, .left = ACTION_BAR_WIDTH };
  this->message_layer = text_layer_create(grect_inset(window_bounds, message_inset));
  text_layer_set_text_alignment(this->message_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
  text_layer_set_background_color(this->message_layer, GColorClear);
  layer_add_child(this->background_layer, text_layer_get_layer(this->message_layer));

  // Create the icon_layer
  this->icon_layer = bitmap_layer_create(window_bounds);
  bitmap_layer_set_compositing_mode(this->icon_layer, GCompOpSet);
  layer_add_child(this->background_layer, bitmap_layer_get_layer(this->icon_layer));

  this->loaded = true;

  prv_choice_dialog_window_update_ui(this);
}

static void prv_window_unload(Window* window) {
  UIChoiceDialogWindow* this = window_get_user_data(window);

  // Destroy all the layers
  action_bar_layer_destroy(this->action_bar_layer);
  layer_destroy(this->background_layer);
  text_layer_destroy(this->message_layer);
  bitmap_layer_destroy(this->icon_layer);

  // Update model
  this->loaded = false;
}

// Public Methods
UIChoiceDialogWindow* ui_choice_dialog_window_create(char* message, GBitmap* body_icon, GBitmap* accept_icon, GBitmap* reject_icon) {
  UIChoiceDialogWindow* this = malloc(sizeof(UIChoiceDialogWindow));

  Window* window = window_create();
  window_set_user_data(window, this);

  window_set_window_handlers(window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });

  this->window = window;
  this->loaded = false;
  this->accept_icon = accept_icon;
  this->reject_icon = reject_icon;
  this->action_bar_color = GColorBlack;
  this->background_color = GColorClear;

  this->message_text = message;
  this->message_color = GColorBlack;
  this->message_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);

  this->body_icon = body_icon;

  return this;
}

void ui_choice_dialog_window_destroy(UIChoiceDialogWindow* this) {
  if (this->window) window_destroy(this->window);

  if (this) free(this);
  this = NULL;
}

Window* ui_choice_dialog_window_get_window(UIChoiceDialogWindow* this) {
  return this->window;
}

void ui_choice_dialog_window_set_background_color(UIChoiceDialogWindow* this, GColor background_color) {
  this->background_color = background_color;
  if (this->loaded) layer_mark_dirty(this->background_layer);
}

void ui_choice_dialog_window_set_action_bar_color(UIChoiceDialogWindow* this, GColor action_bar_color) {
  this->action_bar_color = action_bar_color;
  if (this->loaded) layer_mark_dirty(this->background_layer);
}
