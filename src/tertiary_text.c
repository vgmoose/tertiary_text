

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x71, 0xBB, 0xDC, 0xBD, 0xED, 0xB1, 0x44, 0xAD, 0xBA, 0x39, 0x4D, 0xB5, 0xBD, 0x5E, 0xE5, 0x69 }
PBL_APP_INFO_SIMPLE(MY_UUID, "Tertiary Text", "VGMoose & CCiollar", 1 /* App version */);


Window window;

TextLayer textLayer;
TextLayer buttons1[3];
TextLayer buttons2[3];
TextLayer buttons3[3];


//char* threetext[] = {"a-i","j-q","r-z"};
char* btext1[] = {"abc", "def", "ghi"};
char* btext2[] = {"jkl", "m n", "opq"};
char* btext3[] = {"rst", "uvw", "xyz"};
char** btexts[] = {btext1, btext2, btext3};

char set1[3] = "   ";
char set2[3] = "   ";
char set3[3] = "   ";

void drawSides();


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

void getText(int third)
{
//    btext[0] = master[
    
}

void upThing()
{
//    for (int i=0; i<3; i++)
//        text_layer_set_text(&buttons[i], btexts[0][i]);

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
    drawSides();
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
    drawSides();
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
    drawSides();

    
    
}


void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
    
    text_buffer[--pos] = ' ';
    text_layer_set_text(&textLayer, text_buffer);
    next();
    upThing();

}


void down_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    text_buffer[--pos] = ' ';
    text_layer_set_text(&textLayer, text_buffer);
    next();
    upThing();
    drawSides();

    
}

void up_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    text_buffer[--pos] = ' ';
    text_layer_set_text(&textLayer, text_buffer);
    next();
    upThing();
    
}


// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
    config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
    
    config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
    config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler;
}


// Standard app initialisation

void drawSides()
{
    if (size==27)
    {
        for (int i=0; i<3; i++)
        {
//            text_layer_init(&buttons1[i], GRect(115, 12*i, 100, 100));
            text_layer_set_text(&buttons1[i], btexts[0][i]);
            text_layer_set_background_color(&buttons1[i], GColorClear);
            text_layer_set_font(&buttons1[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            layer_add_child(&window.layer, &buttons1[i].layer);
        }
        
        for (int i=0; i<3; i++)
        {
//            text_layer_init(&buttons2[i], GRect(115, 12*i+50, 100, 100));
            text_layer_set_text(&buttons2[i], btexts[1][i]);
            text_layer_set_background_color(&buttons2[i], GColorClear);
            text_layer_set_font(&buttons2[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            layer_add_child(&window.layer, &buttons2[i].layer);
        }
        
        for (int i=0; i<3; i++)
        {
//            text_layer_init(&buttons3[i], GRect(115, 12*i+100, 100, 100));
            text_layer_set_text(&buttons3[i], btexts[2][i]);
            text_layer_set_background_color(&buttons3[i], GColorClear);
            text_layer_set_font(&buttons3[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            layer_add_child(&window.layer, &buttons3[i].layer);
        }
    } else if (size==9)
    {
        text_layer_set_text(&buttons1[1], " ");
        text_layer_set_text(&buttons1[2], " ");
        text_layer_set_text(&buttons2[1], " ");
        text_layer_set_text(&buttons2[2], " ");
        text_layer_set_text(&buttons3[0], " ");
        text_layer_set_text(&buttons3[2], " ");

//            text_layer_init(&buttons1[0], GRect(111, 5, 100, 100));
            text_layer_set_text(&buttons1[0], btexts[top/9][0]);
            text_layer_set_font(&buttons1[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        
        text_layer_set_text(&buttons2[0], btexts[top/9][1]);
        text_layer_set_font(&buttons2[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        
        text_layer_set_text(&buttons3[1], btexts[top/9][2]);
        text_layer_set_font(&buttons3[1], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        
    } else if (size == 3)
    {
        set1[2] = master[top];
        set2[2] = master[top+1];
        set3[2] = master[top+2];
        
        text_layer_set_text(&buttons1[0], set1);
        text_layer_set_text(&buttons2[0], set2);
        text_layer_set_text(&buttons3[1], set3);
//        text_layer_set_font(&buttons1[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
//        text_layer_set_font(&buttons2[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
//        text_layer_set_font(&buttons3[1], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    }
    
}

void initSides()
{
    for (int i = 0; i<3; i++)
    {
        text_layer_init(&buttons1[i], GRect(115, 12*i, 100, 100));
        text_layer_init(&buttons2[i], GRect(115, 12*i+50, 100, 100));
        text_layer_init(&buttons3[i], GRect(115, 12*i+100, 100, 100));
    }

}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Text Input");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&FONT_DEMO_RESOURCES);

  text_layer_init(&textLayer, window.layer.frame);
  text_layer_set_text(&textLayer, text_buffer);
    text_layer_set_background_color(&textLayer, GColorClear);

  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &textLayer.layer);
    
    initSides();
    drawSides();
    
//    doThing();

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
