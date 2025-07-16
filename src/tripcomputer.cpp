#include "tripcomputer.h"

#include "UI_update.h"
#include "UI_Results.h"
#include "UI_countdown.h"
#include "UI_MainMenu.h"

#include <vector>

static int  TargetDist = 250;
static int  TargetCals = 15;
static int  TargetRounds = 3;
static int  TargetRoundTime = 60;
static int  countdownPeriod_s = 10;

static TC::TIMER_DATA Target_type = TC::CALS ;
static TC::BIKE_MODE Bike_mode = TC::JUST_RIDE ;
static bool RUNNING = false;
static bool WORKING = false;
static bool COUNTING = false;
static bool INITIAL_CONTACT = false;

static long total_time_millis=0;
static long start_time_millis=0;
static long last_seconds = 0;

static long countdown_time_millis=0;
static long countdown_start_time_millis=0;
static long countdown_last_Seconds=0;


//std::vector<Round> exerciseRounds; 
int roundCount = 0;
float TC_Cals;
double TC_metres;
long TC_seconds_round;
long TC_seconds_work;


bool is_running(void) {
  return RUNNING;
}

bool is_working(void) {
  return WORKING;
}
void start_running() {
    RUNNING = true;
    // WORKING = false;
    lv_create_Results();
    roundCount = 0;
    new_round();
}

void stop_running() {
    RUNNING = false;
    // WORKING = false;
}

void new_round() {
    
    TC::BIKE_MODE bm = get_bike_mode();

    TC_Cals=0;
    TC_metres=0;
    TC_seconds_round=0;
    TC_seconds_work=0;
    
    roundCount += 1;
    if (Bike_mode==TC::DEATH_BY) {
        TargetCals = roundCount;
        update_pb_target (TargetCals,10, TC::TARGET_CALS);
    }
    
    last_seconds = 0;

    if ( (bm==TC::EMOM || bm==TC::DEATH_BY) ) {
        start_time_millis = millis(); // move to set when WORKING begins!
        WORKING=true;
        Serial.println("Started WORKING");
    }

    update_time(TC_seconds_round);
    update_dist(TC_metres);
    update_cals(TC_Cals);
    update_round(roundCount,TargetRounds);

    Serial.println("started Running");

    INITIAL_CONTACT = ( digitalRead(reed) == LOW );
}


void run_timer()
{
   if (RUNNING && WORKING) {
    TC_seconds_round= ( millis()-start_time_millis ) / 1000;
    if (TC_seconds_round>last_seconds) {
        last_seconds=TC_seconds_round;
        update_time(TC_seconds_round);
    }
   }   
   if (COUNTING) {
    
    int CD_seconds_round= ( millis()-countdown_start_time_millis ) / 1000;
    int CD_left = countdownPeriod_s - CD_seconds_round;
    if (CD_left != countdown_last_Seconds) {
        countdown_last_Seconds = CD_left;
        update_countdown(CD_left);
        if (CD_left==0) {  // cpuntdown finished
            Serial.println("countdown done");
            COUNTING=false;
            if (get_bike_mode()==TC::RFT) {
                new_round();
                update_rpm(0);
                lv_load_timer();
            }
        }
    }


   }
}

void check_progress() {

  TC::BIKE_MODE bm = get_bike_mode();
   if (INITIAL_CONTACT) {
    INITIAL_CONTACT = false;
    return;
   }

  if (bm==TC::RFT && WORKING==false && COUNTING==false) {  // for FT workouts, start the timer when you start to pedal
        Serial.println("Started WORKING");
        start_time_millis = millis(); 
        WORKING=true;
  }


  if (bm==TC::RFT || bm==TC::EMOM || bm==TC::DEATH_BY ) {

    //has distance or cals target been met?
    if (WORKING && ((TC_metres>=TargetDist && get_target_type()==TC::DIST) || (TC_Cals>=TargetCals && get_target_type()==TC::CALS) ) ) {
      TC_seconds_work = TC_seconds_round;
      add_results_row(roundCount,TC_metres, TC_Cals, TC_seconds_work);
      WORKING=false;

      //enter FT countdown?
      if (bm==TC::RFT && COUNTING==false) {
        COUNTING = true;
        countdown_start_time_millis = millis();
        countdown_last_Seconds = countdownPeriod_s;
        Serial.println("Started countdown");
        lv_load_countdown();
        update_countdown(countdown_last_Seconds);
      }

      //have all rounds been done
      if ((bm==TC::RFT || bm==TC::EMOM) && roundCount==TargetRounds )  {
        RUNNING=false;
        load_results_screen();
      }
      else {
        WORKING=false;
      }
    }   

    //has round time been met?
    if ((bm==TC::EMOM || bm==TC::DEATH_BY) && TC_seconds_round==TargetRoundTime) {
        if (WORKING)  {
            add_results_row(roundCount,TC_metres, TC_Cals, TC_seconds_round);
            if (bm==TC::DEATH_BY) {
                RUNNING=false;
                load_results_screen();
            }
        }
        new_round();
    }
  }
}

void add_metres(double metrestoadd) { 
    TC_metres+=metrestoadd;
    update_dist(TC_metres);
}

void add_calories(float calstoadd) {
    TC_Cals+=calstoadd;
    update_cals(TC_Cals);
}

double get_metres() {return TC_metres;};
float get_calories() {return TC_Cals;};
long get_seconds() {return TC_seconds_round;};
int get_rounds() {return roundCount; };

Round get_round_data(int roundnum) {
    Round ret;
    ret.cals = TC_Cals;
    ret.metres = TC_metres;
    ret.seconds_work = TC_seconds_work;
    ret.seconds_round = TC_seconds_round;
    return ret;
}

// TARGETS & MODES

int get_target(TC::TIMER_DATA target ) {
    switch (target) {
        case TC::CALS: {
            return TargetCals; 
            break;}
        case TC::DIST: {
            return TargetDist;
            break;}
        case TC::TIME: {
            return TargetRoundTime;
            break;}
        case TC::ROUNDS: {
            return TargetRounds;
            break;}
        default:
            return 0;
    }
}

void set_target(TC::TIMER_DATA target, int value) {
    switch (target) {
        case TC::CALS: {
            TargetCals = value;
            Serial.printf("Target Cals set to: %d \n", value);
            break;} 
        case TC::DIST: {
             TargetDist = value;
            Serial.printf("Target Dist set to: %d \n", value);
            break;}
        case TC::TIME: {
             TargetRoundTime = value;
            Serial.printf("Target Dist set to: %d \n", value);
            break;}        
        case TC::ROUNDS: {
             TargetRounds = value;
            Serial.printf("Target Dist set to: %d \n", value);
            break;}
    }
}

TC::TIMER_DATA get_target_type() {
    return Target_type;
};

void set_target_type(TC::TIMER_DATA TT) {
    Target_type = TT;
    Serial.printf("Target Type set to: %d \n", TT);
};


void set_bike_mode(TC::BIKE_MODE BM) {  
    Bike_mode = BM; 
    Serial.printf("Mode to: %d \n", BM);
    }


TC::BIKE_MODE get_bike_mode() { return Bike_mode; }