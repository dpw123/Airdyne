#include "UI_MainMenu.h"
#include "UI_update.h"
#include "TripComputer.h"
#include "UI_countdown.h"





// globals

lv_obj_t * SpinCals;
lv_obj_t * SpinDist ;
lv_obj_t * SpinTime ;
lv_obj_t * SpinRnds ;
lv_obj_t * btnmTargetType ;
lv_obj_t * btnmMode;

// screens
lv_obj_t *scr_mainmenu;
lv_obj_t *scr_timer;
lv_obj_t * scr_countdown;

lv_style_t style_tsp;
lv_style_t style_spin;
lv_style_t style_grey;



// UI HELPER FUNCTIONS


lv_obj_t *create_label(lv_obj_t *parent, int32_t x, int32_t y, const char *label_text, const lv_font_t *font)
{
  lv_obj_t *lbl = lv_label_create(parent);
  lv_label_set_text(lbl, label_text);
  lv_obj_align_to(lbl, parent, LV_ALIGN_TOP_LEFT, x, y);
  lv_obj_set_style_text_font(lbl, font, 0);
  return lbl;
}

lv_obj_t *create_spinbox(lv_obj_t *parent, char *labelText, int32_t x, int32_t y, int32_t digits, int32_t initialValue )
{

  lv_style_init(&style_tsp);                      // Initialize the style
  lv_style_set_bg_opa(&style_tsp, LV_OPA_TRANSP); // Set background opacity to transparent
  lv_style_set_border_width(&style_tsp, 0);       // No border

  lv_style_init(&style_spin);
  lv_style_set_text_letter_space(&style_spin,4);
  lv_style_set_text_align(&style_spin, LV_TEXT_ALIGN_RIGHT);
  lv_style_set_text_font(&style_spin, &lv_font_montserrat_18);

  
  lv_style_init(&style_grey);
  lv_style_set_bg_color(&style_grey, lv_color_make(200,200,200));

  lv_obj_t * SB = lv_obj_create(parent);
  lv_obj_set_size(SB,300,50);
  lv_obj_remove_flag(SB, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_pos(SB,x,y);
  lv_obj_add_style(SB, &style_tsp, 0);
  lv_obj_add_style(SB, &style_grey, LV_PART_SELECTED);

  lv_obj_t * lbl = lv_label_create(SB);
  lv_obj_align(lbl, LV_ALIGN_LEFT_MID,0,0);
  lv_label_set_text(lbl, labelText);
  

  lv_obj_t * spin = lv_spinbox_create(SB);
    lv_spinbox_set_range(spin, 1,  pow(10,digits) - 1);
    lv_spinbox_set_digit_format(spin, digits, 0);
    lv_spinbox_set_value(spin, initialValue);
    lv_spinbox_step_prev(spin);
    lv_obj_set_size(spin, 100 ,40);
    lv_obj_align(spin,LV_ALIGN_LEFT_MID,140,0);
    lv_obj_remove_flag(spin, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(spin,&style_spin,0);

    lv_obj_t * btn = lv_button_create(SB);
  
    lv_obj_set_size(btn, 35, 35);
    lv_obj_align_to(btn, spin, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  NULL);

    btn = lv_button_create(SB);
    lv_obj_set_size(btn, 35, 35);
    lv_obj_align_to(btn, spin, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);

  return SB;
}





// SCREEN DEFINITIONS

void lv_create_MainMenu_2(void)
{

  char buf[10];
  int TargetCals = get_target(TC::CALS);
  int TargetDist = get_target(TC::DIST);
  int TargetRounds = get_target(TC::ROUNDS);
  int TargetRoundTime = get_target(TC::TIME);

  scr_mainmenu = lv_obj_create(NULL);

  /* create a button selector */
  static lv_style_t style_bg;
  lv_style_init(&style_bg);
  lv_style_set_pad_all(&style_bg, 0);
  lv_style_set_pad_gap(&style_bg, 0);
  lv_style_set_clip_corner(&style_bg, true);
  lv_style_set_radius(&style_bg, LV_RADIUS_CIRCLE);
  lv_style_set_border_width(&style_bg, 0);

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_radius(&style_btn, 0);
  lv_style_set_border_width(&style_btn, 1);
  lv_style_set_border_opa(&style_btn, LV_OPA_50);
  lv_style_set_border_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_border_side(&style_btn, LV_BORDER_SIDE_INTERNAL);
  lv_style_set_radius(&style_btn, 0);

  static const char * btnm_map[] = {"Just Ride", "FT", "EMOM", "Death By", "" };
  lv_obj_t * btnmMode = lv_buttonmatrix_create(scr_mainmenu);
  lv_buttonmatrix_set_map(btnmMode, btnm_map);
  lv_obj_add_style(btnmMode, &style_bg, 0);
  lv_obj_add_style(btnmMode, &style_btn, LV_PART_ITEMS);
  
  lv_obj_set_size(btnmMode, 320, 30);
  lv_obj_set_pos(btnmMode, 0, 0);
  lv_obj_add_event_cb(btnmMode, btn_mode_cb, LV_EVENT_ALL, NULL);
  
  lv_buttonmatrix_set_button_ctrl_all(btnmMode, LV_BUTTONMATRIX_CTRL_CHECKABLE);
  lv_buttonmatrix_set_one_checked(btnmMode, true);
  lv_buttonmatrix_set_button_ctrl(btnmMode, 0, LV_BUTTONMATRIX_CTRL_CHECKED);
  lv_buttonmatrix_set_selected_button(btnmMode,0);


  /* create distance target selector */
   int tp = 28;
   int ht = 35;
   int gp = 8;
  SpinCals = create_spinbox(scr_mainmenu,"", 10, tp,3,TargetCals);
  SpinDist = create_spinbox(scr_mainmenu,"", 10, tp+(ht+gp),5,TargetDist);
  SpinTime = create_spinbox(scr_mainmenu,"Time (s)", 10, tp+2*(ht+gp),3,TargetRoundTime);
  SpinRnds = create_spinbox(scr_mainmenu,"Rounds", 10, tp+3*(ht+gp),2,TargetRounds);
  
  lv_obj_add_flag(SpinCals, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(SpinDist, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(SpinTime, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(SpinRnds, LV_OBJ_FLAG_HIDDEN);

  // target type selector
  static const char * btnm_map2[] = {"KCal","\n", "Metres", "" };
  btnmTargetType = lv_buttonmatrix_create(scr_mainmenu);
  lv_buttonmatrix_set_map(btnmTargetType, btnm_map2);
  lv_obj_add_style(btnmTargetType, &style_bg, 0);
  lv_obj_add_style(btnmTargetType, &style_btn, LV_PART_ITEMS);
  
  lv_obj_set_size(btnmTargetType, 80, 78);
  lv_obj_set_pos(btnmTargetType, 20, 38);
  lv_obj_add_flag(btnmTargetType, LV_OBJ_FLAG_HIDDEN);
  
  lv_buttonmatrix_set_button_ctrl_all(btnmTargetType, LV_BUTTONMATRIX_CTRL_CHECKABLE);
  lv_buttonmatrix_set_one_checked(btnmTargetType, true);
  lv_buttonmatrix_set_button_ctrl(btnmTargetType, 0, LV_BUTTONMATRIX_CTRL_CHECKED);
  lv_buttonmatrix_set_selected_button(btnmTargetType,0);

  // GO BUTTON

  lv_obj_t *button_go = lv_button_create(scr_mainmenu);
  lv_obj_set_pos(button_go, 20, 205);
  lv_obj_set_size(button_go, 280, 30);
  lv_obj_remove_flag(button_go, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_add_event_cb(button_go, button_go_cb, LV_EVENT_PRESSED, NULL);

  lv_obj_t *label_go = lv_label_create(button_go);
  lv_label_set_text(label_go, "READY UP!");
  lv_obj_center(label_go);
}

void lv_create_timer()
{

  scr_timer = lv_obj_create(NULL);

  lv_style_init(&style_tsp);                      // Initialize the style
  lv_style_set_bg_opa(&style_tsp, LV_OPA_TRANSP); // Set background opacity to transparent
  lv_style_set_border_width(&style_tsp, 0);       // No border

  lv_obj_t *Round = lv_obj_create(scr_timer);
  lv_obj_align_to(Round, scr_timer, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_size(Round, 320, 40);
  lv_obj_remove_flag(Round, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_style(Round, &style_tsp, 0);

  lv_obj_t *Round_Label = create_label(Round, -5, -10, "Round:", &lv_font_montserrat_24);
  lv_obj_t *Round_Value = create_label(Round, 145, -10, "1 of ?", &lv_font_montserrat_24);

  lv_obj_t *RPM_Label = create_label(scr_timer, 10, 35, "RPM:", &lv_font_montserrat_24);
  lv_obj_t *RPM_Value = create_label(scr_timer, 160, 35, "0.0", &lv_font_montserrat_24);

  lv_obj_t *Time_Label = create_label(scr_timer, 10, 60, "Time:", &lv_font_montserrat_24);
  lv_obj_t *Time_Value = create_label(scr_timer, 160, 60, "00:00:00", &lv_font_montserrat_24);

  lv_obj_t *Dist_Label = create_label(scr_timer, 10, 85, "Distance:", &lv_font_montserrat_24);
  lv_obj_t *Dist_Value = create_label(scr_timer, 160, 85, "0 m", &lv_font_montserrat_24);

  lv_obj_t *Cals_Label = create_label(scr_timer, 10, 110, "Calories:", &lv_font_montserrat_24);
  lv_obj_t *Cals_Value = create_label(scr_timer, 160, 110, "0 KCal", &lv_font_montserrat_24);

  set_UI_value_object(UI::ROUND, Round);
  set_UI_value_object(UI::CALS, Cals_Value);
  set_UI_value_object(UI::DIST, Dist_Value);
  set_UI_value_object(UI::TIME, Time_Value);
  set_UI_value_object(UI::RPM, RPM_Value);

  // PROGRESS BAR

  lv_obj_t *PB = lv_obj_create(scr_timer);
  lv_obj_align_to(PB, scr_timer, LV_ALIGN_TOP_LEFT, 0, 145);
  lv_obj_set_size(PB, 320, 32);
  lv_obj_remove_flag(PB, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_style(PB, &style_tsp, 0);

  lv_obj_t *Progress_bar = lv_bar_create(PB);
  lv_obj_set_size(Progress_bar, 280, 30);
  lv_obj_align_to(Progress_bar, PB, LV_ALIGN_CENTER, 0, 0);
  lv_bar_set_value(Progress_bar, 00, LV_ANIM_OFF);

  lv_obj_t *Target_Label = lv_label_create(PB);
  lv_obj_align_to(Target_Label, Progress_bar, LV_ALIGN_CENTER, 0, 0);
  set_UI_value_object(UI::ProgBar, PB);
  
  // STOP BUTTON

  lv_obj_t *button_stop = lv_button_create(scr_timer);
  lv_obj_set_pos(button_stop, 20, 205);
  lv_obj_set_size(button_stop, 280, 30);
  lv_obj_remove_flag(button_stop, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_add_event_cb(button_stop, button_stop_cb, LV_EVENT_PRESSED, NULL);

  lv_obj_t *label_stop = lv_label_create(button_stop);
  lv_label_set_text(label_stop, "STOP!");
  lv_obj_center(label_stop);
}




void lv_create_countdown()
{

   scr_countdown = lv_obj_create(NULL);

    lv_obj_t * lbl_countdown = lv_label_create(scr_countdown);
    lv_label_set_text(lbl_countdown, "XX");
    lv_obj_align(lbl_countdown, LV_ALIGN_CENTER, 0, 0);

    set_UI_value_object(UI::countdown, lbl_countdown);

}

// LOAD SCREENS

void lv_load_timer()
{
  lv_screen_load_anim(scr_timer, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, false);
  TC::BIKE_MODE bm = get_bike_mode();
  if (bm == TC::JUST_RIDE)
  {
    lv_obj_add_flag(get_UI_value_object(UI::ROUND), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(get_UI_value_object(UI::ProgBar), LV_OBJ_FLAG_HIDDEN);
  }
  if (bm == TC::RFT)
  {
    lv_obj_remove_flag(get_UI_value_object(UI::ROUND), LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(get_UI_value_object(UI::ProgBar), LV_OBJ_FLAG_HIDDEN);
  }
  if (bm == TC::EMOM || bm == TC::DEATH_BY)
  {
    lv_obj_remove_flag(get_UI_value_object(UI::ROUND), LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(get_UI_value_object(UI::ProgBar), LV_OBJ_FLAG_HIDDEN);
  }
  if (bm == TC::DEATH_BY)
  {
    set_target(TC::ROUNDS, 999);
    set_target(TC::TIME, 60);
    set_target(TC::CALS, 1);
  }

  
  lv_obj_t *  PB = get_UI_value_object(UI::ProgBar);
  lv_obj_t *  Progress_bar = lv_obj_get_child(PB,0);
  lv_obj_t *  Target_Label = lv_obj_get_child(PB,1);
  switch (get_target_type())
  {
  case TC::CALS:
  {
    Serial.println("CALS");
    lv_bar_set_range(Progress_bar, 00, get_target(TC::CALS) * 10);
    lv_label_set_text_fmt(Target_Label, "%d KCals", get_target(TC::CALS));
    break;
  }
  case TC::DIST:
  {
    Serial.println("DIST");
    lv_bar_set_range(Progress_bar, 00, get_target(TC::DIST));
    lv_label_set_text_fmt(Target_Label, "%d m", get_target(TC::DIST));
    break;
  }
  }

}

void lv_load_mainmenu(bool deleteold)
{
 // lv_tabview_set_active(tabview, (int)get_bike_mode(), LV_ANIM_OFF);
  char buf[10];
  int TargetCals = get_target(TC::CALS);
  int TargetDist = get_target(TC::DIST);
  int TargetRounds = get_target(TC::ROUNDS);
  int TargetRoundTime = get_target(TC::TIME);

  // lv_slider_set_value(targetCals_FT, TargetCals, LV_ANIM_OFF);
  // lv_slider_set_value(targetCals_EMOM, TargetCals, LV_ANIM_OFF);
  // lv_slider_set_value(targetDist_FT, TargetDist / 250, LV_ANIM_OFF);
  // lv_slider_set_value(targetDist_EMOM, TargetDist / 250, LV_ANIM_OFF);
  // lv_slider_set_value(targetRnds_EMOM, TargetRounds, LV_ANIM_OFF);
  // lv_slider_set_value(targetRndTime_EMOM, TargetRoundTime / 30, LV_ANIM_OFF);
  lv_screen_load_anim(scr_mainmenu, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, deleteold);
};


void lv_load_countdown() {
  Serial.println("try");
   lv_screen_load_anim(scr_countdown,LV_SCR_LOAD_ANIM_FADE_ON,100,0,false);
  Serial.println("did");
  }



///////////// CALL BACKS /////////////////////
void btn_mode_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    uint32_t id = 99;
    if(code == LV_EVENT_VALUE_CHANGED) {
        id = lv_buttonmatrix_get_selected_button(obj);
        Serial.printf("%d was pressed\n", id);
    }
    switch (id) {
      case 0: 
      lv_obj_add_flag(btnmTargetType, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinCals, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinDist, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinTime, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinRnds, LV_OBJ_FLAG_HIDDEN);
      set_bike_mode(TC::JUST_RIDE);
        break;      
      case 1: 
      lv_obj_remove_flag(btnmTargetType, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinCals, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinDist, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinTime, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinRnds, LV_OBJ_FLAG_HIDDEN);
      set_bike_mode(TC::RFT);
        break;
      case 2: 
      lv_obj_remove_flag(btnmTargetType, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinCals, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinDist, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinTime, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinRnds, LV_OBJ_FLAG_HIDDEN);
      set_bike_mode(TC::EMOM);
        break;      
      case 3: 
      lv_obj_remove_flag(btnmTargetType, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinCals, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinDist, LV_OBJ_FLAG_HIDDEN);
      lv_obj_remove_flag(SpinTime, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(SpinRnds, LV_OBJ_FLAG_HIDDEN);
      set_bike_mode(TC::DEATH_BY);
        break;
    }
}

static void lv_spinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(lv_obj_get_child(lv_obj_get_parent(lv_event_get_target_obj(e)),1));
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(lv_obj_get_child(lv_obj_get_parent(lv_event_get_target_obj(e)),1) );
    }
}


/// @brief when the GO  button is clicked
/// @param e the event reference
void button_go_cb(lv_event_t *e)
{
  Serial.print("\nGo pressed\n");
  switch (lv_buttonmatrix_get_selected_button(btnmTargetType)) {
    case 0: 
      set_target_type(TC::CALS);
      break;
    case 1:
      set_target_type(TC::DIST);
      break;
  }

  set_target(TC::CALS, lv_spinbox_get_value(lv_obj_get_child(SpinCals,1)));
  set_target(TC::DIST, lv_spinbox_get_value(lv_obj_get_child(SpinDist,1)));
  set_target(TC::TIME, lv_spinbox_get_value(lv_obj_get_child(SpinTime,1)));
  set_target(TC::ROUNDS, lv_spinbox_get_value(lv_obj_get_child(SpinRnds,1)));
  lv_load_timer();
  start_running();
}

void button_stop_cb(lv_event_t *e)
{
  lv_screen_load_anim(scr_mainmenu, LV_SCR_LOAD_ANIM_NONE, 100, 0, false);
  stop_running();
}
