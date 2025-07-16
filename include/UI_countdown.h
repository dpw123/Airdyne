#ifndef UI_COUNTDOWN_H
#define UI_COUNTDOWN_H


#include <lvgl.h>
#include <Arduino.h>


// external functions to build GUI
void lv_create_countdown(void);
void lv_load_countdown();
void countdown(int time_seconds);

#endif