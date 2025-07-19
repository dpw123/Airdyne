#include "UI_Results.h"

#include "UI_MainMenu.h"
#include "TripComputer.h"

static lv_obj_t * table; 
static lv_obj_t * scr_Results;


void button_reset_cb(lv_event_t * e)
{
    lv_load_mainmenu(false);
    stop_running();
}

void add_results_row(int round, double metres, float cals, long seconds) {
    char buf[16]; 
    int  row =round;
    Serial.printf("Adding Row %d: %d %0.f %1.f %d \n",row, round,metres,cals,seconds);

    Serial.printf("table has %d rows\n", lv_table_get_row_cnt(table));
    if (lv_table_get_row_cnt(table) <= row) {
        // Add a new row if it doesn't exist
        lv_table_set_row_cnt(table, row + 1);
        Serial.printf("Adding new row %d\n", row+1);
        Serial.printf("table now has %d rows\n", lv_table_get_row_cnt(table));
    }


    sprintf(buf, "%d", round);
    lv_table_set_cell_value(table, row , 0, buf); 

    sprintf(buf, "%.0f", metres);
    lv_table_set_cell_value(table, row , 1, buf); 

    sprintf(buf, "%.1f", cals);
    lv_table_set_cell_value(table, row , 2, buf);
    
    
    int hh = seconds / 60 / 60;
    int mm = (seconds - 60*60*hh) / 60;
    int ss = (seconds - 60*60*hh) - 60*mm;

    sprintf(buf, "%02d:%02d:%02d",hh,mm,ss);
    lv_table_set_cell_value(table, row , 3, buf);
    Serial.printf("table has %d rows, %s rounds, %s m\n", lv_table_get_row_cnt(table), lv_table_get_cell_value(table, round, 0), lv_table_get_cell_value(table, round, 1));
}

void load_results_screen() { lv_screen_load_anim(scr_Results,LV_SCR_LOAD_ANIM_FADE_ON,100,0,false);};

void lv_create_Results()
{

   scr_Results= lv_obj_create(NULL);
  // lv_screen_load_anim(scr_Results,LV_SCR_LOAD_ANIM_NONE,0,0,true);

  table = lv_table_create(scr_Results);
  lv_obj_align_to(table, scr_Results, LV_ALIGN_TOP_LEFT, 10, 10);

  // Set column widths (optional)
  lv_table_set_column_width(table, 0, 35);
  lv_table_set_column_width(table, 1, 70);
  lv_table_set_column_width(table, 2, 70);
  lv_table_set_column_width(table, 3, 90);

  // Set column header labels
  const char * headers[] = {"#", "m", "KCal", "Time"};
  lv_table_set_cell_value(table, 0, 0, headers[0]);
  lv_table_set_cell_value(table, 0, 1, headers[1]);
  lv_table_set_cell_value(table, 0, 2, headers[2]);
  lv_table_set_cell_value(table, 0, 3, headers[3]);

  
    lv_table_set_row_cnt(table, get_target(TC::ROUNDS) + 1); // +1 for header row
  // RESET BUTTON
  
  lv_obj_t * button_stop = lv_button_create(scr_Results);
  lv_obj_set_pos(button_stop, 20,205);
  lv_obj_set_size(button_stop,280,30);
  lv_obj_remove_flag(button_stop, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_add_event_cb(button_stop, button_reset_cb, LV_EVENT_PRESSED, NULL);
  
  lv_obj_t * label_stop = lv_label_create(button_stop);
  lv_label_set_text(label_stop, "RESET!");
  lv_obj_center(label_stop);

}