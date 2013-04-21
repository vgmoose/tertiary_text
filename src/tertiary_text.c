

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x71, 0xBB, 0xDC, 0xBD, 0xED, 0xB1, 0x44, 0xAD, 0xBA, 0x39, 0x4D, 0xB5, 0xBD, 0x5E, 0xE5, 0x69 }
PBL_APP_INFO_SIMPLE(MY_UUID, "Tertiary Text", "VGMoose & CCiollar", 1 /* App version */);


Window window;

TextLayer textLayer;


char master[] = "abcdefghijklm nopqrstuvwxyz";

char text_buffer[] = "                             ";
int pos = 1;
int top, end, size;

void next()
{
    top = 0;
    end = 26;
    size = 27;
}

// Modify these common button handlers

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
    
    if (size > 3)
    {
        size /= 3;
        end -= 2*size;
    }
    else
    {
        text_buffer[pos++] = master[top];
        text_layer_set_text(&textLayer, text_buffer);
        next();
    }
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
    
    if (size > 3)
    {
        size /= 3;
        end -= size;
        top += size;
    }
    else
    {
        text_buffer[pos++] = master[top+1];
        text_layer_set_text(&textLayer, text_buffer);
        next();
    }
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    if (size > 3)
    {
        size /= 3;
        top += 2*size;
    }
    else
    {
        text_buffer[pos++] = master[top+2];
        text_layer_set_text(&textLayer, text_buffer);
        next();
    }
    
    
}


void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

}


// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}


// Standard app initialisation

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Text Input");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&FONT_DEMO_RESOURCES);

  text_layer_init(&textLayer, window.layer.frame);
  text_layer_set_text(&textLayer, text_buffer);
  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &textLayer.layer);

  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
    next();
  app_event_loop(params, &handlers);
}
