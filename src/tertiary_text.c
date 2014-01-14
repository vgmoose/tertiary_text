#include <pebble.h>

#define TOP 0
#define MID 1
#define BOT 2

static Window* window;

static TextLayer* text_layer;
static TextLayer* wordsYouWrite;

static TextLayer* buttons1[3];
static TextLayer* buttons2[3];
static TextLayer* buttons3[3];
static TextLayer** bbuttons[3];

static bool menu = false;

// Here are the three cases, or sets
static char caps[] =    "ABCDEFGHIJKLM NOPQRSTUVWXYZ";
static char letters[] = "abcdefghijklm nopqrstuvwxyz";
static char numsym[] = "1234567890!?-'\"$()&*+#:@/,.";

// the below three strings just have to be unique, abc - xyz will be overwritten with the long strings above
static char* btext1[] = {"abc\0", "def\0", "ghi\0"};
static char* btext2[] = {"jkl\0", "m n\0", "opq\0"};
static char* btext3[] = {"rst\0", "uvw\0", "xyz\0"};
static char** btexts[] = {btext1, btext2, btext3};

// These are the actual sets that are displayed on each button, also need to be unique
static char set1[4] = "  a\0";
static char set2[4] = "  b\0";
static char set3[4] = "  c\0";
static char* setlist[] = {set1, set2, set3};

static char* cases[] = {"CAP", "low", "#@1"};

static int cur_set = 1;
static bool blackout = false;

static void drawSides();
static void drawMenu();
static void set_menu();

static char* rotate_text[] = {caps, letters, numsym};
static void next();

static char* master = letters;

static char text_buffer[60];
static int pos = 0;
static int top, end, size;

// This function changes the next case/symbol set.
static void change_set(int s, bool lock)
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
    
    drawSides();
}

static void next()
{
    top = 0;
    end = 26;
    size = 27;
}

static void up_long_release_handler(ClickRecognizerRef recognizer, void *window) {}
static void select_long_release_handler(ClickRecognizerRef recognizer, void *window) {}
static void down_long_release_handler(ClickRecognizerRef recognizer, void *window) {}

static void clickButton(int b)
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
            text_layer_set_text(wordsYouWrite, text_buffer);
            change_set(cur_set, false);
            next();
        }
        
        drawSides();
    }
    
}

// Modify these common button handlers
static void up_single_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    clickButton(TOP);
    
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    clickButton(MID);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    clickButton(BOT);
}

static bool common_long(int b)
{
    if (menu)
    {
        change_set(b, true);
        return true;
    }
    return false;
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    if (common_long(TOP)) return;
    
    set_menu();
    
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    if (common_long(MID)) return;
    
    blackout = !blackout;
    
    if (blackout)
    text_layer_set_background_color(text_layer, GColorBlack);
    else
    text_layer_set_background_color(text_layer, GColorClear);
    
}


static void down_long_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    if (common_long(BOT)) return;
    
    // delete or cancel when back is held
    
    if (size==27 && pos>0 && !blackout)
    {
        text_buffer[--pos] = ' ';
        text_layer_set_text(wordsYouWrite, text_buffer);
    }
    else
    {
        next();
        drawSides();
    }
    
}

static void set_menu()
{
    if (!blackout)
    {
        menu = true;
        drawMenu();
    }
}

// This usually won't need to be modified

static void click_config_provider(void* context) {
    
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_UP, 1000, up_long_click_handler, up_long_release_handler);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler, select_long_release_handler);
    
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_DOWN, 1000, down_long_click_handler, down_long_release_handler);
}

static void drawMenu()
{
    for (int i=0; i<3; i++)
    {
        text_layer_set_text(bbuttons[i][i!=2], " ");
        text_layer_set_text(bbuttons[i][2], " ");
        
        text_layer_set_text(bbuttons[i][i==2], cases[i]);
        text_layer_set_font(bbuttons[i][0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    }
}


// This method draws the characters on the right side near the buttons
static void drawSides()
{
    if (size==27) // first click (full size)
    {
        // update all 9 labels to their proper values
        for (int h=0; h<3; h++)
        {
            for (int i=0; i<3; i++)
            {
                text_layer_set_text(bbuttons[h][i], btexts[h][i]);
                text_layer_set_background_color(bbuttons[h][i], GColorClear);
                text_layer_set_font(bbuttons[h][i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            }
            
        }
    }
    else if (size==9)   // second click
    {
        
        for (int i=0; i<3; i++)
        {
            text_layer_set_text(bbuttons[i][i!=2], " ");
            text_layer_set_text(bbuttons[i][2], " ");
            
            text_layer_set_text(bbuttons[i][i==2], btexts[top/9][i]);
            text_layer_set_font(bbuttons[i][i==2], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        }
        
    } else if (size == 3)
    {
        for (int i=0; i<3; i++)
        {
            setlist[i][2] = master[top+i];
            text_layer_set_text(bbuttons[i][i==2], setlist[i]);
            
        }
    }
    
}

static void initSidesAndText()
{
    Layer *window_layer = window_get_root_layer(window);

    wordsYouWrite = text_layer_create((GRect) { .origin = { 10, 0 }, .size = { 100, 150 } });

    text_layer_set_background_color(wordsYouWrite, GColorClear);
    text_layer_set_font(wordsYouWrite, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(wordsYouWrite));
    
    for (int i = 0; i<3; i++)
    {
        buttons1[i] = text_layer_create((GRect) { .origin = { 115, 12*i }, .size = { 100, 100 } });
        buttons2[i] = text_layer_create((GRect) { .origin = { 115, 12*i+50 }, .size = { 100, 100 } });
        buttons3[i] = text_layer_create((GRect) { .origin = { 115, 12*i+100 }, .size = { 100, 100 } });
    }
    
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            layer_add_child(window_layer, text_layer_get_layer(bbuttons[i][j]));
    
}

static void deinit(void) {
    window_destroy(window);
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
}

static void window_load(Window* window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });

    //  text_layer_set_text(&textLayer, text_buffer);
//    text_layer_set_background_color(&textLayer, GColorClear);
    
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
    
    initSidesAndText();
    drawSides();
    
    // Attach our desired button functionality
//    window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);

}

static void init(void) {
    window = window_create();
    
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    
    const bool animated = true;
    window_stack_push(window, animated);
}


int main(void) {
    bbuttons[0] = buttons1;
    bbuttons[1] = buttons2;
    bbuttons[2] = buttons3;
    init();
//    PebbleAppHandlers handlers = {
//        .init_handler = &handle_init
//    };
    change_set(1, true);
    next();
    drawSides();
    app_event_loop();
    deinit();
}
