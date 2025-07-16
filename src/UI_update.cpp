#include "UI_update.h"

static lv_obj_t * active_screen;
static lv_obj_t * RPM_Value;
static lv_obj_t * Round_Value;
static lv_obj_t * Time_Value;
static lv_obj_t * Dist_Value;
static lv_obj_t * Cals_Value;
static lv_obj_t * Progress_bar;
static lv_obj_t * running_btn;
static lv_obj_t * lbl_countdown;


void update_time( long seconds) {
  int hh = seconds / 60 / 60;
  int mm = (seconds - 60*60*hh) / 60;
  int ss = (seconds - 60*60*hh) - 60*mm;
  // Serial.printf("Updating time at %p", Time_Value);
  lv_label_set_text_fmt(  Time_Value , "%02d:%02d:%02d",hh,mm,ss );
}

void update_button_label(char new_text[30]) {

}

void update_rpm( float rpm) { lv_label_set_text_fmt(  get_UI_value_object(UI::RPM) , "%3.1f ",rpm ); }

void update_dist( double metres) {
    lv_label_set_text_fmt(  get_UI_value_object(UI::DIST) , "%.0f m",metres );
    if ((get_bike_mode()==TC::RFT ||get_bike_mode()==TC::EMOM||get_bike_mode()==TC::DEATH_BY)  && get_target_type()==TC::DIST)         lv_bar_set_value(lv_obj_get_child_by_type(get_UI_value_object(UI::ProgBar),0,&lv_bar_class ), (int32_t)metres, LV_ANIM_OFF);
}

void update_cals( float cals) {
    lv_label_set_text_fmt(  get_UI_value_object(UI::CALS) , "%.1f KCal",cals );
    if ((get_bike_mode()==TC::RFT ||get_bike_mode()==TC::EMOM||get_bike_mode()==TC::DEATH_BY)  && get_target_type()==TC::CALS)        lv_bar_set_value(lv_obj_get_child_by_type(get_UI_value_object(UI::ProgBar),0,&lv_bar_class ), (int32_t)(cals*10), LV_ANIM_OFF);
}

void update_round(int rc, int tr) {
    lv_label_set_text_fmt(  lv_obj_get_child( get_UI_value_object(UI::ROUND), 1 ) , "%d of %d", rc, tr );
}
void update_countdown(int secs) {
    lv_label_set_text_fmt(   get_UI_value_object(UI::countdown) , "%d", secs );
    Serial.println(secs);
}

void update_pb_target(int target, int divisor, TC::TARGET_TYPE target_type) {
    lv_bar_set_range(lv_obj_get_child_by_type(get_UI_value_object(UI::ProgBar),0,&lv_bar_class ),0,target*divisor);
    switch (target_type) {
        case TC::CALS:
            lv_label_set_text_fmt(lv_obj_get_child_by_type(get_UI_value_object(UI::ProgBar),0,&lv_label_class ),"%d KCal",target );
            break;
        case TC::DIST:
            lv_label_set_text_fmt(lv_obj_get_child_by_type(get_UI_value_object(UI::ProgBar),0,&lv_label_class ),"%d m",target );
            break;
    }
}

void set_UI_value_object(UI::OBJECT target_label, lv_obj_t * label_object){
    switch (target_label) {
        case UI::CALS: {
            Cals_Value = label_object;
            break;}
        case UI::ROUND: {
            Round_Value = label_object;
            break;}
        case UI::DIST: {
            Dist_Value = label_object;
            break;}
        case UI::TIME: {
            Time_Value = label_object;
            break;}
        case UI::RPM: {
            RPM_Value = label_object;
            break;}
        case UI::ProgBar: {
            Progress_bar = label_object ;
            break;}        
        case UI::countdown: {
            lbl_countdown = label_object ;
            break;}
    }

};

lv_obj_t * get_UI_value_object(UI::OBJECT target_label){
    switch (target_label) {
        case UI::CALS: {
            return Cals_Value;
            break;}
        case UI::ROUND: {
            return Round_Value;
            break;}
        case UI::DIST: {
            return Dist_Value ;
            break;}
        case UI::TIME: {
            return Time_Value ;
            break;}
        case UI::RPM: {
            return RPM_Value ;
            break;}
        case UI::ProgBar: {
            return Progress_bar ;
            break;}
        case UI::countdown: {
            return lbl_countdown ;
            break;}
        default:
            return NULL;
    }
};