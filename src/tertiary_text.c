#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x71, 0xBB, 0xDC, 0xBD, 0xED, 0xB1, 0x44, 0xAD, 0xBA, 0x39, 0x4D, 0xB5, 0xBD, 0x5E, 0xE5, 0x69 }
PBL_APP_INFO_SIMPLE(MY_UUID, "Tertiary Text", "VGMoose & CCiollar", 1 /* App version */);

#define TOP 0
#define MID 1
#define BOT 2

Window window;

TextLayer textLayer;
TextLayer wordsYouWrite;

TextLayer buttons1[3];
TextLayer buttons2[3];
TextLayer buttons3[3];
TextLayer* bbuttons[] = {buttons1, buttons2, buttons3};

bool menu = false;

// Here are the three cases, or sets
char caps[] =    "ABCDEFGHIJKLM NOPQRSTUVWXYZ";
char letters[] = "abcdefghijklm nopqrstuvwxyz";
char numsym[] = "1234567890!?-'\"$()&*+#:@/,.";

// the below three strings just have to be unique, abc - xyz will be overwritten with the long strings above
char* btext1[] = {"abc", "def", "ghi"};
char* btext2[] = {"jkl", "m n", "opq"};
char* btext3[] = {"rst", "uvw", "xyz"};
char** btexts[] = {btext1, btext2, btext3};

// These are the actual sets that are displayed on each button
char set1[3] = "   ";
char set2[3] = "   ";
char set3[3] = "   ";

int cur_set = 1;
bool blackout = false;

void drawSides();
void drawMenu();
void set_menu();

char* rotate_text[] = {caps, letters, numsym};
void next();

char* master = letters;

char text_buffer[40];
int pos = 0;
int top, end, size;

// This function changes the next case/symbol set.
void change_set(int s, bool lock)
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
    if (lock) cur_set = s;
    
    next();
    drawSides();
}

void next()
{
    top = 0;
    end = 26;
    size = 27;
}

// These are to prevent missed clicks on a hold
void up_long_release_handler(ClickRecognizerRef recognizer, Window *window) {}
void select_long_release_handler(ClickRecognizerRef recognizer, Window *window) {}
void down_long_release_handler(ClickRecognizerRef recognizer, Window *window) {}

void clickButton(int b)
{
    if (!blackout)
    {
        if (menu)
        {
            change_set(b, false);
            return;
        }
        
        if (size > 3)
        {
            size /= 3;
            if (b == TOP)
                end -= 2*size;
            else if (b == MID)
            {
                top += size;
                end -= size;
            }
            else if (b == BOT)
                top += 2*size;
        }
        else
        {
            text_buffer[pos++] = master[top+b];
            text_layer_set_text(&wordsYouWrite, text_buffer);
            change_set(cur_set, false);
            next();
        }
    
        drawSides();
    }
    
}

// Modify these common button handlers
void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
    
    clickButton(TOP);
   
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
   
    clickButton(MID);
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    clickButton(BOT);
}

bool common_long(int b)
{
    if (menu)
    {
        change_set(b, true);
        return true;
    }
    return false;
}

void up_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    if (common_long(TOP)) return;
    
    set_menu();
    
}

void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;
    
    if (common_long(MID)) return;
    
    blackout = !blackout;

    if (blackout)
        text_layer_set_background_color(&textLayer, GColorBlack);
    else
         text_layer_set_background_color(&textLayer, GColorClear);

}


void down_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    
    if (common_long(BOT)) return;
    
    // delete or cancel when back is held
    
    if (size==27 && pos>0 && !blackout)
    {
        text_buffer[--pos] = ' ';
        text_layer_set_text(&wordsYouWrite, text_buffer);
    }
    else
    {
        next();
        drawSides();
    }

}

void set_menu()
{
    if (!blackout)
    {
        menu = true;
        drawMenu();
    }
}

// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {
  (void)window;

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
    config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;
    config[BUTTON_ID_SELECT]->long_click.release_handler = (ClickHandler) select_long_release_handler;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
    
    config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler;
    config[BUTTON_ID_UP]->long_click.release_handler = (ClickHandler) up_long_release_handler;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
    config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler;
    config[BUTTON_ID_DOWN]->long_click.release_handler = (ClickHandler) down_long_release_handler;

}

void drawMenu()
{
    text_layer_set_text(&buttons1[1], " ");
    text_layer_set_text(&buttons1[2], " ");
    
    text_layer_set_text(&buttons2[1], " ");
    text_layer_set_text(&buttons2[2], " ");
    
    text_layer_set_text(&buttons3[0], " ");
    text_layer_set_text(&buttons3[2], " ");
    
    text_layer_set_text(&buttons1[0], "CAP");
    text_layer_set_font(&buttons1[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    
    text_layer_set_text(&buttons2[0], "lower");
    text_layer_set_font(&buttons2[0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    
    text_layer_set_text(&buttons3[1], "num\nsym");
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

void initSidesAndText()
{
    text_layer_init(&wordsYouWrite, GRect(10, 0, 100, 135));
    text_layer_set_background_color(&wordsYouWrite, GColorClear);
    text_layer_set_font(&wordsYouWrite, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(&window.layer, &wordsYouWrite.layer);


    for (int i = 0; i<3; i++)
    {
        text_layer_init(&buttons1[i], GRect(115, 12*i, 100, 100));
        text_layer_init(&buttons2[i], GRect(115, 12*i+50, 100, 100));
        text_layer_init(&buttons3[i], GRect(115, 12*i+100, 100, 100));
    }
    
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            layer_add_child(&window.layer, &bbuttons[i][j].layer);

}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Text Input");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&FONT_DEMO_RESOURCES);

  text_layer_init(&textLayer, window.layer.frame);
//  text_layer_set_text(&textLayer, text_buffer);
    text_layer_set_background_color(&textLayer, GColorClear);

  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(&window.layer, &textLayer.layer);
    
    initSidesAndText();
    drawSides();
    
  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
    
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  change_set(1, true);
  app_event_loop(params, &handlers);
}
