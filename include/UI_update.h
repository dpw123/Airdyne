#ifndef UI_UPDATE_H
#define UI_UPDATE_H

#include <lvgl.h>
#include <Arduino.h>
#include "TripComputer.h"


namespace UI { 
enum OBJECT {
    TIME,
    DIST,
    CALS,
    RPM,
    ProgBar,
    ROUND,
    countdown
    };
}


// functions to update gui
void update_time( long seconds);
void update_rpm( float rpm);
void update_dist( double metres);
void update_cals( float cals);
void update_round(int rc, int tr);
void update_pb_target(int target, int divisor, TC::TARGET_TYPE target_type);
void update_countdown(int secs);

void set_UI_value_object(UI::OBJECT target_label, lv_obj_t * label_object);
lv_obj_t * get_UI_value_object(UI::OBJECT target_label);

#endif