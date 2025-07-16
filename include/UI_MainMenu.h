#ifndef MM_H
#define MM_H

#include <lvgl.h>
#include <Arduino.h>


// functions to build GUI
void lv_create_MainMenu(void);
void lv_create_MainMenu_2(void);
void lv_create_timer();

void lv_load_mainmenu(bool deleteold);
void lv_load_timer();

static void slider_event_cb(lv_event_t * e);
static void target_type_changed(lv_event_t * e);
static void mode_type_changed(lv_event_t * e);
static void button_go_cb(lv_event_t * e);
static void button_stop_cb(lv_event_t * e);
static void btn_mode_cb(lv_event_t *e);
static void lv_spinbox_increment_event_cb(lv_event_t * e);
static void lv_spinbox_decrement_event_cb(lv_event_t * e);

#endif