//https://arduino-pico.readthedocs.io/en/latest/install.html#installing-via-arduino-boards-manager
// make sure you follow here to install the PICO2W board

//in Tools I have IP/Bluetooth stack set to IPv4 + Bluetooth
//in Tools I have Flash size set to 4MB(Sketch 2MB FS 2MB )

//This is the manufactured board I'm working with. This version of the code is meant to work on this board.
//The base for this code is taken from: 
//https://github.com/noodalls/2026-PICO2W-testing-setup/blob/36da2963b38cb11dc18ddafbd6cf8014119c9f06/Arduino%20IDE%20sketch
//Version of board is here: https://oshpark.com/shared_projects/scDfyYnd
//Phototransistor board is here: https://oshpark.com/shared_projects/rQflS270


//playlist showing soldering and setup is here
//https://www.youtube.com/playlist?list=PLevqZFjlWIdexYMa2yQZgw0dfLVjzN8m7

//This version backports web server and interface that was added added for the pinout in newer revisions.

#include "LittleFS.h"
#include <SerialBT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>          
int wifimode = 0;

String ssid = "fubarduck";
String password = "noodalls";

String whole_url = "";
int detailed_graphics = 0;

WiFiServer server(80);
float website_refresh = 30;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String OLED_message1 = "";
String OLED_message2 = "";
String OLED_message3 = "";
String OLED_message4 = "";
String OLED_message5 = "";
String OLED_message6 = "";
String OLED_message7 = "";
int warning_message_issued = 0;
int mode = 0;
int cursor_position = 0;
int initial_cursor_position = 0;
int hold_on_startup = -5;

int youvegotmail = 0;
String youvegotemail = "0.0";

int int_val = 0;
float float_val = 0.0;

char abc=' ';
uint8_t OPT1_decimal[10000];
uint8_t OPT2_decimal[10000];

int OPT1_summary[255];
int OPT2_summary[255];

int OPT1_current = 0;
int OPT2_current = 0;
float OPT1_results[1000];
float OPT2_results[1000];

float OPT1_fastest = 0.0;
float OPT2_fastest = 0.0;
float OPT1_slowest = 999.9;
float OPT2_slowest = 999.9;

int save_mode = 0;
int calculate_mode = 0;
int fastest_slowest_mode = 1;
int only_one_test = 0;

float upper = 8;
float lower = 48;
float top_of_screen = 17;
float bottom_of_screen = 354;

float current_temp = 0.0;
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long test_time=0;

unsigned long elapsed_time = 0;
unsigned long elapsed_time_ms = 0;

unsigned long timea = 0;
unsigned long timeb = 0;
unsigned long timec = 0;

unsigned long OPT1_sum = 0;
unsigned long OPT2_sum = 0;
int OPT1_count = 0;
int OPT2_count = 0;

float OPT1_avg = 0.0;
float OPT2_avg = 0.0;

float OPT1_top_a = 0.0;
float OPT2_top_a = 0.0;
float OPT1_bottom_a = 0.0;
float OPT2_bottom_a = 0.0;
float OPT1_top_b = 0.0;
float OPT1_bottom_b = 0.0;
float screen_draw_time_known = 8.33;
float screen_hz_known = 120;
float screen_draw_time_calculated = 8.33;
float screen_hz_calculated = 120;

int over = 2;
int UNDER = 2;
int player = 2;
int PLAY1P2P = 4;

int trial_number = 0;
int TRIAL_MAX = 300;

int read_time = 100;
int REPORT_TIME = 250;
int query_time = 100;
int QUIT_TIME = 350;

int OPT1 = 27;
int OPT2 = 28;

float OPT1_time = 0;
float OPT2_time = 0;
int OPT1_min = 255;
int OPT1_max = 0;
int OPT2_min = 255;
int OPT2_max = 0;

int P1_wins = 0;
int P2_wins = 0;
int P1_P2_draw = 0;

int startPos = 0;
int endPos = 0;

int LEFT = 8;
int RIGHT = 9;
int CONFIRM = 6;
int CANCEL = 7;

int RED_LED = 6;
int GREEN_LED = 8;
int BLUE_LED = 9;

int PPS = 22;

int external_trigger = 0;

int current_button = 0;
int last_button = 0;

String results = "";

int OUTPUT_ON_TIME[] = { -5, -5, -5, -5, -5, -5, -5, -5, 100, -5, -5, -5, 100, 101 };
int output_off_time[] = {0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0,  50,  50};
int OUTPUT_ON_STATE[]    = {0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1};
int output_off_state[]   = {1,1,1,1, 1,1,1,1, 1,1,1,1, 0,0};
int output_reset_state[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1};
int OUTPUT_BUTTONS[] = { 10, 21, 11, 20, 19, 12, 18, 13, 14, 17, 15, 16, 2, 3 };


String serial_titles[] = { "UP_ON", "DOWN_ON", "RIGHT_ON", "LEFT_ON", "1P_ON", "1P_ON", "3P_ON", "4P_ON", "1K_ON", "2K_ON", "3K_ON", "4K_ON", "OPT1_OFF", "OPT2_OFF",
                           "up_off", "down_off", "right_off", "left_off", "1p_off", "2p_off", "3p_off", "4p_off", "1k_off", "2k_off", "3k_off", "4k_off", "opt1_on", "opt2_on",
                           "trial_number", "TRIAL_MAX",  "read_time", "REPORT_TIME", "query_time", "QUIT_TIME", "over", "under", "players", "1P2PCOMP", "Initial cursor", "fastest_slowest_mode","only_one_test","calculate_mode","save_data" };

String float_serial_titles[] = { "upper_value", "lower_value","top_of_screen", "bottom_of_screen","screen_hz", "screen_time_ms", };

char serial_names[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        't', 'T',  'r', 'R', 'q', 'Q', 'o', 'O', 'p', 'P', 'z', '!', '@', '_', '`' };

char float_serial_names[] = { 'w', 'W','s','S','x', 'X', };

int min_list[] = { -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -1, -1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, -5, 0, 0 };

int max_list[] = { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 999, 99, 99, 1999, 1999, 1999, 1999, 50, 50, 2, 0, 54, 1, 1 };


int *pointers[] = { &OUTPUT_ON_TIME[0], &OUTPUT_ON_TIME[1], &OUTPUT_ON_TIME[2], &OUTPUT_ON_TIME[3], &OUTPUT_ON_TIME[4], &OUTPUT_ON_TIME[5], &OUTPUT_ON_TIME[6],
                    &OUTPUT_ON_TIME[7], &OUTPUT_ON_TIME[8], &OUTPUT_ON_TIME[9], &OUTPUT_ON_TIME[10], &OUTPUT_ON_TIME[11], &OUTPUT_ON_TIME[12], &OUTPUT_ON_TIME[13],

                    &output_off_time[0], &output_off_time[1], &output_off_time[2], &output_off_time[3], &output_off_time[4], &output_off_time[5], &output_off_time[6],
                    &output_off_time[7], &output_off_time[8], &output_off_time[9], &output_off_time[10], &output_off_time[11], &output_off_time[12], &output_off_time[13],

                    &trial_number, &TRIAL_MAX, 
                    &read_time, &REPORT_TIME, &query_time, &QUIT_TIME,
                    &over, &UNDER, &player, &PLAY1P2P, &initial_cursor_position, &fastest_slowest_mode, &only_one_test, &calculate_mode, &save_mode };

float *float_pointers[] = { &upper, &lower, &top_of_screen, &bottom_of_screen, &screen_hz_known, &screen_draw_time_known,};

int length_array = sizeof(serial_names) / sizeof(serial_names[0]);
int float_length_array = sizeof(float_serial_names) / sizeof(float_serial_names[0]);

void prepare_summary(){
  for (int i = 0; i < 255; i++) {
    OPT1_summary[i] = 0;
    OPT2_summary[i] = 0;
  }
  File writesummary = LittleFS.open("/summary.txt","w");
    for (int i = 0; i < length_array; i++) {
    writesummary.print(serial_names[i]);
    writesummary.println(*pointers[i]);
  }
  for (int i = 0; i < float_length_array; i++) {
    writesummary.print(float_serial_names[i]);
    writesummary.println(*float_pointers[i]);
  }
  
  writesummary.close();
}

void connect_sub_board(){
            OUTPUT_ON_STATE[0] = 1; OUTPUT_ON_STATE[1] = 1; OUTPUT_ON_STATE[2] = 1; OUTPUT_ON_STATE[3] = 1;
          output_off_state[0]=0; output_off_state[1]=0; output_off_state[2]=0; output_off_state[3]=0;
          output_reset_state[0]=0; output_reset_state[1]=0; output_reset_state[2]=0; output_reset_state[3]=0;
}

void save_summary(){
  for (int i=0;i < 255; i++){
    OPT1_summary[i] = OPT1_decimal[i * 10];
    OPT2_summary[i] = OPT2_decimal[i * 10];
  }

  File writeraw = LittleFS.open("/raw.txt", "a");
  writeraw.print("u");  
  writeraw.print(trial_number);
  writeraw.print("=");
  for (int i = 0; i < 255; i++) {
    writeraw.print(OPT1_summary[i]);
    writeraw.print(",");
  }
  writeraw.println();
  writeraw.print("v");  
  writeraw.print(trial_number);
  writeraw.print("=");
  for (int i = 0; i < 255; i++) {
    writeraw.print(OPT2_summary[i]);
    writeraw.print(",");
  }
  writeraw.println();

  writeraw.close();

  for (int i = 0; i < 255; i++) {
    OPT1_summary[i] = 0;
    OPT2_summary[i] = 0;
  }
}

void turn_off_buttons() {
  
  if (cursor_position < 0) {
    for (int i = 0; i < 14; i++) {
      digitalWrite(OUTPUT_BUTTONS[i], output_reset_state[i]);
    }
  }

  if (cursor_position  > 0) {
    for (int i = 0; i < 14; i++) {
      if (output_off_time[i] >= 0) { digitalWrite(OUTPUT_BUTTONS[i], output_off_state[i]); }
      if (output_off_time[i] < 0) { digitalWrite(OUTPUT_BUTTONS[i], OUTPUT_ON_STATE[i]); }
      if (output_off_time[i] < OUTPUT_ON_TIME[i]) { digitalWrite(OUTPUT_BUTTONS[i], OUTPUT_ON_STATE[i]); }
    }
  }
  if (cursor_position == 0) {
    for (int i = 0; i < 14; i++) {
      digitalWrite(OUTPUT_BUTTONS[i], output_reset_state[i]);
    }
  }
}

void setColorBGR(int _blue, int _green, int _red) {
  digitalWrite(BLUE_LED, _blue);
  digitalWrite(GREEN_LED, _green);
  digitalWrite(RED_LED, _red);
}

void calculate_screen_time() {

  OPT1_top_a = 0.0;
  OPT2_top_a = 0.0;
  OPT1_bottom_a = 0.0;
  OPT2_bottom_a = 0.0;
  OPT1_top_b = 0.0;
  OPT1_bottom_b = 0.0;

   //known hz
    OPT1_top_a = OPT1_avg - screen_draw_time_known * upper / 100;
    OPT2_top_a = OPT2_avg - screen_draw_time_known * lower / 100;
    OPT1_bottom_a = OPT1_top_a + screen_draw_time_known;
    OPT2_bottom_a = OPT2_top_a + screen_draw_time_known;
    
    if ((OPT1_avg != OPT2_avg) && (OPT1_avg != 0) && (OPT2_avg != 0)) {

      screen_draw_time_calculated = 100 * (OPT2_avg - OPT1_avg) / (lower - upper);

      screen_hz_calculated = 1000 / screen_draw_time_calculated;

      OPT1_top_b = OPT1_avg - screen_draw_time_calculated * upper / 100;      
      OPT1_bottom_b = OPT1_top_b + screen_draw_time_calculated;

    }
  
}

void calculate_winners(){
  P1_wins = 0;
  P2_wins = 0;
  P1_P2_draw = 0;


for (int i = 0; i < 1000; i++) {
if ((OPT1_results[i] != 0) &&  (OPT2_results[i] != 0)){
  if ( (OPT2_results[i] - OPT1_results[i]) > PLAY1P2P){P1_wins++;}
  else if ( (OPT1_results[i] - OPT2_results[i]) > PLAY1P2P){P2_wins++;}
  else {P1_P2_draw++;}
}
}
}


void display_screen() {

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  if (cursor_position == -10) { display.println("Temp"); }
  if (cursor_position == -7) { display.println("Test time"); }
  if (cursor_position == -6) { display.println("Print summary"); }
  if (cursor_position == -5) { display.println("Review"); }
  if (cursor_position == -4) { display.println("Delete all"); }
  if (cursor_position == -3) { display.println("Print all"); }
  if (cursor_position == -2) { display.println("Print one"); }
  if (cursor_position == -11) { display.println("Controller variables"); }
  if (cursor_position == -12) { display.println("Integer variables"); }
  if (cursor_position == -13) { display.println("Float variables"); }
  
  if (cursor_position == 0) { display.println("RESET"); }

  if (cursor_position == 1){display.print("OPT1");}
  if (cursor_position == 2){display.print("OPT1");}
  if (cursor_position == 3){display.print("OPT2");}
  if (cursor_position == 4){display.print("OPT2");}
  if (cursor_position == 5) { display.println("Screen time known"); }
  if (cursor_position == 6) { display.println("Screen time calc"); }
  if (cursor_position == 7) { display.println("Compare 1P2P"); }  
  if (cursor_position == 8) { display.println("Non PT tests"); }  
  if (cursor_position == 9) { display.println("Play test"); }
  if (cursor_position == 10) { display.println("Play test max"); }  

  if (cursor_position == 11) { display.println("Review"); }

if (cursor_position == 0){
    display.setCursor(0, 8);display.print(OLED_message1);
    display.setCursor(0,16);display.print(OLED_message2);
    display.setCursor(0,24);display.print(OLED_message3);
    display.setCursor(0,32);display.print(OLED_message4);
    display.setCursor(0,40);display.print(OLED_message5);
    display.setCursor(0,48);display.print(OLED_message6);
    display.setCursor(0,56);display.print(OLED_message7);
}

  if (fastest_slowest_mode == 1){display.setCursor(120,0);display.print("!");}
    if (only_one_test == 1){display.setCursor(116,0);display.print("@");}
    if (save_mode == 1){display.setCursor(112,0);display.print("`");}

if (cursor_position == 1||cursor_position ==2) {

    display.setCursor(0,8);display.print(OPT1_avg);
    display.setCursor(0,56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);
     display.setCursor(90, 56);display.print(OPT1_current);
    
    display.setTextSize(3);
    if (OPT1_results[trial_number] < 100){display.setTextSize(4);}

    display.setCursor(0,16);display.print(OPT1_results[trial_number]);display.setTextSize(1);
  }

    if (cursor_position ==3||cursor_position ==4) {
    
    display.setCursor(0,8);display.print(OPT2_avg);
  display.setCursor(110, 56);display.print(OPT2_current);
    display.setCursor(0,56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);
    
    display.setTextSize(3);
    if (OPT2_results[trial_number] < 100){
    display.setTextSize(4);}
    
    display.setCursor(0,16);display.print(OPT2_results[trial_number]);display.setTextSize(1);
  }


  if ( cursor_position == 5||cursor_position == 6||cursor_position == 7){
  display.setCursor(0,56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);
  display.print("(");display.print(OPT1_count);display.print("/");display.print(OPT2_count);display.print(")");
  
  display.setCursor(0, 16);display.print(OPT1_results[trial_number]);display.print("/");display.print(OPT1_avg);
  display.setCursor(96, 16);display.print("P1/av");
  display.setCursor(0, 24);display.print(OPT2_results[trial_number]);display.print("/");display.print(OPT2_avg);
  display.setCursor(96, 24);display.print("P2/av");

  display.setCursor(90, 56);display.print(OPT1_current);
  display.setCursor(110, 56);display.print(OPT2_current);
}
  

  if (cursor_position == 5 || cursor_position == 6) {
    display.setCursor(96, 32);display.print("P1/\\/");
    display.setCursor(96, 40);display.print("P2/\\/");
    display.setCursor(96, 48);display.print("hz/ms");
    
    display.setCursor(32, 8);display.print(upper,0);
    display.setCursor(96, 8);display.print(lower,0);
  }
    
  if (cursor_position == 5) {
    display.setCursor(0, 32);display.print(OPT1_top_a);display.print("/");display.print(OPT1_bottom_a);
    display.setCursor(0, 40);display.print(OPT2_top_a);display.print("/");display.print(OPT2_bottom_a);
    display.setCursor(0, 48);display.print(screen_hz_known);display.print("/");display.print(screen_draw_time_known);
    }

  
  if (cursor_position == 6) {
    display.setCursor(0, 32);display.print(OPT1_top_b);display.print("/");display.print(OPT1_bottom_b);
    display.setCursor(0, 40);display.print(OPT1_top_b);display.print("/");display.print(OPT1_bottom_b);
    display.setCursor(0, 48);display.print(screen_hz_calculated);display.print("/");display.print(screen_draw_time_calculated);
       
  }

    if (cursor_position == 7){

    display.setCursor(0,32);display.print(OPT1_top_a-OPT2_top_a);

    display.setCursor(0,40);display.print(P1_wins);
    display.setCursor(32,40);display.print(P1_P2_draw);
    display.setCursor(64,40);display.print(P2_wins);
    display.setCursor(96, 40);display.print("<=>");
    display.setCursor(120, 40);display.print(PLAY1P2P);
  }


    if ( cursor_position == 8||cursor_position ==9){
    display.setCursor(0,56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);}
  
  if (cursor_position == -11) {
    display.setCursor(0, 8 );for (int i = 0; i < 4; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    display.setCursor(0, 16);for (int i = 4; i < 8; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    display.setCursor(0, 24);for (int i = 8; i < 12; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    display.setCursor(0, 32);for (int i = 14; i < 18; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    display.setCursor(0, 40);for (int i = 18; i < 22; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    display.setCursor(0, 48);for (int i = 22; i < 26; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}  
    display.setCursor(0, 56);for (int i = 12; i < 14; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    for (int i = 26; i < 28; i++) {display.print(serial_names[i]);display.print(*pointers[i]);display.print(" ");}
    
  }

    if (cursor_position == -12) {
    display.setCursor(0, 16);
    for (int i = 28; i < length_array; i++) {
      display.print(serial_names[i]);
      display.print(*pointers[i]);
      display.print(" ");
    }
  }

    if (cursor_position == -13) {
    display.setCursor(0, 16);

    for (int i = 0; i < float_length_array; i++) {
      display.print(float_serial_names[i]);
      display.print(*float_pointers[i]);
      display.print(" ");
    }
  }



  if (cursor_position == -2 || cursor_position == -3 || cursor_position == -4){
    display.setCursor(0,16);
    display.print(results);
  } 

  if (cursor_position == -10) {
    display.clearDisplay();
    current_temp = analogReadTemp(); 
    display.setTextSize(4);
    display.setCursor(0,16);
    display.print(current_temp);
    delay(500);    
    display.setTextSize(1);
  } 

    if (cursor_position == -7) {
    display.setTextSize(3);
    display.setCursor(0,24);
    display.print(test_time);    
    display.setTextSize(1);
  }


  display.display();
}




void calculate_times() {
  OPT1_min = 255;
  OPT1_max = 0;
  OPT1_time = 0.0;
  OPT2_min = 255;
  OPT2_max = 0;
  OPT2_time = 0.0;

  for (int i = 0; i < read_time * 10; i++) {
    if ((OPT1_decimal[i] < OPT1_min) && (OPT1_decimal[i] != 0)) { OPT1_min = OPT1_decimal[i]; }
    if ((OPT2_decimal[i] < OPT2_min) && (OPT2_decimal[i] != 0)) { OPT2_min = OPT2_decimal[i]; }
    if ((OPT1_decimal[i] > OPT1_max) && (OPT1_decimal[i] != 0)) { OPT1_max = OPT1_decimal[i]; }
    if ((OPT2_decimal[i] > OPT2_max) && (OPT2_decimal[i] != 0)) { OPT2_max = OPT2_decimal[i]; }
  }

  for (int i = query_time * 10; i < REPORT_TIME * 10; i++) {

    if ((over >= 0) && (OPT1_time == 0.0) && (OPT1_decimal[i] > OPT1_max + over) && (OPT1_decimal[i + 1] > OPT1_max + over) && (OPT1_decimal[i + 2] > OPT1_max + over) && (OPT1_decimal[i] != 0))
    {
      OPT1_time = i;
      OPT1_results[trial_number] = float((OPT1_time) / 10 - read_time);
    }

    if ((over >= 0) && (OPT2_time == 0.0) && (OPT2_decimal[i] > OPT2_max + over) && (OPT2_decimal[i + 1] > OPT2_max + over) && (OPT2_decimal[i + 2] > OPT2_max + over) && (OPT2_decimal[i] != 0)) {
      OPT2_time = i;
      OPT2_results[trial_number] = float((OPT2_time) / 10 - read_time);
    }

    if ((UNDER >= 0) && (OPT1_time == 0.0) && (OPT1_decimal[i] < OPT1_min - UNDER) && (OPT1_decimal[i + 1] < OPT1_min - UNDER) && (OPT1_decimal[i + 2] < OPT1_min - UNDER) && (OPT1_decimal[i] != 0)) {
      OPT1_time = i;
      OPT1_results[trial_number] = float((OPT1_time) / 10 - read_time);
    }

    if ((UNDER >= 0) && (OPT2_time == 0.0) && (OPT2_decimal[i] < OPT2_min - UNDER) && (OPT2_decimal[i + 1] < OPT2_min - UNDER) && (OPT2_decimal[i + 2] < OPT2_min - UNDER) && (OPT2_decimal[i] != 0)) {
      OPT2_time = i;
      OPT2_results[trial_number] = float((OPT2_time) / 10 - read_time);
    }
  }
}



void calculate_averages() {
  OPT1_sum = 0;
  OPT1_count = 0;
  OPT1_avg = 0.0;
  OPT2_sum = 0;
  OPT2_count = 0;
  OPT2_avg = 0.0;
  OPT1_fastest = 0.0;
  OPT2_fastest = 0.0;
  OPT1_slowest = 999.9;
  OPT2_slowest = 999.9;
  for (int i = 0; i < TRIAL_MAX; i++){
    if (OPT1_results[i] > OPT1_fastest){OPT1_fastest = OPT1_results[i];}
    if ((OPT1_results[i] != 0) && (OPT1_results[i] < OPT1_slowest)){OPT1_slowest = OPT1_results[i];}
    if (OPT2_results[i] > OPT2_fastest){OPT2_fastest = OPT2_results[i];}
    if ((OPT2_results[i] != 0) && (OPT2_results[i] < OPT2_slowest)){OPT2_slowest = OPT2_results[i];}
  }
  for (int i = 0; i < 1000; i++) {
    if (OPT1_results[i] != 0) {
      OPT1_sum = OPT1_sum + OPT1_results[i];
      OPT1_count++;
    }
    if (OPT2_results[i] != 0) {
      OPT2_sum = OPT2_sum + OPT2_results[i];
      OPT2_count++;
    }
  }
  if (fastest_slowest_mode == 0){
    if (OPT1_count != 0) { OPT1_avg = (float(OPT1_sum) / float(OPT1_count)); }
    if (OPT2_count != 0) { OPT2_avg = (float(OPT2_sum) / float(OPT2_count)); }
  }
  if (fastest_slowest_mode == 1){
    if (OPT1_count != 0) { OPT1_avg = ((OPT1_fastest+OPT1_slowest)/2); }
    if (OPT2_count != 0) { OPT2_avg = ((OPT2_fastest+OPT2_slowest)/2); }
  }
}


void create_sheet(){
  results = "";
  results = results+",,,";
  
  if (calculate_mode == 0){results=results+"Phototransistor - known hz,";}
  else if(calculate_mode == 1){results=results+"Phototransistor - calculated,";}
  else if(calculate_mode == 2){results=results+"Phototransistor - compare,";}
  else{results=results+",";}
  if (calculate_mode == 1){
    results = results + OPT1_top_b;  results = results + ",";
    results = results + OPT1_top_b;  results = results + ",";
  }
  else{
  results = results + OPT1_top_a;  results = results + ",";
  results = results + OPT2_top_a;  results = results + ",";
  }

  results = results + OPT1_avg;  results = results + ",";
  results = results + OPT2_avg;  results = results + ",";

  results = results + OPT1_count;  results = results + ",";
  results = results + OPT2_count;  results = results + ",";

  results = results + ",,,,,,,,,,,,,,,";

  results = results + upper;results = results+",";
  results = results + lower;results = results+",";

results = results + ",,,,,";

  results = results + TRIAL_MAX;  results = results + ",";
  results = results + TRIAL_MAX;  results = results + ",";

  results = results + screen_hz_known;results = results + ",";
  results = results + screen_draw_time_known;results = results + ",";
  results = results + OPT1_top_a;results = results + ",";
  results = results + OPT1_bottom_a;results = results + ",";
  results = results + OPT2_top_a;results = results + ",";
  results = results + OPT2_bottom_a;results = results + ",";
  
  results = results + screen_hz_calculated;results = results + ",";
  results = results + screen_draw_time_calculated;results = results + ",";
  results = results + OPT1_top_b;results = results + ",";
  results = results + OPT1_bottom_b;results = results + ",";

  results = results + P1_wins;results = results + ",";
  results = results + P2_wins;results = results + ",";
  results = results + P1_P2_draw;results = results + ",";
  
}
  



void reset_all_values() {
  File writeraw = LittleFS.open("/raw.txt", "w");
  writeraw.close();

  trial_number = 0;
  for (int i = 0; i < 10000; i++) {
    OPT1_decimal[i] = 0;
    OPT2_decimal[i] = 0;
  }
  for (int i = 0; i < 1000; i++) {
    OPT1_results[i] = 0;
    OPT2_results[i] = 0;
  }
  trial_number = 0;
}

void reset_OPT_values() {
  OPT1_min = 255;
  OPT1_max = 0;
  OPT2_min = 255;
  OPT2_max = 0;
}

void save_settings() {
  File writefile = LittleFS.open("/settings.txt", "w");
  for (int i = 0; i < length_array; i++) {
    writefile.print(serial_names[i]);
    writefile.println(*pointers[i]);
  }
  for (int i = 0; i < float_length_array; i++) {
    writefile.print(float_serial_names[i]);
    writefile.println(*float_pointers[i]);
  }
  writefile.close();
  Serial.println("Saved");
}


void load_settings() {
  File readfile = LittleFS.open("/settings.txt", "r");
  if (readfile) {
    Serial.print("starting");
    while (readfile.available()) {
      String line = readfile.readStringUntil('\n');
      for (int i = 0; i < length_array; i++) {
        if (line[0] == serial_names[i]) {
          line.remove(0, 1);
          int temp_int = line.toInt();
          *pointers[i] = temp_int;
          Serial.print(serial_names[i]);
          Serial.println(*pointers[i]);
        }
      }

      for (int i = 0; i < float_length_array; i++) {
        if (line[0] == float_serial_names[i]) {
          line.remove(0, 1);
          float temp_float = line.toFloat();
          *float_pointers[i] = temp_float;
          Serial.print(float_serial_names[i]);
          Serial.println(*float_pointers[i]);
        }
      }
    }
    readfile.close();
    Serial.println("Loaded");
  }
}

void inbox(){
  if (youvegotmail !=0){
  if (char(youvegotmail) == '$'){ current_button = 1; }
  if (char(youvegotmail) == '%'){ current_button = 5; }
  if (char(youvegotmail) == '&'){ current_button = 4; }
  if (char(youvegotmail) == '*'){ current_button = 6; }

  if(char(youvegotmail) == 'y'){cursor_position = youvegotemail.toFloat();}
  if(char(youvegotmail) == 'Y'){cursor_position = youvegotemail.toFloat();current_button = 5;}
  
  if(char(youvegotmail) == '?'){
    for (int i = 0; i < length_array; i++) {
    SerialBT.print(serial_names[i]);
    SerialBT.println(*pointers[i]);
    Serial.print(serial_names[i]);
    Serial.println(*pointers[i]);}

      for (int i = 0; i < float_length_array; i++) {
    SerialBT.print(float_serial_names[i]);
    SerialBT.println(*float_pointers[i]);
    Serial.print(float_serial_names[i]);
    Serial.println(*float_pointers[i]);
    }}
  
  for (int i = 0; i < 14; i++){
  if(char(youvegotmail) == serial_names[i]){
    if((youvegotemail.toInt() >= min_list[i]) && (youvegotemail.toInt() <= max_list[i])){
    *pointers[i] = youvegotemail.toInt();
    SerialBT.print(serial_names[i]);SerialBT.println(*pointers[i]);}
    if (*pointers[i] == 0) {
    *pointers[i + 14] = -5;
    SerialBT.print(serial_names[i + 14]);SerialBT.println(*pointers[i + 14]);}
    save_settings();}
  }

  
  for (int i = 14; i < 28; i++) {
  if(char(youvegotmail) == serial_names[i]){
    if((youvegotemail.toInt() >= min_list[i]) && (youvegotemail.toInt() <= max_list[i])){
      *pointers[i] = youvegotemail.toInt();
      SerialBT.print(serial_names[i]);SerialBT.println(*pointers[i]);}
      if (*pointers[i] == 0) {
      *pointers[i - 14] = -5;
      SerialBT.print(serial_names[i - 14]);SerialBT.println(*pointers[i - 14]);}
      save_settings();
    }}

  for (int i = 28; i < length_array; i++) {  
    if(char(youvegotmail) ==  serial_names[i]) {
      if((youvegotemail.toInt() >= min_list[i]) && (youvegotemail.toInt() <= max_list[i])){
    *pointers[i] = youvegotemail.toInt();
    Serial.print(serial_names[i]);Serial.println(*pointers[i]);
    save_settings();}
  }}

    for (int i = 0; i < float_length_array; i++) {
    if (char(youvegotmail) == float_serial_names[i]) {
    *float_pointers[i] = youvegotemail.toFloat();
    if (upper < 0) { upper = 100 * (abs(upper) - top_of_screen) / (bottom_of_screen - top_of_screen); }
    if (lower < 0) { lower = 100 * (abs(lower) - top_of_screen) / (bottom_of_screen - top_of_screen); }
    SerialBT.print(float_serial_names[i]);SerialBT.println(*float_pointers[i]);
    save_settings();
    }}

youvegotmail = 0;
youvegotemail = "";
}
}


void setup() {

    for (int i = 0; i < 14; i++) {
    pinMode(OUTPUT_BUTTONS[i], OUTPUT);
    digitalWrite(OUTPUT_BUTTONS[i], HIGH);
  }

  Serial.begin(115200);
  delay(1000);

  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(CONFIRM, INPUT_PULLUP);
  pinMode(CANCEL, INPUT_PULLUP);

  pinMode(OPT1, INPUT);
  pinMode(OPT2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(A3,INPUT);
  LittleFS.begin();

  OPT1_current = analogRead(OPT1) / 4;
  OPT2_current = analogRead(OPT2) / 4;
  if (OPT1_current == 4){OLED_message4 = "Check OPT1";}
  if ((OPT2_current == 4) && (OLED_message4 == "Check OPT1")){OLED_message5 = "Check OPT2";}

  if (digitalRead(CANCEL) == 0) { save_settings(); cursor_position = -4; last_button = 1; }

  String defaultSsid = ssid;
  String defaultPassword = password;

  if (digitalRead(RIGHT) == 1){
   File password_load = LittleFS.open("/password.txt", "r");
   if (password_load) {
     Serial.print("starting");
     while (password_load.available()) {
       ssid = password_load.readStringUntil('\n');
       ssid.remove(ssid.length()-1);
       Serial.println("ssid");
       Serial.println(ssid);
       password = password_load.readStringUntil('\n');
       password.remove(password.length()-1);
       Serial.println("password");
       Serial.println(password);
       wifimode = 1;
   }}
  password_load.close();}


  if (digitalRead(RIGHT) == 0){wifimode = 0;}

  if (wifimode == 1){
   WiFi.begin(ssid.c_str(), password.c_str());
   int wifi_attempts = 0;
   while (WiFi.status() != WL_CONNECTED && wifi_attempts < 40) {
     delay(500);
     Serial.print(".");
     wifi_attempts++;
   }
   if (WiFi.status() != WL_CONNECTED) {
     Serial.println("\nWiFi connect failed, falling back to AP mode");
     ssid = defaultSsid;
     password = defaultPassword;
     wifimode = 0;
   } else {
     Serial.print("\nConnected! Open this IP: ");
     Serial.println(WiFi.localIP());
     OLED_message3 = WiFi.localIP().toString();
   }
  }

if (wifimode == 0){
WiFi.beginAP(ssid.c_str(), password.c_str());
Serial.println(WiFi.softAPIP());
OLED_message1 = ssid;
OLED_message2 = password; 
OLED_message3 = WiFi.softAPIP().toString();

}


  server.begin();


  load_settings();
  if (digitalRead(LEFT) == 0) { initial_cursor_position = -4; save_settings(); }
  if (digitalRead(LEFT) == 0) { connect_sub_board(); }
cursor_position = initial_cursor_position;

Serial.print(length_array);

  SerialBT.setName("noodalls  00:00:00:00:00:00");
  SerialBT.begin();
  SerialBT.setTimeout(200);
  Serial.setTimeout(200);




  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  trial_number = 0;


  for (int i = 0; i < 10000; i++) {
    OPT1_decimal[i] = 0;
    OPT2_decimal[i] = 0;
  }
  for (int i = 0; i < 255; i++) {
    OPT1_summary[i] = 0;
    OPT2_summary[i] = 0;
  }
}


void loop() {

  time1 = micros() / 100;
  if ((time1) != (time2)) {

    OPT1_current = analogRead(OPT1) / 4;
    OPT2_current = analogRead(OPT2) / 4;
    elapsed_time = (time1 - time3);
    if (OPT1_summary[OPT1_current] == 0) {OPT1_summary[OPT1_current] = elapsed_time; }
    if (OPT2_summary[OPT2_current] == 0) {OPT2_summary[OPT2_current] = elapsed_time; }
    elapsed_time_ms = elapsed_time / 10;
    if (digitalRead(CANCEL) == 0) {
      current_button = 1;
      last_button = 1;
      mode = 0;
      digitalWrite(RED_LED,LOW);digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,LOW);
      pinMode(LEFT, INPUT_PULLUP);pinMode(RIGHT, INPUT_PULLUP);pinMode(CONFIRM, INPUT_PULLUP);
    }


    if (mode == 1) {

      if (elapsed_time < 10000){
      OPT1_decimal[elapsed_time] = OPT1_current;
      OPT2_decimal[elapsed_time] = OPT2_current;
      }

      for (int i = 0; i < 14; i++) {
        if (elapsed_time_ms == OUTPUT_ON_TIME[i]) { digitalWrite(OUTPUT_BUTTONS[i], OUTPUT_ON_STATE[i]); }
        if (elapsed_time_ms == output_off_time[i]) { digitalWrite(OUTPUT_BUTTONS[i], output_off_state[i]); }
      }

      if (elapsed_time_ms < read_time){
        if (OPT1_current > OPT1_max){OPT1_max = OPT1_current;}
        if (OPT1_current < OPT1_min){OPT1_min = OPT1_current;}
        if (OPT2_current > OPT2_max){OPT2_max = OPT2_current;}
        if (OPT2_current < OPT2_min){OPT2_min = OPT2_current;}
      }
      
      if (elapsed_time_ms == read_time){
        digitalWrite(RED_LED,LOW);digitalWrite(GREEN_LED,HIGH); digitalWrite(BLUE_LED,LOW);
      }

      if (elapsed_time_ms > query_time){
        if (cursor_position == 1||cursor_position == 2||cursor_position == 3||cursor_position == 4||cursor_position == 5||cursor_position == 6||cursor_position == 7){
        if ((over >= 0) && (OPT1_current > (OPT1_max + over))){
          digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,HIGH);
        }

        if ((over >= 0) && (OPT2_current > (OPT2_max + over)))
        { digitalWrite(GREEN_LED,LOW); digitalWrite(RED_LED,HIGH);
        }

        if ((UNDER >= 0) && (OPT1_current < (OPT1_min - UNDER)))
        { digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,HIGH);
        }

        if ((UNDER >= 0) && (OPT2_current < (OPT2_min - UNDER)))
        { digitalWrite(GREEN_LED,LOW); digitalWrite(RED_LED,HIGH);
        }

    }}

     if (elapsed_time_ms == REPORT_TIME) {
        if (cursor_position == 1||cursor_position == 2||cursor_position == 3||cursor_position == 4||cursor_position == 5||cursor_position == 6||cursor_position == 7){
          calculate_times();
          calculate_averages();
          calculate_screen_time();
          calculate_winners();
          if ((save_mode == 0) && (trial_number < TRIAL_MAX)){save_summary();}
          for (int i = 0; i < 255; i++){
            OPT1_summary[i] = 0;
            OPT2_summary[i] = 0;}
        }
        if (cursor_position == 7){
          if ((OPT1_results[trial_number] != 0) &&  (OPT2_results[trial_number] != 0)){
          if ((OPT2_results[trial_number] - OPT1_results[trial_number]) > PLAY1P2P){digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,HIGH);digitalWrite(RED_LED,LOW);}
          if ((OPT1_results[trial_number] - OPT2_results[trial_number]) > PLAY1P2P){digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,LOW);digitalWrite(RED_LED,HIGH);}
        }}

        display_screen();

        if (Serial.available() > 0){ youvegotmail = Serial.read(); }
        if (SerialBT.available() > 0){ youvegotmail = SerialBT.read(); }

        if (char(youvegotmail) == '$'){
            digitalWrite(RED_LED,LOW);digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,LOW);
            pinMode(LEFT, INPUT_PULLUP);pinMode(RIGHT, INPUT_PULLUP);pinMode(CONFIRM, INPUT_PULLUP);
        mode = 0; youvegotmail = 0; youvegotemail = "";}

trial_number = trial_number + 1;
        
        }


      if (elapsed_time_ms >= QUIT_TIME) {
        digitalWrite(RED_LED,LOW);digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,LOW);

        if (only_one_test==1) {
          reset_OPT_values();
          pinMode(LEFT, INPUT_PULLUP);pinMode(RIGHT, INPUT_PULLUP);pinMode(CONFIRM, INPUT_PULLUP);mode = 0;
        }

        if (cursor_position == 1 ||cursor_position == 3||cursor_position == 9) {
          reset_OPT_values();
          pinMode(LEFT, INPUT_PULLUP);pinMode(RIGHT, INPUT_PULLUP);pinMode(CONFIRM, INPUT_PULLUP);mode = 0; 
        }
        if (cursor_position == 2||cursor_position == 4||cursor_position == 5||cursor_position == 6||cursor_position == 7
        ||cursor_position == 8||cursor_position == 10) {
          reset_OPT_values();
          time3 = 1 + micros() / 100;
        }}

        if (trial_number >= TRIAL_MAX) {
          if (cursor_position == 1 ||cursor_position == 2||cursor_position == 3||cursor_position == 4||cursor_position == 5||cursor_position == 6||cursor_position == 7){          
            create_sheet();          
            File writesummary = LittleFS.open("/summary.txt","a");
            writesummary.print("U");writesummary.print(",");
            for (int i = 0 ; i < TRIAL_MAX; i++){writesummary.print(OPT1_results[i]);writesummary.print(",");}
            writesummary.println();
            writesummary.print("V");writesummary.print(",");
            for (int i = 0 ; i < TRIAL_MAX; i++){writesummary.print(OPT2_results[i]);writesummary.print(",");}
            writesummary.println();
            writesummary.close();

            File writeresults = LittleFS.open("/results.txt", "a");
            writeresults.println(results);
            writeresults.close();
            Serial.println("Saved results");
          }
      
          if (cursor_position == 2||cursor_position == 4||cursor_position == 5||cursor_position == 6||cursor_position == 7||cursor_position == 8){
            pinMode(LEFT, INPUT_PULLUP);pinMode(RIGHT, INPUT_PULLUP);pinMode(CONFIRM, INPUT_PULLUP);mode = 0;}
    }
    }

    if (mode == 10) {
      current_button = 0;
      if (digitalRead(LEFT) == 0) { current_button = 4; }
      if (digitalRead(RIGHT) == 0) { current_button = 6; }
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }
      
      if ((current_button == 4) && (last_button == 0)) {
        trial_number = max(0, trial_number - 1);
      }
      if ((current_button == 6) && (last_button == 0)) {
        trial_number = min(TRIAL_MAX, trial_number + 1);
      }
      if ((current_button == 5) && (last_button == 0) && (trial_number < TRIAL_MAX)) {
        reset_OPT_values();
        cursor_position = 1;
        time3 = 1 + micros() / 100;
        mode = 1;
      }
      display_screen();
    }

if (mode == -1){
  for (int i=0;i<length_array;i++){
  SerialBT.print(serial_names[i]);SerialBT.println(*pointers[i]);
  Serial.print(serial_names[i]);Serial.println(*pointers[i]);} 
  for (int i=0;i<float_length_array;i++){SerialBT.print(float_serial_names[i]);SerialBT.println(*float_pointers[i]);
  Serial.print(float_serial_names[i]);Serial.println(*float_pointers[i]);}
  mode = 0;
}

if (mode == -2){
  Serial.println(results);
  SerialBT.println(results);
  mode = 0;
}

if (mode == -3){
    File readresults = LittleFS.open("/results.txt", "r");
    while (readresults.available()) {
      char temp_char = readresults.read();
    Serial.write(temp_char);SerialBT.write(temp_char);
    }
    Serial.println();
    readresults.close();
    mode = 0;        
}

if (mode == -4){
  current_button = 0;
  if (digitalRead(CONFIRM) == 0) { current_button = 5; }
  if ((current_button == 5) && (last_button ==0)){
  File writeresults = LittleFS.open("/results.txt", "w");
  writeresults.close();
  Serial.println("deleted");
  last_button = 5;
  cursor_position = initial_cursor_position;
  mode = 0;}
  }

  if (mode == -6){
    File readsummary = LittleFS.open("/summary.txt", "r");
    while (readsummary.available()) {
    char temp_char = readsummary.read();
    Serial.write(temp_char);}
    Serial.println();
    readsummary.close();
    mode = 0;
  }

  if (mode == -7){
    timea=micros();
    timeb=micros();
    test_time=timeb-timea;
    mode = 0;
  }



    if (mode == 0) {
      display_screen();

      current_button = 0;

      turn_off_buttons();

        if (Serial.available() > 0){
        youvegotmail=Serial.read();
        youvegotemail =  Serial.readStringUntil('\n');youvegotemail.trim();}

        if (SerialBT.available() > 0){
        youvegotmail = SerialBT.read();
        youvegotemail =  SerialBT.readStringUntil('\n');youvegotemail.trim();}




WiFiClient client = server.accept();
if (client) {
Serial.println("New browser request.");

while (client.connected()) {
if (client.available()) {
char c = client.read();
  if (c == '\n') {

    if(whole_url.length() == 0){

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();

      client.println("<!DOCTYPE html><html>");
      client.println("<style>button { width: 250px; height: 80px; font-size: 22px; margin: 10px; }</style>");

      client.println("<a href=\"/set?Y=1\"><button>Start OPT1</button></a>");
      client.println("<a href=\"/set?Y=2\"><button>Start OPT2</button></a>");
      client.println("<a href=\"/\"><button>Refresh</button></a>");
      client.println("<p></p>");

      client.println("<h1>" + String(trial_number) + " / "+String(TRIAL_MAX) +  "trials</h1>");

      client.println("<h1>OPT1 results = "+ String(OPT1_results[trial_number])+"ms (AVG = "+String(OPT1_avg)+"ms)</h1>");
      client.println("<h1>OPT2 results = "+ String(OPT2_results[trial_number])+"ms (AVG = "+String(OPT2_avg)+"ms)</h1>");

      client.println("<h1>OPT1 reading: " + String(OPT1_current) + " OPT2 reading: "+String(OPT2_current) +  "</h1>");

      client.println("<p></p>");
      client.println("<a href=\"/set?*\"><button>UP</button></a>");
      client.println("<a href=\"/set?%\"><button>OK</button></a>");

      client.println("<p></p>");
      client.println("<a href=\"/set?&\"><button>RIGHT</button></a>");
      client.println("<a href=\"/set?$\"><button>LEFT</button></a>");

      client.println("<p></p>");

      client.println("  <canvas id=\"canvas_last\" width=\""+String(REPORT_TIME)+"\" height=\"255\" style=\"border:1px solid black;\"></canvas>");
      client.println("  <script>");
      client.println("    var ctx_last = document.getElementById(\"canvas_last\").getContext(\"2d\");");
      client.println("    ctx_last.fillStyle = \"blue\";");
      for (int variable = 0;variable < REPORT_TIME;variable ++){
      client.println("    ctx_last.fillRect("+String(variable)+","+String(255-OPT1_decimal[variable*10])+", 1, 1);");}
      client.println("    ctx_last.fillStyle = \"red\";");
      for (int variable = 0;variable < REPORT_TIME;variable ++){
      client.println("    ctx_last.fillRect("+String(variable)+","+String(255-OPT2_decimal[variable*10])+", 1, 1);");}
      client.println("  </script>");

      client.println("<p></p>");

      client.println("  <canvas id=\"canvas_avg\" width=\""+String(TRIAL_MAX)+"\" height=\""+String(REPORT_TIME-read_time)+"\" style=\"border:1px solid black;\"></canvas>");
      client.println("  <script>");
      client.println("    var ctx_avg = document.getElementById(\"canvas_avg\").getContext(\"2d\");");
      client.println("    ctx_avg.fillStyle = \"blue\";");
      for (int variable = 0;variable < TRIAL_MAX;variable ++){
      client.println("    ctx_avg.fillRect("+String(variable)+","+String(REPORT_TIME-read_time-int(OPT1_results[variable]))+", 1, 1);");}
      client.println("    ctx_avg.fillStyle = \"red\";");
      for (int variable = 0;variable < TRIAL_MAX;variable ++){
      client.println("    ctx_avg.fillRect("+String(variable)+","+String(REPORT_TIME-read_time-int(OPT2_results[variable]))+", 1, 1);");}
      client.println("  </script>");
      client.println("<p></p>");

      client.println("<details>");
      client.println("  <summary>Click for results</summary>");            
      client.println("<p></p>");

      client.println("<a href=\"/detailed\"><button>SHOW GRAPHS</button></a>");
      client.println("<p></p>");

      for (int n = 0;n<TRIAL_MAX;n++){  
      client.println("<p>"+String(n) + " = " + String(OPT1_results[n])+"ms, "+String(OPT2_results[n])+"ms</p>");

      if (detailed_graphics == 1){
      client.println("  <canvas id=\"canvas"+String(n)+"\" width=\"255\" height=\"255\" style=\"border:1px solid black;\"></canvas>");
      }}
      client.println("</details>");
      client.println("<p></p>");
      if (detailed_graphics == 1){
      File rawfile = LittleFS.open("/raw.txt", "r");
      if (rawfile) {
      while (rawfile.available()) {
      String raw_line = rawfile.readStringUntil('\n');
      raw_line.trim();

      if (raw_line[0] == 'u'){ 
      raw_line=raw_line.substring(1);
      int raw_deletion_point = raw_line.indexOf('=');

      int raw_test = raw_line.substring(0,raw_deletion_point).toInt();
      raw_line = raw_line.substring(raw_deletion_point+1);  

      client.println("  <script>");
      client.println("    var ctx"+String(raw_test)+" = document.getElementById(\"canvas"+String(raw_test)+"\").getContext(\"2d\");");

      client.println("    ctx"+String(raw_test)+".fillStyle = \"black\";");
      client.println("    ctx"+String(raw_test)+".fillRect("+String(read_time)+",0, 1, 255);");

      client.println("    ctx"+String(raw_test)+".fillStyle = \"blue\";");


      for (int raw_x_coord = 0;raw_x_coord < 255; raw_x_coord ++){            
      int raw_deletion_point = raw_line.indexOf(',');
      int raw_y_coord = raw_line.substring(0,raw_deletion_point).toInt();
      raw_line = raw_line.substring(raw_deletion_point+1);
      
      client.println("    ctx"+String(raw_test)+".fillRect("+String(raw_x_coord)+","+String(255-raw_y_coord)+", 1, 1);");}
      client.println("  </script>");   
      }

      if (raw_line[0] == 'v'){ 
      raw_line=raw_line.substring(1);
      int raw_deletion_point = raw_line.indexOf('=');

      int raw_test = raw_line.substring(0,raw_deletion_point).toInt();
      raw_line = raw_line.substring(raw_deletion_point+1);

      client.println("  <script>");
      client.println("    var ctx"+String(raw_test)+" = document.getElementById(\"canvas"+String(raw_test)+"\").getContext(\"2d\");");
      client.println("    ctx"+String(raw_test)+".fillStyle = \"red\";");

      for (int raw_x_coord = 0;raw_x_coord < 255; raw_x_coord ++){            
      int raw_deletion_point = raw_line.indexOf(',');
      int raw_y_coord = raw_line.substring(0,raw_deletion_point).toInt();
      raw_line = raw_line.substring(raw_deletion_point+1);
      
      client.println("    ctx"+String(raw_test)+".fillRect("+String(raw_x_coord)+","+String(255-raw_y_coord)+", 1, 1);");}
      client.println("  </script>");   
      }

      }

      rawfile.close();}

      detailed_graphics = 0;}

      client.println("<details>");
      client.println("  <summary>Click for controller settings</summary>");
      client.println("<p></p>");
      for (int n = 0;n<28;n++){  
      client.println("<form action='/set' method='GET'>");
      client.println("  <input type='number'  name="+String(serial_names[n])+" placeholder="+String(*pointers[n]) +">");
      client.println("  <input type='submit' value="+String(serial_titles[n])+">");
      client.println("</form>");}
      client.println("</details>");
      client.println("<p></p>");

      client.println("<details>");
      client.println("  <summary>Click for test settings</summary>");
      client.println("<p></p>");
      for (int n = 28;n<length_array;n++){  
      client.println("<form action='/set' method='GET'>");
      client.println("  <input type='number'  name="+String(serial_names[n])+" placeholder="+String(*pointers[n]) +">");
      client.println("  <input type='submit' value="+String(serial_titles[n])+">");
      client.println("</form>");}
      client.println("</details>");
      client.println("<p></p>");


      client.println("<details>");
      client.println("  <summary>Click for screen settings</summary>");
      client.println("<p></p>");
      for (int n = 0;n<float_length_array;n++){  
      client.println("<form action='/set' method='GET'>");
      client.println("  <input type='text'  name="+String(float_serial_names[n])+" placeholder="+String(*float_pointers[n]) +">");
      client.println("  <input type='submit' value="+String(float_serial_titles[n])+">");
      client.println("</form>");}
      client.println("</details>");

      client.println("<p></p>");

      if (wifimode == 0){
      client.println("<h3>If you want to connect to network mode next time, input WiFi details here</h3>");


      client.println("<form action=\"/\" method=\"GET\">");
      client.println("Value: <input type=\"text\" name=\"val\"><br><br>");
      client.println("Value2: <input type=\"text\" name=\"val2\"><br><br>");
      client.println("<input type=\"submit\" value=\"Submit\">");
      client.println("</form>");}

      client.println("<p></p>");
      client.println("<details>");
      client.println("  <summary>Data for export to spreadsheet</summary>");

      client.println("<h4>"+String(results)+"</h4>");

      client.println("</details>");

      client.println("<p></p>");

      client.print("<head>");
      client.print("  <meta name='viewport' content='width=device-width, initial-scale=1'>");
      client.print("  <title id='page-title'>Input Lag Testing</title>");
      client.print("</head>");
      client.print("<body style='font-family: Arial; text-align: left; margin-top: 50px;'>");

      client.print("  <input type='text' id='titleInput' placeholder='Type new title...'");
      client.print("<style='width: 200px; height: 20px; font-size: 22px; padding: 10px;'>");
      client.print("  <button onclick='updateTitle()'>Update Title</button>");

      client.print("  <script>");
      client.print("    function updateTitle() {");
      client.print("      var newTitle = document.getElementById('titleInput').value;");
      client.print("      if(newTitle.trim() !== '') {");
      client.print("        document.title = newTitle;");
      client.print("        document.getElementById('display-heading').innerText = newTitle;");
      client.print("      }");
      client.print("    }");
      client.print("  </script>");
      client.println("<p></p>");
      client.println("</body></html>");

      break;


    } else {
    if (whole_url.indexOf("favicon") < 0) {

      if(whole_url.indexOf("/detailed") >=0){detailed_graphics=1;}

      if(whole_url.indexOf("/?val") >=0){
      startPos = whole_url.indexOf("val=")+4;
      endPos = whole_url.indexOf('&',startPos);
      String ssid_new = whole_url.substring(startPos,endPos);
      Serial.println("ssid");
      Serial.println(ssid_new);

      startPos = whole_url.indexOf("val2=")+5;
      endPos = whole_url.indexOf(" ",startPos);
      String password_new = whole_url.substring(startPos,endPos);
      Serial.println("password");Serial.println(password_new);

      File password_save = LittleFS.open("/password.txt", "w");
      password_save.println(ssid_new);
      password_save.println(password_new);
      password_save.close();
      }  
      if (whole_url.indexOf("GET /set?") >= 0) {
        
        int qPos = whole_url.indexOf('?');
        if (qPos != -1) {
          
          youvegotmail = whole_url[qPos + 1];

          
          int eqPos = whole_url.indexOf('=', qPos);
          if (eqPos != -1) {
            
            int endVal = whole_url.indexOf(' ', eqPos);
            if (endVal == -1) endVal = whole_url.indexOf('&', eqPos);
            if (endVal == -1) endVal = whole_url.length();

            youvegotemail = whole_url.substring(eqPos + 1, endVal);
            youvegotemail.trim();
          }
        }

        Serial.print("Web command: ");
        Serial.print((char)youvegotmail);
        Serial.print("  value: ");
        Serial.println(youvegotemail);

        client.println("HTTP/1.1 303 See Other");
        client.println("Location: /");
        client.println("Connection: close");
        client.println();
      }
    }
    whole_url = "";
      }}
      else if (c !='\r'){whole_url += c;}
  }
  }

delay(10);
client.stop();
Serial.println("Client disconnected.");
}


      inbox();


      if (digitalRead(CANCEL) == 0) { current_button = 1; }
      if (digitalRead(LEFT) == 0) { current_button = 4; }
      if (digitalRead(RIGHT) == 0) { current_button = 6; }
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }

      if((current_button == 4) && (last_button == 1)){current_button = 11;save_mode = !save_mode;delay(300);}
      if((current_button == 6) && (last_button == 1)){current_button = 11;only_one_test = !only_one_test;delay(300);}
      if((current_button == 5) && (last_button == 1)){current_button = 11; fastest_slowest_mode = !fastest_slowest_mode;delay(300);}
      
      if ((digitalRead(CONFIRM) == 0) && (digitalRead(RIGHT) == 0)) { current_button = 10; }
      

      if ((current_button == 6) && (last_button == 4)) {initial_cursor_position = cursor_position; save_settings();}

      if ((current_button == 10) && (last_button != 10)) {
        trial_number = max(0, trial_number - 1);
        cursor_position = 1;
      }

      if ((current_button == 1) && (last_button == 0) && (mode == 0)) { cursor_position = 0; }

      if ((current_button == 4) && (last_button == 0)) { cursor_position = max(-13, cursor_position - 1); }
      if ((current_button == 6) && (last_button == 0)) { cursor_position = min(52, cursor_position + 1); }

      if ((current_button == 5) && (last_button == 0) && (cursor_position == 0)) {
        reset_all_values();
        last_button = 5;
        cursor_position = initial_cursor_position;
        mode = 0;
      }

      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 5)){calculate_mode = 0;}
      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 6)){calculate_mode = 1;}
      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 9)){trial_number = TRIAL_MAX + 1;}

      if ((current_button == 5) && (last_button == 0) && (cursor_position == 10)){connect_sub_board();}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -11)) { mode = -11; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -12)) { mode = -12; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -13)) { mode = -13; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -2)) { mode = -2; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -3)) { mode = -3; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -4)) {mode = -4;last_button = 5;}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -6)) { mode = -6; }
      if ((current_button == 5) && (last_button == 0) && (trial_number < TRIAL_MAX) && 
      (cursor_position == 1 || cursor_position == 2 ||cursor_position == 3 ||cursor_position == 4 ||cursor_position == 5 ||cursor_position == 6||cursor_position == 7
      ||cursor_position == 8||cursor_position == 9||cursor_position == 10 ))
      { reset_OPT_values();
        pinMode(RED_LED,OUTPUT);pinMode(GREEN_LED,OUTPUT);pinMode(BLUE_LED,OUTPUT);
        digitalWrite(RED_LED,LOW);digitalWrite(GREEN_LED,LOW);digitalWrite(BLUE_LED,LOW);
        time3 = 1 + micros() / 100;
        if ((cursor_position >=1) && (cursor_position <=8) && (trial_number < TRIAL_MAX)){prepare_summary();}
        mode = 1;
      }

    }

    last_button = current_button;
    time2 = micros() / 100;
  }
}
