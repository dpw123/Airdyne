#ifndef TC_H
#define TC_H




namespace TC { 
enum TIMER_DATA {
    TIME,
    DIST,
    CALS,
    RPM,
    ProgBar,
    ROUNDS,
    REST_TIME
    };
enum TARGET_TYPE {
    TARGET_TIME,
    TARGET_ROUNDS,
    TARGET_DIST,
    TARGET_CALS
    };
enum BIKE_MODE {
    JUST_RIDE,
    RFT,
    EMOM,
    DEATH_BY,
    TABATA
    };
}

struct Round {
    double metres {0}; // distance covered in round
    float cals {0}; // calories in round
    long seconds_work {0}; // time for the round target to be met
    long seconds_round {0}; // time for the round target to be met
};



// static long FT_completed_in;

// // DATA FOR TRIP COMPUTER
// static double metres = 0;
// static float calories = 0;
// static long seconds = 0;

// functions to return data
bool is_running(void);
bool is_working(void);

TC::TIMER_DATA get_target_type();
void set_target_type(TC::TIMER_DATA);

int get_target(TC::TIMER_DATA target );
void set_target(TC::TIMER_DATA target, int value);

void start_running();
void stop_running();
void tc_clear();
void check_progress();
void new_round();

void set_bike_mode(TC::BIKE_MODE BM);
TC::BIKE_MODE get_bike_mode();

void add_metres(double metrestoadd);
void add_calories(float calstoadd);
void run_timer();

// void set_FT_completed_in(long timer);
// long get_FT_completed_in();

double get_metres();
float get_calories();
long get_seconds();
int get_rounds();
Round get_round_data(int roundnum);


const int reed = 22;



#endif