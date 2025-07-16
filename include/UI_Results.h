#ifndef UI_RESULTS_H
#define UI_RESULTS_H


#include <lvgl.h>
#include <Arduino.h>


// external functions to build GUI
void lv_create_Results(void);
void load_results_screen();
void add_results_row(int round, double metres, float cals, long seconds);

// internal call back functions
static void button_reset_cb(lv_event_t * e);

#endif