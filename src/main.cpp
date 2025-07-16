#include <lvgl.h>
#include <Arduino.h>

#include "setuplv.h"
#include "TripComputer.h"
#include "UI_MainMenu.h"
#include "UI_update.h"
#include "UI_Results.h"
#include "UI_countdown.h"
// #include "BLE.h"


const long interval = 1000;
unsigned long previousMillis = 0;
int count = 0;

// DATA FOR ROTATION TIMER SETUP
////////////////////////
unsigned long  last_millis;
bool contact=false;
bool inuse=false;
unsigned long  timer;
int reedval;

void setup() {
  
  Serial.begin(115200);
  my_lv_begin(); 
  lv_create_MainMenu_2(); 
  lv_create_timer();
  lv_create_Results();
  lv_create_countdown();

  lv_load_mainmenu(true);

  pinMode(reed, INPUT_PULLUP);
  contact = false;

  
	//BLEDevice::init("IndoorBike"); // name of the ble device
	// InitBLEServer();
  //connectToServer();
  // 

}

void loop() {
  lv_task_handler();  // let the GUI do its work
  lv_tick_inc(5);     // tell LVGL how much time has passed
  delay(5);           // let this time pass
  
  if (is_running() ) {
    run_timer();       // in tripcomputer
    reedval = digitalRead(reed);
    

    if (contact == false) {
     
      if (reedval == LOW) {

        contact = true;
        timer = millis() - last_millis;

        if (timer > 50) {
          last_millis = millis();

          float rpm = 60 / (timer / 1000.0);
          
          update_rpm(rpm);
          add_metres(8.045);
          add_calories( max(0.0, timer / 1000.0 * ((0.00000005073 * rpm * rpm * rpm) + (0.00008806 * rpm * rpm) + (-0.00043782 * rpm))) );

          check_progress();  // in tripcomputer

          inuse = true;
        }
      }
    }
    if (reedval == HIGH) {

      contact = false;
      if (inuse) {
        if (millis() - last_millis > 3000) {
          update_rpm(0.0);
          inuse = false;
        }
      }
    }


  }

}