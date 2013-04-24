

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
TextLayer* bbuttons[] = {buttons1, buttons2, buttons3};

bool menu = false;

//char* threetext[] = {"a-i","j-q","r-z"};
char* btext1[] = {"abc", "def", "ghi"};
char* btext2[] = {"jkl", "m n", "opq"};
char* btext3[] = {"rst", "uvw", "xyz"};
char** btexts[] = {btext1, btext2, btext3};

char set1[3] = "   ";
char set2[3] = "   ";
char set3[3] = "   ";

void drawSides();
void drawMenu();

char caps[] =    "ABCDEFGHIJKLM NOPQRSTUVWXYZ";
char letters[] = "abcdefghijklm nopqrstuvwxyz";
char numsym[] = "1234567890!?-'\"$()&*+%:@/,.";

char* rotate_text[] = {caps, letters, numsym};
void next();

char* master = letters;

char text_buffer[] = "                             ";
int pos = 1;
int top, end, size;

void change_set(int s)
{
    int count = 0;
    master = rotate_text[s];
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            for (int k=0; k<3; k++)
            {
                btexts[i][j][k] = master[count];
                count++;
            }
        }
    }
    menu = false;
    next();
    drawSides();
}

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
    
    if (menu)
    {
        change_set(0);
        return;
    }
    
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
    if (menu)
    {
        change_set(1);
        return;
    }
    
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
    
    if (menu)
    {
        change_set(2);
        return;
    }
    
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

}


void down_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    text_buffer[--pos] = ' ';
    text_layer_set_text(&textLayer, text_buffer);
    next();
    drawSides();

    
}

void set_menu()
{
    menu = true;
    drawMenu();
}

void up_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    set_menu();
    
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

void drawMenu()
{
    text_layer_set_text(&buttons1[1], " ");
    text_layer_set_text(&buttons1[2], " ");
    
    text_layer_set_text(&buttons2[1], " ");
    text_layer_set_text(&buttons2[2], " ");
    
    text_layer_set_text(&buttons3[0], " ");
    text_layer_set_text(&buttons3[2], " ");
    
    text_layer_set_text(&buttons1[0], "CAPS");
    text_layer_set_font(&buttons1[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    
    text_layer_set_text(&buttons2[0], "small");
    text_layer_set_font(&buttons2[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    
    text_layer_set_text(&buttons3[1], "num/sym");
    text_layer_set_font(&buttons3[1], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

}


// This method draws the characters on the right side near the buttons
void drawSides()
{
    if (size==27) // first click (full size)
    {
        // update all 9 labels to their proper values
        for (int h=0; h<3; h++)
        {
            for (int i=0; i<3; i++)
            {
    //            text_layer_init(&buttons1[i], GRect(115, 12*i, 100, 100));
                text_layer_set_text(&bbuttons[h][i], btexts[h][i]);
                text_layer_set_background_color(&bbuttons[h][i], GColorClear);
                text_layer_set_font(&bbuttons[h][i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
                layer_add_child(&window.layer, &bbuttons[h][i].layer);
            }

        }
    }
    else if (size==9)   // second click
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
