#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xC3, 0x3B, 0x34, 0xBF, 0x4F, 0x1D, 0x4F, 0x07, 0xBA, 0x34, 0x73, 0x50, 0xC8, 0xAB, 0xA8, 0xF9 }
PBL_APP_INFO(MY_UUID,
             "PPF", "saulricardo",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
BmpContainer background_image_container;
TextLayer timePastLayer; //Clock
TextLayer timePresentLayer; //Clock
TextLayer timeFutureLayer; //Clock

void update_time_display() {
  static char timePast[] = "00:00";
  static char timePresent[] = "00:00";
  static char timeFuture[] = "00:00";
  char *timeFormat;

  PblTm currentTime;
  PblTm pastTime;
  PblTm futureTime;
  
    timeFormat = "%I:%M";
  //get pebble Time
  get_time(&currentTime);
  get_time(&pastTime);
  get_time(&futureTime);
  
  if(currentTime.tm_min == 0){
 	 pastTime.tm_min = 59;
 	 pastTime.tm_hour = pastTime.tm_hour - 1;
  } else{
    pastTime.tm_min = pastTime.tm_min - 1;
  }
  if(currentTime.tm_min == 59){
 	 futureTime.tm_min = 0;
 	 futureTime.tm_hour = futureTime.tm_hour + 1;
  } else {
    futureTime.tm_min = futureTime.tm_min + 1;
  }
  string_format_time(timePast, sizeof(timePast), timeFormat, &pastTime);
  string_format_time(timePresent, sizeof(timePresent), timeFormat, &currentTime);
  string_format_time(timeFuture, sizeof(timeFuture), timeFormat, &futureTime);

  if (!clock_is_24h_style() && (timePast[0] == '0')) {
    memmove(timePast, &timePast[1], sizeof(timePast) - 1);
  }
  if (!clock_is_24h_style() && (timePresent[0] == '0')) {
    memmove(timePresent, &timePresent[1], sizeof(timePresent) - 1);
  }
  if (!clock_is_24h_style() && (timeFuture[0] == '0')) {
    memmove(timeFuture, &timeFuture[1], sizeof(timeFuture) - 1);
  }

  text_layer_set_text(&timePastLayer, timePast);
  text_layer_set_text(&timePresentLayer, timePresent);
  text_layer_set_text(&timeFutureLayer, timeFuture);

}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "PPF");
  window_stack_push(&window, true /* Animated */);
  
  resource_init_current_app(&APP_RESOURCES);

  // Set up a layer for the static watch face background
  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image_container);
  layer_add_child(&window.layer, &background_image_container.layer.layer);
  
  //Initializes the timePastLayer  
  text_layer_init(&timePastLayer, GRect(0, 168-42, 60 /* width */, 45 /* height */));
  text_layer_set_text_alignment(&timePastLayer, GTextAlignmentCenter);
  text_layer_set_text_color(&timePastLayer, GColorBlack);
  layer_set_frame(&timePastLayer.layer, GRect(0, 18, 144, 168-18));
  text_layer_set_background_color(&timePastLayer, GColorClear);
  text_layer_set_font(&timePastLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &timePastLayer.layer);
  
  //Initializes the timePresentLayer  
  text_layer_init(&timePresentLayer, GRect(0, 168-42, 60 /* width */, 45 /* height */));
  text_layer_set_text_alignment(&timePresentLayer, GTextAlignmentCenter);
  text_layer_set_text_color(&timePresentLayer, GColorBlack);
  layer_set_frame(&timePresentLayer.layer, GRect(0, 65, 144, 168-65));
  text_layer_set_background_color(&timePresentLayer, GColorClear);
  text_layer_set_font(&timePresentLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &timePresentLayer.layer);
  
  //Initializes the timeFutureLayer  
  text_layer_init(&timeFutureLayer, GRect(0, 168-42, 60 /* width */, 45 /* height */));
  text_layer_set_text_alignment(&timeFutureLayer, GTextAlignmentCenter);
  text_layer_set_text_color(&timeFutureLayer, GColorBlack);
  layer_set_frame(&timeFutureLayer.layer, GRect(0, 110, 144, 168-110));
  text_layer_set_background_color(&timeFutureLayer, GColorClear);
  text_layer_set_font(&timeFutureLayer, fonts_get_system_font(FONT_KEY_GOTHAM_30_BLACK));
  layer_add_child(&window.layer, &timeFutureLayer.layer);
  
  update_time_display();
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image_container);

}

static void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  (void)t;
  update_time_display();
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
