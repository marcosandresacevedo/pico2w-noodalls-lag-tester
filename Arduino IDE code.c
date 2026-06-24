//https://arduino-pico.readthedocs.io/en/latest/install.html#installing-via-arduino-boards-manager
// make sure you follow here to install the PICO2W board

//in Tools I have IP/Bluetooth stack set to IPv4 + Bluetooth
//in Tools I have Flash size set to 4MB(Sketch 2MB FS 2MB )

//Will post board links once I've tested them

//playlist showing soldering and setup is here
//https://www.youtube.com/playlist?list=PLevqZFjlWIdexYMa2yQZgw0dfLVjzN8m7


#include "LittleFS.h"
#include <SerialBT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include <WiFi.h>          
int wifimode = 0; //0 is AP mode, 1 is Networked mode. 


String ssid = "fubarduck";
String password = "noodalls";


String whole_url = "";
int detailed_graphics = 0;

WiFiServer server(80);
float website_refresh = 30;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
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
int initial_cursor_position = 1;

int youvegotmail = 0;
String youvegotemail = "0.0";

int int_val = 0;
float float_val = 0.0;



char abc=' ';
uint8_t PTmain_decimal[10000];
uint8_t PTsub_decimal[10000];

int PTmain_summary[255];
int PTsub_summary[255];

int PTmain_current = 0;
int PTsub_current = 0;
float PTmain_results[1000];
float PTsub_results[1000];

float PTmain_fastest = 0.0;
float PTsub_fastest = 0.0;
float PTmain_slowest = 999.9;
float PTsub_slowest = 999.9;

int save_mode = 0; //0 = save results, 1 = don't save results
int calculate_mode = 0;//0 = Phototransistor known, 1 = phototransistor calc, 2 = phototransistor compare
int fastest_slowest_mode = 0; //1 = only use fastest and slowest results when calculating averages
int only_one_test = 0; //1 = test once at a time

float upper = 27;
float lower = 79;
float top_of_screen = 0;
float bottom_of_screen = 336;

float current_temp = 0.0;
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long test_time = 0;

unsigned long elapsed_time = 0;
unsigned long elapsed_time_ms = 0;

unsigned long timea = 0;
unsigned long timeb = 0;
unsigned long timec = 0;

unsigned long PTmain_sum = 0;
unsigned long PTsub_sum = 0;
int PTmain_count = 0;
int PTsub_count = 0;

float PTmain_avg = 0.0;
float PTsub_avg = 0.0;

float PTmain_top_a = 0.0;
float PTsub_top_a = 0.0;
float PTmain_bottom_a = 0.0;
float PTsub_bottom_a = 0.0;
float PTmain_top_b = 0.0;
float PTmain_bottom_b = 0.0;
float screen_draw_time_known = 8.33;
float screen_hz_known = 120;
float screen_draw_time_calculated = 8.33;
float screen_hz_calculated = 120;

int under = 2;
int OVER = 2;
int player = 2;
int PLAY1P2P = 4;

int trial_number = 0;
int TRIAL_MAX = 100;

int read_time = 100;
int REPORT_TIME = 250;
int query_time = 100;
int QUIT_TIME = 499;

int PTmain = 28;int PTsub = 26;
//int PTmain = 28; int PTsub = 26; //4/2026

float PTmain_time = 0;
float PTsub_time = 0;
int PTmain_min = 255;
int PTmain_max = 0;
int PTsub_min = 255;
int PTsub_max = 0;

int P1_wins = 0;
int P2_wins = 0;
int P1_P2_draw = 0;

//future - pin 6 is red and left, pin 7 is green and right, pin 8 is blue and confirm, pin 9 is cancel // need to go back and confirm

int startPos = 0;
int endPos = 0;
// int LEFT = 6;//int RIGHT = 7;//int OK = 8;//int CANCEL = 9;  //4/2026

int LEFT = 18;int RIGHT = 19;int CONFIRM = 16;int CANCEL = 17;

int BLUE_LED = 18;int GREEN_LED = 16;int RED_LED = 19;
//int BLUE_LED = 6; int GREEN_LED = 7; int RED_LED = 8; // 4/2026


//int PPS = 22;

int external_trigger = 0;

int current_button = 0;
int last_button = 0;

String results = "";



//reset
//A-5 B-5 C-5 D-5 E-5 F-5 G-5 H-5 I-5 J-5 K-5 L-5 M100 N101
//a0 b0 c0 d0 e0 f0 g0 h0 i0 j0 k0 l0 m50 n50

int OUTPUT_ON_TIME[] = { -5, -5, -5, -5,  100, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5};
int output_off_time[] = { 0, 0, 0, 0, 133, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int OUTPUT_ON_STATE[]    = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0};
int output_off_state[]   = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1};
int output_reset_state[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1};

//int OUTPUT_BUTTONS[] = { 15,14,13,12,11,10,9,8,7,6,3,2,20,21};
int OUTPUT_BUTTONS[]={15, 14, 13, 12, 11, 10, 9,8,7,6,3,2,21,20,22};
// DULR 1p2p3p4p 1k2k3k4k se ho st
//int OUTPUT_BUTTONS[] = {21,10,20,11, 19,12,18,13, 14,17,15,16, 22,22,22}         //4.2026


String serial_titles[] = { "DOWN_ON", "UP_ON", "LEFT_ON", "RIGHT_ON", "1P_ON", "2P_ON", "3P_ON", "4P_ON", "1K_ON", "2K_ON", "3K_ON", "4K_ON", "SE_ON", "HO_ON","ST_ON",
                           "down_off", "up_off", "left_off", "right_off", "1p_off", "2p_off", "3p_off", "4p_off", "1k_off", "2k_off", "3k_off", "4k_off", "se_off", "ho_off","st_off",
                           "trial_number", "TRIAL_MAX", "read_time", "REPORT_TIME", "query_time", "QUIT_TIME", 
                           "under", "OVER", "players", "1P2PCOMP", "Initial cursor", "fastest_slowest_mode","only_one_test","calculate_mode","save_data" };

String float_serial_titles[] = {
  "upper_value",  "lower_value",  "top_of_screen",  "bottom_of_screen",  "screen_hz",  "screen_time_ms"};

char float_serial_names[] = {
  'w',  'W',  's',  'S',  'x',  'X'};

char serial_names[] = {
  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N', 'O',
  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n', 'o',
  't',  'T',  'r',  'R',  'q',  'Q',  
  'u',  'U',  'p',  'P',  'z',  '!',  '@',  '_',  '`'
};



int min_list[] = { -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -1, 10, 1, 1, 1, 1, 
                   0, 0, 0, 0, -20, 0, 0, 0,0};

int max_list[] = { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 1000, 999, 1999, 999, 1999, 
                   50, 50, 2, 50, 60, 1, 1, 2, 1};

//  &trial_number,  &TRIAL_MAX,  &read_time,  &REPORT_TIME,  &query_time,  &QUIT_TIME,  
//  &under,  &OVER,  &player,  &PLAY1P2P,  &initial_cursor_position,  &fastest_slowest_mode,  &only_one_test, &calculate_mode, &save_mode
int *pointers[] = {
  &OUTPUT_ON_TIME[0],
  &OUTPUT_ON_TIME[1],
  &OUTPUT_ON_TIME[2],
  &OUTPUT_ON_TIME[3],
  &OUTPUT_ON_TIME[4],
  &OUTPUT_ON_TIME[5],
  &OUTPUT_ON_TIME[6],
  &OUTPUT_ON_TIME[7],
  &OUTPUT_ON_TIME[8],
  &OUTPUT_ON_TIME[9],
  &OUTPUT_ON_TIME[10],
  &OUTPUT_ON_TIME[11],
  &OUTPUT_ON_TIME[12],
  &OUTPUT_ON_TIME[13],
  &OUTPUT_ON_TIME[14],

  &output_off_time[0],
  &output_off_time[1],
  &output_off_time[2],
  &output_off_time[3],
  &output_off_time[4],
  &output_off_time[5],
  &output_off_time[6],
  &output_off_time[7],
  &output_off_time[8],
  &output_off_time[9],
  &output_off_time[10],
  &output_off_time[11],
  &output_off_time[12],
  &output_off_time[13],
  &output_off_time[14],

  &trial_number,  &TRIAL_MAX,  &read_time,  &REPORT_TIME,  &query_time,  &QUIT_TIME,  &under,  &OVER,  &player,  &PLAY1P2P,  &initial_cursor_position,  &fastest_slowest_mode,  &only_one_test, &calculate_mode, &save_mode
};

float *float_pointers[] = {
  &upper,
  &lower,
  &top_of_screen,
  &bottom_of_screen,
  &screen_hz_known,
  &screen_draw_time_known,
};

int length_array = sizeof(serial_names) / sizeof(serial_names[0]);
int float_length_array = sizeof(float_serial_names) / sizeof(float_serial_names[0]);

void prepare_summary() {
  for (int variable = 0; variable < 255; variable++) {
    PTmain_summary[variable] = 0;//***
    PTsub_summary[variable] = 0;//***
  }
  File writesummary = LittleFS.open("/summary.txt", "w");
  for (int variable = 0; variable < length_array; variable++) {
    writesummary.print(serial_names[variable]);
    writesummary.println(*pointers[variable]);
  }
  for (int variable = 0; variable < float_length_array; variable++) {
    writesummary.print(float_serial_names[variable]);
    writesummary.println(*float_pointers[variable]);
  }

  writesummary.close();


  
  
}

void connect_sub_board(){
            OUTPUT_ON_STATE[0] = 1; OUTPUT_ON_STATE[1] = 1; OUTPUT_ON_STATE[2] = 1;OUTPUT_ON_STATE[3] = 1;
          output_off_state[0]=0; output_off_state[1]=0; output_off_state[2]=0;output_off_state[3]=0;
          output_reset_state[0]=0; output_reset_state[1]=0; output_reset_state[2]=0;output_reset_state[2]=0;
}

void save_summary() {
  for (int variable=0;variable < 255; variable ++){
    PTmain_summary[variable] = PTmain_decimal[variable * 10];
    PTsub_summary[variable] = PTsub_decimal[variable * 10];

  }

  File writeraw = LittleFS.open("/raw.txt", "a");
  writeraw.print("u");  
  writeraw.print(trial_number);
  writeraw.print("=");
  for (int variable = 0; variable < 255; variable++) {
    writeraw.print(PTmain_summary[variable]);
    writeraw.print(",");
  }
  writeraw.println();
  writeraw.print("v");  
  writeraw.print(trial_number);
  writeraw.print("=");
  for (int variable = 0; variable < 255; variable++) {
    writeraw.print(PTsub_summary[variable]);
    writeraw.print(",");
  }
  writeraw.println();


  writeraw.close();

}

void calculate_screen_time() {//consider if upper == lower then set calculate_mode to 2

  PTmain_top_a = 0.0;
  PTsub_top_a = 0.0;
  PTmain_bottom_a = 0.0;
  PTsub_bottom_a = 0.0;
  PTmain_top_b = 0.0;
  PTmain_bottom_b = 0.0;

  //known hz
  PTmain_top_a = PTmain_avg - screen_draw_time_known * upper / 100;
  PTsub_top_a = PTsub_avg - screen_draw_time_known * lower / 100;
  PTmain_bottom_a = PTmain_top_a + screen_draw_time_known;
  PTsub_bottom_a = PTsub_top_a + screen_draw_time_known;

  if ((PTmain_avg != PTsub_avg) && (PTmain_avg != 0) && (PTsub_avg != 0)) {

    screen_draw_time_calculated = 100 * (PTsub_avg - PTmain_avg) / (lower - upper);

    screen_hz_calculated = 1000 / screen_draw_time_calculated;

    PTmain_top_b = PTmain_avg - screen_draw_time_calculated * upper / 100;
    PTmain_bottom_b = PTmain_top_b + screen_draw_time_calculated;
  }
}

void calculate_winners() {
  P1_wins = 0;
  P2_wins = 0;
  P1_P2_draw = 0;


  for (int variable = 0; variable < 1000; variable++) {
    if ((PTmain_results[variable] != 0) && (PTsub_results[variable] != 0)) {
      if ((PTsub_results[variable] - PTmain_results[variable]) > PLAY1P2P) {
        P1_wins++;
      } else if ((PTmain_results[variable] - PTsub_results[variable]) > PLAY1P2P) {
        P2_wins++;
      } else {
        P1_P2_draw++;
      }

      //digitalWrite(GREEN_LED,LOW); digitalWrite(BLUE_LED,LOW);digitalWrite(RED_LED,HIGH);
      //digitalWrite(GREEN_LED,LOW); digitalWrite(RED_LED,LOW);digitalWrite(BLUE_LED,HIGH);
    }
  }
}


void display_screen() {
  //calculate_screen_time();//not here? add to serial!!
  //calculate_winners();//not here?

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
  if (cursor_position == -1) { display.println("Review"); }
  //if (cursor_position == -11) { display.println("Controller variables"); }
  if (cursor_position == -12) { display.println("Integer variables"); }
  if (cursor_position == -13) { display.println("Float variables"); }
  

  if (cursor_position == 0) { display.println("RESET"); }

  if (cursor_position == 1) { display.print("PTmain"); }
  if (cursor_position == 2) { display.print("PTsub"); } 
  if (cursor_position == 3) { display.println("Screen time known"); }
  if (cursor_position == 4) { display.println("Screen time calc"); }
   
  if (cursor_position == 6) { display.println("PTmain+2 Graph"); }
  if (cursor_position == 7) { display.println("PTmain+2 Summary"); }  
  if (cursor_position == 8) { display.println("Compare 1P2P"); }

  if (cursor_position == 9) { display.println("Test_max"); }
  if (cursor_position == 10) { display.println("Non PT tests"); }
  


if (cursor_position == 0){
    display.setCursor(0, 8);display.print(OLED_message1);
    display.setCursor(0,16);display.print(OLED_message2);
    display.setCursor(0,24);display.print(OLED_message3);
    display.setCursor(0,32);display.print(OLED_message4);
    display.setCursor(0,40);display.print(OLED_message5);
    display.setCursor(0,48);display.print(OLED_message6);
    display.setCursor(0,56);display.print(OLED_message7);

//if (wifimode == 0){OLED_message3 = WiFi.softAPIP().toString();};

}

  if (fastest_slowest_mode == 1){display.setCursor(120,0);display.print("!");}
    if (only_one_test == 1){display.setCursor(116,0);display.print("@");}
    if (save_mode == 1){display.setCursor(112,0);display.print("`");}

  if (cursor_position == -1) {
    display.setCursor(0, 56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);    
    display.setCursor(90, 16);display.print(PTmain_fastest);
    display.setCursor(90, 24);display.print(PTmain_slowest);
    display.setCursor(90, 32);display.print(PTsub_fastest);
    display.setCursor(90, 40);display.print(PTsub_slowest);
    if (mode == -1){
    display.setTextSize(2);
    display.setCursor(0, 16);display.print(PTmain_results[trial_number]);
    display.setCursor(0, 32);display.print(PTsub_results[trial_number]);
    display.setTextSize(1);}
    if (mode != -1){
    display.setTextSize(2);
    display.setCursor(0, 16);display.print(PTmain_avg);
    display.setCursor(0, 32);display.print(PTsub_avg);
    display.setTextSize(1);
    }
  }


  if (cursor_position == 1) {

    display.setCursor(0, 8);
    display.print(PTmain_avg);
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);

    display.setCursor(90, 8);
    display.print(PTmain_current);

    display.setTextSize(3);
    if (PTmain_results[trial_number] < 100) { display.setTextSize(4); }

    display.setCursor(0, 16);
    display.print(PTmain_results[trial_number]);
    display.setTextSize(1);
  }

  if (cursor_position == 2) {

        display.setCursor(110, 8);
    display.print(PTsub_current);

    display.setCursor(0, 8);
    display.print(PTsub_avg);

    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);

    display.setTextSize(3);
    if (PTsub_results[trial_number] < 100) {
      display.setTextSize(4);
    }

    display.setCursor(0, 16);
    display.print(PTsub_results[trial_number]);
    display.setTextSize(1);
  }


  if (cursor_position == 3||cursor_position == 4||cursor_position == 8) {
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);
    
    display.setCursor(64, 56);
    display.print("(");
    display.print(PTmain_count);
    display.print("/");
    display.print(PTsub_count);
    display.print(")");


    display.setCursor(0, 16);
    display.print(PTmain_results[trial_number]);
    display.print("/");
    display.print(PTmain_avg);
    display.setCursor(96, 16);
    display.print("P1/av");
    display.setCursor(0, 24);
    display.print(PTsub_results[trial_number]);
    display.print("/");
    display.print(PTsub_avg);
    display.setCursor(96, 24);
    display.print("P2/av");

    //display.setCursor(80, 56);display.print(PTmain_current);
    //display.setCursor(104, 56);display.print(PTsub_current);
    display.setCursor(90, 8);
    display.print(PTmain_current);
    display.setCursor(110, 8);
    display.print(PTsub_current);
  }


  if (cursor_position == 3||cursor_position == 4) {
    display.setCursor(96, 32);
    display.print("P1/\\/");
    display.setCursor(96, 40);
    display.print("P2/\\/");
    display.setCursor(96, 48);
    display.print("hz/ms");

    display.setCursor(0, 8);
    display.print(upper, 0);
    display.setCursor(32, 8);
    display.print(lower, 0);
  }

  if (cursor_position == 3) {
    display.setCursor(0, 32);
    display.print(PTmain_top_a);
    display.print("/");
    display.print(PTmain_bottom_a);
    display.setCursor(0, 40);
    display.print(PTsub_top_a);
    display.print("/");
    display.print(PTsub_bottom_a);
    display.setCursor(0, 48);
    display.print(screen_hz_known);
    display.print("/");
    display.print(screen_draw_time_known);
  }


  if  (cursor_position == 4) {
    display.setCursor(0, 32);
    display.print(PTmain_top_b);
    display.print("/");
    display.print(PTmain_bottom_b);
    display.setCursor(0, 40);
    display.print(PTmain_top_b);
    display.print("/");
    display.print(PTmain_bottom_b);
    display.setCursor(0, 48);
    display.print(screen_hz_calculated);
    display.print("/");
    display.print(screen_draw_time_calculated);
  }

  if  (cursor_position == 8) {

    display.setCursor(0, 32);
    display.print(PTmain_top_a - PTsub_top_a);  //should this be average?

    display.setCursor(0, 40);
    display.print(P1_wins);
    display.setCursor(32, 40);
    display.print(P1_P2_draw);
    display.setCursor(64, 40);
    display.print(P2_wins);
    display.setCursor(96, 40);
    display.print("<=>");
    display.setCursor(120, 40);
    display.print(PLAY1P2P);
  }




  if ((cursor_position >= 6) && (cursor_position <= 10)) {
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);
  }

  if (cursor_position ==6||cursor_position == 7){
    display.setCursor(0, 8);display.print(PTmain_results[trial_number]);display.print("/");display.print(PTmain_avg);
    display.setCursor(0, 16);display.print(PTsub_results[trial_number]);display.print("/");display.print(PTsub_avg);}


  if  (cursor_position == 6){
  for (int variable = 0; variable < 128; variable++){
    display.drawPixel(variable,64-int(PTmain_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
  for (int variable = 0; variable < 128; variable++){
    display.drawPixel(variable,64-int(PTsub_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
 
  }
  if (cursor_position ==7){
for (int n = 0;n<32;n++){
  if ((trial_number - n) >= 0){if(PTmain_results[trial_number-n] < 128){display.setCursor(int(PTmain_results[trial_number-n]),56-n);display.print(".");}}
  if ((trial_number - n) >= 0){if(PTsub_results[trial_number-n] < 128){display.setCursor(int(PTsub_results[trial_number-n]),56-n);display.print(".");}}

}}

    //display.drawPixel(20,20,SSD1306_WHITE);
    // display.setCursor(100, min(56,64-int(PTmain_decimal[query_time*10 + 1000]/4)));
    // display.println("PTmain");
    // if (cursor_position == 12||cursor_position == 13||cursor_position == 14){
    // display.setCursor(0, 8);display.print(PTsub_results[trial_number]);display.print("/");display.print(PTsub_avg);
    //       display.setCursor(100, min(56,64-int(PTsub_decimal[query_time*10 + 1000]/4)));
    // display.println("PTsub");
    // for (int variable = 0; variable < 128; variable++){
    //   display.drawPixel(variable,64-int(PTsub_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
    // }


  
  
  
  if (cursor_position == -11) {
    //display.clearDisplay();
    display.setCursor(0, 0);
    for (int variable = 0; variable < 4; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 8);
    for (int variable = 4; variable < 8; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 16);
    for (int variable = 8; variable < 12; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }

    display.setCursor(0, 24);
    for (int variable = 12; variable < 15; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }

    display.setCursor(0, 32);
    for (int variable = 15; variable < 19; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 40);
    for (int variable = 19; variable < 23; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 48);
    for (int variable = 23; variable < 27; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 56);
    for (int variable = 27; variable < 30; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }

  }

  if (cursor_position == -12) {
    //display.clearDisplay();
    display.setCursor(0, 16);
    for (int variable = 28; variable < length_array; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
  }

  if (cursor_position == -13) {
    //display.clearDisplay();
    display.setCursor(0, 16);

    for (int variable = 0; variable < float_length_array; variable++) {
      display.print(float_serial_names[variable]);
      display.print(*float_pointers[variable]);
      display.print(" ");
    }
  }



  if (cursor_position == -2 || cursor_position == -3 || cursor_position == -4) {
    display.setCursor(0, 16);
    display.print(results);
  }

  if (cursor_position == -10) {
    display.clearDisplay();
    current_temp = analogReadTemp();
    display.setTextSize(4);
    display.setCursor(0, 16);
    display.print(current_temp);
    delay(500);
    display.setTextSize(1);
  }

  if (cursor_position == -7) {
    display.setTextSize(3);
    display.setCursor(0, 24);
    display.print(test_time);
    display.setTextSize(1);
  }


  // if ((cursor_position >=10) && (cursor_position <50)) {
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.print(serial_names[cursor_position - 10]);
  //   display.setCursor(0, 8);
  //   display.print(serial_titles[cursor_position - 10]);
  //   display.setCursor(0, 16);
  //   display.print(*pointers[cursor_position - 10]);
  // }

  // if ((cursor_position >=50) && (cursor_position <52)) {
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.print(float_serial_names[cursor_position - 50]);
  //   display.setCursor(0, 8);
  //   display.print(float_serial_titles[cursor_position - 50]);
  //   display.setCursor(0, 16);
  //   display.print(*float_pointers[cursor_position - 50]);
  // }
  if (cursor_position == -14){

    // display.setTextSize(0);
    // for (int variable = 0;variable < 12;variable +=2){      
    //   display.setCursor(0,16+variable*4);display.print(serial_names[variable]);
    // }
    //     for (int variable = 1;variable < 12;variable +=2){      
    //   display.setCursor(8,12+variable*4);display.print(serial_names[variable]);
    // }
      //display.fillRect(0, 8, 100, 3,1);
  }

      // for (int variable = 0; variable < 15; variable++) {
      //   if (elapsed_time_ms == OUTPUT_ON_TIME[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }    //buttons on
      //   if (elapsed_time_ms == output_off_time[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], output_off_state[variable]); }  //buttons_off
      //   //for Brook pins, HIGH is unpressed, LOW is pressed
      // }



  display.display();
}





void calculate_times() {
  PTmain_min = 255;
  PTmain_max = 0;
  PTmain_time = 0.0;
  PTsub_min = 255;
  PTsub_max = 0;
  PTsub_time = 0.0;

  for (int variable = 0; variable < read_time * 10; variable++) {
    if ((PTmain_decimal[variable] < PTmain_min) && (PTmain_decimal[variable] != 0)) { PTmain_min = PTmain_decimal[variable]; }
    if ((PTsub_decimal[variable] < PTsub_min) && (PTsub_decimal[variable] != 0)) { PTsub_min = PTsub_decimal[variable]; }
    if ((PTmain_decimal[variable] > PTmain_max) && (PTmain_decimal[variable] != 0)) { PTmain_max = PTmain_decimal[variable]; }
    if ((PTsub_decimal[variable] > PTsub_max) && (PTsub_decimal[variable] != 0)) { PTsub_max = PTsub_decimal[variable]; }
  }

  for (int variable = query_time * 10; variable < REPORT_TIME * 10; variable++) {

    if ((OVER >= 0) && (PTmain_time == 0.0) && (PTmain_decimal[variable] > PTmain_max + OVER) && (PTmain_decimal[variable + 1] > PTmain_max + OVER) && (PTmain_decimal[variable + 2] > PTmain_max + OVER) && (PTmain_decimal[variable] != 0))  //***add for all?
    {
      PTmain_time = variable;
      PTmain_results[trial_number] = float((PTmain_time) / 10 - read_time);
    }

    if ((OVER >= 0) && (PTsub_time == 0.0) && (PTsub_decimal[variable] > PTsub_max + OVER) && (PTsub_decimal[variable + 1] > PTsub_max + OVER) && (PTsub_decimal[variable + 2] > PTsub_max + OVER) && (PTsub_decimal[variable] != 0)) {
      PTsub_time = variable;
      PTsub_results[trial_number] = float((PTsub_time) / 10 - read_time);
    }

    if ((under >= 0) && (PTmain_time == 0.0) && (PTmain_decimal[variable] < PTmain_min - under) && (PTmain_decimal[variable + 1] < PTmain_min - under) && (PTmain_decimal[variable + 2] < PTmain_min - under) && (PTmain_decimal[variable] != 0)) {
      PTmain_time = variable;
      PTmain_results[trial_number] = float((PTmain_time) / 10 - read_time);
    }

    if ((under >= 0) && (PTsub_time == 0.0) && (PTsub_decimal[variable] < PTsub_min - under) && (PTsub_decimal[variable + 1] < PTsub_min - under) && (PTsub_decimal[variable + 2] < PTsub_min - under) && (PTsub_decimal[variable] != 0)) {
      PTsub_time = variable;
      PTsub_results[trial_number] = float((PTsub_time) / 10 - read_time);
    }
  }
}



void calculate_averages() {
  PTmain_sum = 0;
  PTmain_count = 0;
  PTmain_avg = 0.0;
  PTsub_sum = 0;
  PTsub_count = 0;
  PTsub_avg = 0.0;
  PTmain_fastest = 0.0;
  PTsub_fastest = 0.0;
  PTmain_slowest = 999.9;
  PTsub_slowest = 999.9;
  
  for (int variable = 0; variable < TRIAL_MAX; variable ++){
if (PTmain_results[variable] > PTmain_fastest){PTmain_fastest = PTmain_results[variable];}
      if ((PTmain_results[variable] != 0) && (PTmain_results[variable] < PTmain_slowest)){PTmain_slowest = PTmain_results[variable];}
      if (PTsub_results[variable] > PTsub_fastest){PTsub_fastest = PTsub_results[variable];}
      if ((PTsub_results[variable] != 0) && (PTsub_results[variable] < PTsub_slowest)){PTsub_slowest = PTsub_results[variable];}
  }


  for (int variable = 0; variable < 1000; variable++) {
      
    
    if (PTmain_results[variable] != 0) {
      PTmain_sum = PTmain_sum + PTmain_results[variable];
      PTmain_count++;

    }
    if (PTsub_results[variable] != 0) {
      PTsub_sum = PTsub_sum + PTsub_results[variable];
      PTsub_count++;
    }}

    if (fastest_slowest_mode == 0){
    if (PTmain_count != 0) { PTmain_avg = (float(PTmain_sum) / float(PTmain_count)); }
    if (PTsub_count != 0) { PTsub_avg = (float(PTsub_sum) / float(PTsub_count)); }
    }
    if (fastest_slowest_mode == 1){
    if (PTmain_count != 0) { PTmain_avg = ((PTmain_fastest+PTmain_slowest)/2); }
    if (PTsub_count != 0) { PTsub_avg = ((PTsub_fastest+PTsub_slowest)/2); }
    }

}


void create_sheet() {
  results = "";
  results = results + ",,,";

  if (calculate_mode == 0) {
    results = results + "Phototransistor - known hz,";
  } else if (calculate_mode == 1) {
    results = results + "Phototransistor - calculated,";
  } else if (calculate_mode == 0) {
    results = results + "Phototransistor - compare,";
  } else {
    results = results + ",";
  }
  if (calculate_mode == 1) {
    results = results + PTmain_top_b;
    results = results + ",";
    results = results + PTmain_top_b;
    results = results + ",";
  } else {
    results = results + PTmain_top_a;
    results = results + ",";
    results = results + PTsub_top_a;
    results = results + ",";
  }

  results = results + PTmain_avg;
  results = results + ",";
  results = results + PTsub_avg;
  results = results + ",";

  results = results + PTmain_count;
  results = results + ",";
  results = results + PTsub_count;
  results = results + ",";

  results = results + ",,,,,,,,,,,,,,,";

  results = results + upper;
  results = results + ",";
  results = results + lower;
  results = results + ",";

  results = results + ",,,,,";

  results = results + TRIAL_MAX;
  results = results + ",";
  results = results + TRIAL_MAX;
  results = results + ",";

  results = results + screen_hz_known;
  results = results + ",";
  results = results + screen_draw_time_known;
  results = results + ",";
  results = results + PTmain_top_a;
  results = results + ",";
  results = results + PTmain_bottom_a;
  results = results + ",";
  results = results + PTsub_top_a;
  results = results + ",";
  results = results + PTsub_bottom_a;
  results = results + ",";

  results = results + screen_hz_calculated;
  results = results + ",";
  results = results + screen_draw_time_calculated;
  results = results + ",";
  results = results + PTmain_top_b;
  results = results + ",";
  results = results + PTmain_bottom_b;
  results = results + ",";

  results = results + P1_wins;
  results = results + ",";
  results = results + P2_wins;
  results = results + ",";
  results = results + P1_P2_draw;
  results = results + ",";
}



void reset_all_values() {
  File writeraw = LittleFS.open("/raw.txt", "w"); //this will reset raw.txt to nothing  
  writeraw.close();

  trial_number = 0;
  for (int variable = 0; variable < 10000; variable++) {
    PTmain_decimal[variable] = 0;
    PTsub_decimal[variable] = 0;
  }
  for (int variable = 0; variable < 1000; variable++) {
    PTmain_results[variable] = 0;
    PTsub_results[variable] = 0;
  }
  trial_number = 0;
}

void reset_OPT_values() {
  PTmain_min = 255;
  PTmain_max = 0;
  PTsub_min = 255;
  PTsub_max = 0;
}




void save_settings() {
  File writefile = LittleFS.open("/settings.txt", "w");  // Open for writing
  for (int variable = 0; variable < length_array; variable++) {
    writefile.print(serial_names[variable]);
    writefile.println(*pointers[variable]);
  }
  for (int variable = 0; variable < float_length_array; variable++) {
    writefile.print(float_serial_names[variable]);
    writefile.println(*float_pointers[variable]);
  }
  writefile.close();
  Serial.println("Saved");
}




void load_settings() {
  File readfile = LittleFS.open("/settings.txt", "r");  // Open for reading
  if (readfile) {
    Serial.print("starting");
    while (readfile.available()) {
      String line = readfile.readStringUntil('\n');
      for (int variable = 0; variable < length_array; variable++) {  /////check this length is right
        if (line[0] == serial_names[variable]) {
          line.remove(0, 1);
          int temp_int = line.toInt();
          *pointers[variable] = temp_int;
          Serial.print(serial_names[variable]);
          Serial.println(*pointers[variable]);
        }
      }

      for (int variable = 0; variable < float_length_array; variable++) {  /////check this length is right
        if (line[0] == float_serial_names[variable]) {
          line.remove(0, 1);
          float temp_float = line.toFloat();
          *float_pointers[variable] = temp_float;
          Serial.print(float_serial_names[variable]);
          Serial.println(*float_pointers[variable]);
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

  //if(char(youvegotmail)=='_'){Serial.print("ready");Serial.println(mode);}
  

  if(char(youvegotmail) == 'y'){cursor_position = youvegotemail.toFloat();}
  if(char(youvegotmail) == 'Y'){cursor_position = youvegotemail.toFloat();current_button = 5;}
  
  if(char(youvegotmail) == '?'){
    for (int variable = 0; variable < length_array; variable++) {
    SerialBT.print(serial_names[variable]);
    SerialBT.println(*pointers[variable]);
    Serial.print(serial_names[variable]);
    Serial.println(*pointers[variable]);}

      for (int variable = 0; variable < float_length_array; variable++) {
    SerialBT.print(float_serial_names[variable]);
    SerialBT.println(*float_pointers[variable]);
    Serial.print(float_serial_names[variable]);
    Serial.println(*float_pointers[variable]);
    }}
  
  for (int variable = 0;variable < 15; variable++){//change to 15 in next update
  if(char(youvegotmail) == serial_names[variable]){
    if((youvegotemail.toInt() >= min_list[variable]) && (youvegotemail.toInt() <= max_list[variable])){
    *pointers[variable] = youvegotemail.toInt();
    SerialBT.print(serial_names[variable]);SerialBT.println(*pointers[variable]);}
    if (*pointers[variable] == 0) {
    *pointers[variable + 15] = -5;
    SerialBT.print(serial_names[variable + 15]);SerialBT.println(*pointers[variable + 14]);}
    save_settings();}
  }

  
  for (int variable = 15; variable < 30; variable++) {//change to 15-30 in next update
  if(char(youvegotmail) == serial_names[variable]){
    if((youvegotemail.toInt() >= min_list[variable]) && (youvegotemail.toInt() <= max_list[variable])){
      *pointers[variable] = youvegotemail.toInt();
      SerialBT.print(serial_names[variable]);SerialBT.println(*pointers[variable]);}
      if (*pointers[variable] == 0) {
      *pointers[variable - 15] = -5;
      SerialBT.print(serial_names[variable - 15]);SerialBT.println(*pointers[variable - 15]);}
      save_settings();
    }}

  for (int variable = 30; variable < length_array; variable++) {  
    if(char(youvegotmail) ==  serial_names[variable]) {
      if((youvegotemail.toInt() >= min_list[variable]) && (youvegotemail.toInt() <= max_list[variable])){
    *pointers[variable] = youvegotemail.toInt();
    Serial.print(serial_names[variable]);Serial.println(*pointers[variable]);
    save_settings();}
  }}

    for (int variable = 0; variable < float_length_array; variable++) {
    if (char(youvegotmail) == float_serial_names[variable]) {
    *float_pointers[variable] = youvegotemail.toFloat();
    if (upper < 0) { upper = 100 * (abs(upper) - top_of_screen) / (bottom_of_screen - top_of_screen); }
    if (lower < 0) { lower = 100 * (abs(lower) - top_of_screen) / (bottom_of_screen - top_of_screen); }
    SerialBT.print(float_serial_names[variable]);SerialBT.println(*float_pointers[variable]);
    save_settings();
    }}

youvegotmail = 0;
youvegotemail = "";
}
}




void setup() {

  for (int variable = 0; variable < 15; variable++) {
    pinMode(OUTPUT_BUTTONS[variable], OUTPUT);
    digitalWrite(OUTPUT_BUTTONS[variable], HIGH);
  }

  Serial.begin(115200);

  

  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(CONFIRM, INPUT_PULLUP);
  pinMode(CANCEL, INPUT_PULLUP);

  pinMode(PTmain, INPUT);  /////
  pinMode(PTsub, INPUT);  /////



  
  

  pinMode(A3, INPUT);
  LittleFS.begin();

  cursor_position = initial_cursor_position;

  PTmain_current = analogRead(PTmain) / 4;
  PTsub_current = analogRead(PTsub) / 4;
  if (PTmain_current == 4){OLED_message4 = "Check PT_main";}
  if ((PTsub_current == 4) && (OLED_message4 == "Check PT_main")){OLED_message5 = "Check PT_sub";}

  if (digitalRead(CANCEL) == 0) { save_settings(); cursor_position = -4;last_button = 1;}  // by doing this we can reset to default
  
  if (digitalRead(RIGHT) == 1){//unpressed
//load password

  File password_load = LittleFS.open("/password.txt", "r");  // Open for reading
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

  
  if (digitalRead(RIGHT) == 0){wifimode = 0;} // force AP mode if password change etc. needed
   //pressed
  
if (wifimode == 1){
  //Serial.print(ssid);
  //Serial.print(password);
  
  //use this to connect to local network via ssid and password
  //WiFi.begin(ssid.c_str(), password.c_str());  
  WiFi.begin(ssid.c_str(), password.c_str()); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("\nConnected! Open this IP: ");
  Serial.println(WiFi.localIP());
  OLED_message3 = WiFi.localIP().toString();}


if (wifimode == 0){//use this to change to access point mode
WiFi.beginAP(ssid.c_str(), password.c_str());
Serial.println(WiFi.softAPIP());
OLED_message1 = ssid;
OLED_message2 = password; 
OLED_message3 = WiFi.softAPIP().toString();

}


  server.begin();
;



  
  load_settings();
  
  if (digitalRead(LEFT)==0){
    connect_sub_board();//   
    }

  
  
  

  Serial.print(length_array);


  SerialBT.setName("noodalls2.0  00:00:00:00:00:00");
  SerialBT.begin();
  SerialBT.setTimeout(200);
  Serial.setTimeout(200);
  //SerialBT.setTimeout(10000);
  //cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE);




  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  //display.setRotation(2); //4/2026
  trial_number = 0;


  for (int variable = 0; variable < 10000; variable++) {
    PTmain_decimal[variable] = 0;
    PTsub_decimal[variable] = 0;
  }
  for (int variable = 0; variable < 255; variable++) {
    PTmain_summary[variable] = 0;//***
    PTsub_summary[variable] = 0;//***
  }
}


void loop() {

  ///test here
  //if (ssid == ssid){Serial.println("They're the same picture");}
  // Serial.print("length = ");Serial.println(ssid.length());
  // Serial.print("length1 = ");Serial.println(ssid1.length());

  ///test here

  time1 = micros() / 100;
  if ((time1) != (time2)) {

    PTmain_current = analogRead(PTmain) / 4;
    PTsub_current = analogRead(PTsub) / 4;
    elapsed_time = (time1 - time3);
    elapsed_time_ms = elapsed_time / 10;
    if (digitalRead(CANCEL) == 0) {
      current_button = 1;
      last_button = 1;
      mode = 0;
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      pinMode(LEFT, INPUT_PULLUP);
      pinMode(RIGHT, INPUT_PULLUP);
      pinMode(CONFIRM, INPUT_PULLUP);
    }
    
    
    
    if (mode == 1) {
//< 1000??
      if (elapsed_time < 10000){
      PTmain_decimal[elapsed_time] = PTmain_current;
      PTsub_decimal[elapsed_time] = PTsub_current;
      }
      for (int variable = 0; variable < 15; variable++) {
        if (elapsed_time_ms == OUTPUT_ON_TIME[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }    //buttons on
        if (elapsed_time_ms == output_off_time[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], output_off_state[variable]); }  //buttons_off
        //for Brook pins, HIGH is unpressed, LOW is pressed
      }


      if (elapsed_time_ms < read_time) {
        if (PTmain_current > PTmain_max) { PTmain_max = PTmain_current; }
        if (PTmain_current < PTmain_min) { PTmain_min = PTmain_current; }
        if (PTsub_current > PTsub_max) { PTsub_max = PTsub_current; }
        if (PTsub_current < PTsub_min) { PTsub_min = PTsub_current; }
      }

      if (elapsed_time_ms == read_time) {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
      }

      if (elapsed_time_ms > query_time) {
        if ((cursor_position >= 1) && (cursor_position <= 14)) {
          if ((OVER >= 0) && (PTmain_current > (PTmain_max + OVER))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
          }

          if ((OVER >= 0) && (PTsub_current > (PTsub_max + OVER))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
          }

          if ((under >= 0) && (PTmain_current < (PTmain_min - under))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
          }

          if ((under >= 0) && (PTsub_current < (PTsub_min - under))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
          }
        }
      }

      if (elapsed_time_ms == REPORT_TIME) {
        if ((cursor_position >=1) && (cursor_position <= 7)) {
          calculate_times();
          calculate_averages();
          calculate_screen_time();
          calculate_winners();
          if ((save_mode == 0) && (trial_number < TRIAL_MAX)){save_summary();}
          for (int variable = 0; variable < 255; variable++) {
          PTmain_summary[variable] = 0;//***
          PTsub_summary[variable] = 0;}//***
        }
        if (cursor_position == 8) {//PT comp
          if ((PTmain_results[trial_number] != 0) && (PTsub_results[trial_number] != 0)) {
            if ((PTsub_results[trial_number] - PTmain_results[trial_number]) > PLAY1P2P) {
              digitalWrite(GREEN_LED, LOW);
              digitalWrite(BLUE_LED, HIGH);
              digitalWrite(RED_LED, LOW);
            }
            if ((PTmain_results[trial_number] - PTsub_results[trial_number]) > PLAY1P2P) {
              digitalWrite(GREEN_LED, LOW);
              digitalWrite(BLUE_LED, LOW);
              digitalWrite(RED_LED, HIGH);
            }
          }
        }

        display_screen();
        

        if (Serial.available() > 0){
        youvegotmail = Serial.read();}

        if (SerialBT.available() > 0){
        youvegotmail = SerialBT.read();}
       

        if (char(youvegotmail) == '$'){
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            pinMode(LEFT, INPUT_PULLUP);
            pinMode(RIGHT, INPUT_PULLUP);
            pinMode(CONFIRM, INPUT_PULLUP);
        mode = 0;youvegotmail = 0;youvegotemail = "";}

trial_number = trial_number + 1;
        
        }



      if (elapsed_time_ms >= QUIT_TIME) {
        
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);

        if (only_one_test==1) { //odd modes
          reset_OPT_values();
          pinMode(LEFT, INPUT_PULLUP);
          pinMode(RIGHT, INPUT_PULLUP);
          pinMode(CONFIRM, INPUT_PULLUP);
          mode = 0;//detailed_graphics = 1;
        }
        //if (cursor_position == -1){trial_number = trial_number -1;mode = 11;}

        //if (cursor_position%2 == 0) {
          reset_OPT_values();
          time3 = 1 + micros() / 100;
        //}
      }

      if (trial_number == TRIAL_MAX) {
        if ((cursor_position >=1) && (cursor_position <= 7)) {
          create_sheet();
          File writesummary = LittleFS.open("/summary.txt", "a");
          writesummary.print("U");
          writesummary.print(",");
          for (int variable = 0; variable < TRIAL_MAX; variable++) {
            writesummary.print(PTmain_results[variable]);
            writesummary.print(",");
          }
          writesummary.println();
          writesummary.print("V");
          writesummary.print(",");
          for (int variable = 0; variable < TRIAL_MAX; variable++) {
            writesummary.print(PTsub_results[variable]);
            writesummary.print(",");
          }
          writesummary.println();
          writesummary.close();

          File writeresults = LittleFS.open("/results.txt", "a");  // Open for appending to the end
          writeresults.println(results);
          writeresults.close();
          Serial.println("Saved results");
        }

        // if (cursor_position%2 == 0) {
          pinMode(LEFT, INPUT_PULLUP);
          pinMode(RIGHT, INPUT_PULLUP);
          pinMode(CONFIRM, INPUT_PULLUP);
          mode = 0;//detailed_graphics = 1;
        // }
      }
    }

    if (mode == -1) {
      /// BUTTON INPUTS HERE
      
      current_button = 0;
      if (digitalRead(LEFT) == 0) { current_button = 4; }
      if (digitalRead(RIGHT) == 0) { current_button = 6; }
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }

      if ((current_button == 4) && (last_button == 0)) {
        trial_number = max(0, trial_number - 1);
        //Serial.print(trial_number);
      }
      if ((current_button == 6) && (last_button == 0)) {
        trial_number = min(TRIAL_MAX, trial_number + 1);
        //Serial.print(trial_number);
      }

      display_screen();

      if ((current_button == 5) && (last_button == 0) && (trial_number < TRIAL_MAX)) {
        reset_OPT_values();
        pinMode(RED_LED, OUTPUT);
        pinMode(GREEN_LED, OUTPUT);
        pinMode(BLUE_LED, OUTPUT);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        time3 = 1 + micros() / 100;
        prepare_summary();
        mode = 1;
      }
      
    }



    if (mode == -2) {

      Serial.println(results);
      SerialBT.println(results);
      mode = 0;
    }

    if (mode == -3) {
      File readresults = LittleFS.open("/results.txt", "r");  // Open for appending to the end
      while (readresults.available()) {
        char temp_char = readresults.read();
        Serial.write(temp_char);
        SerialBT.write(temp_char);  // Read and print byte by byte
      }
      Serial.println();  // Add a newline for better formatting
      readresults.close();

      mode = 0;
    }

    if (mode == -4) {  //use this to delete all the results
      current_button = 0;
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }
      if ((current_button == 5) && (last_button == 0)) {

        File writeresults = LittleFS.open("/results.txt", "w");  // Open for appending to the end
        writeresults.close();
        Serial.println("deleted");
        last_button = 5;
        cursor_position = initial_cursor_position;
        mode = 0;
      }
    }

    if (mode == -6) {                                         // use this to print out summary of last testing process
      File readsummary = LittleFS.open("/summary.txt", "r");  // 33608
      while (readsummary.available()) {
        char temp_char = readsummary.read();
        Serial.write(temp_char);
      }                  // Read and print byte by byte
      Serial.println();  // Add a newline for better formatting
      readsummary.close();

      mode = 0;
    }

    if (mode == -7) {  // use this just to test how long a process takes
      timea = micros();


      timeb = micros();
      test_time = timeb - timea;
      mode = 0;
    }



    if (mode == 0) {
      display_screen();

      current_button = 0;

      // turn_off_buttons();    
      if (cursor_position < 1) {
      for (int variable = 0; variable < 15; variable++) {
      digitalWrite(OUTPUT_BUTTONS[variable], output_reset_state[variable]);
      }}

      if (cursor_position > 0) {
      for (int variable = 0; variable < 15; variable++) {
      if (output_off_time[variable] >= 0) { digitalWrite(OUTPUT_BUTTONS[variable], output_off_state[variable]); }
      if (output_off_time[variable] < 0) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }
      if (output_off_time[variable] < OUTPUT_ON_TIME[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }
      }}




        if (Serial.available() > 0){
        youvegotmail=Serial.read();
        youvegotemail =  Serial.readStringUntil('\n');youvegotemail.trim();}

        if (SerialBT.available() > 0){
        youvegotmail = SerialBT.read();
        youvegotemail =  SerialBT.readStringUntil('\n');youvegotemail.trim();}






//////HTML LOOP

WiFiClient client = server.accept();
if (client) {
Serial.println("New browser request.");

while (client.connected()) {
if (client.available()) {
char c = client.read();
  if (c == '\n') {

    if(whole_url.length() == 0){
      ///main HTML page

      // Standard HTTP response headers
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println(); // Mandatory blank line separating headers from HTML

      // The Basic HTML Document Content
      client.println("<!DOCTYPE html><html>");
      client.println("<style>button { width: 250px; height: 80px; font-size: 22px; margin: 10px; }</style>");

      client.println("<a href=\"/set?Y=1\"><button>Start PTmain</button></a>");
      client.println("<a href=\"/set?Y=2\"><button>Start PTsub</button></a>");
      client.println("<a href=\"/\"><button>Refresh</button></a>");
      client.println("<p></p>");

      client.println("<h1>" + String(trial_number) + " / "+String(TRIAL_MAX) +  "trials</h1>");

      client.println("<h1>PTmain results = "+ String(PTmain_results[trial_number])+"ms (AVG = "+String(PTmain_avg)+"ms)</h1>");
      client.println("<h1>PTsub results = "+ String(PTsub_results[trial_number])+"ms (AVG = "+String(PTsub_avg)+"ms)</h1>");

      client.println("<h1>PTmain reading: " + String(PTmain_current) + " PTsub reading: "+String(PTsub_current) +  "</h1>");

      client.println("<p></p>");
      client.println("<a href=\"/set?*\"><button>UP</button></a>");
      client.println("<a href=\"/set?%\"><button>OK</button></a>");

      client.println("<p></p>");
      client.println("<a href=\"/set?&\"><button>DOWN</button></a>");
      client.println("<a href=\"/set?$\"><button>CANCEL</button></a>");

      client.println("<p></p>");

      client.println("  <canvas id=\"canvas_last\" width=\""+String(REPORT_TIME)+"\" height=\"255\" style=\"border:1px solid black;\"></canvas>");
      client.println("  <script>");
      client.println("    var ctx_last = document.getElementById(\"canvas_last\").getContext(\"2d\");");
      //client.println("ctx_last.imageSmoothingEnabled = false;");
      client.println("    ctx_last.fillStyle = \"blue\";");
      for (int variable = 0;variable < REPORT_TIME;variable ++){
      client.println("    ctx_last.fillRect("+String(variable)+","+String(255-PTmain_decimal[variable*10])+", 1, 1);");}
      client.println("    ctx_last.fillStyle = \"red\";");
      for (int variable = 0;variable < REPORT_TIME;variable ++){
      client.println("    ctx_last.fillRect("+String(variable)+","+String(255-PTsub_decimal[variable*10])+", 1, 1);");}
      client.println("  </script>");

      client.println("<p></p>");

      //client.println("  <canvas id=\"canvas_avg\" width=\""+String(TRIAL_MAX)+"\" height=\"255\" style=\"border:1px solid black;\"></canvas>");
      client.println("  <canvas id=\"canvas_avg\" width=\""+String(TRIAL_MAX)+"\" height=\""+String(REPORT_TIME-read_time)+"\" style=\"border:1px solid black;\"></canvas>");
      client.println("  <script>");
      client.println("    var ctx_avg = document.getElementById(\"canvas_avg\").getContext(\"2d\");");
      //client.println("ctx_avg.imageSmoothingEnabled = false;");
      client.println("    ctx_avg.fillStyle = \"blue\";");
      for (int variable = 0;variable < TRIAL_MAX;variable ++){
      client.println("    ctx_avg.fillRect("+String(variable)+","+String(REPORT_TIME-read_time-int(PTmain_results[variable]))+", 1, 1);");}
      client.println("    ctx_avg.fillStyle = \"red\";");
      for (int variable = 0;variable < TRIAL_MAX;variable ++){
      client.println("    ctx_avg.fillRect("+String(variable)+","+String(REPORT_TIME-read_time-int(PTsub_results[variable]))+", 1, 1);");}
      client.println("  </script>");
      client.println("<p></p>");

      client.println("<details>");
      client.println("  <summary>Click for results</summary>");            
      client.println("<p></p>");

      client.println("<a href=\"/detailed\"><button>SHOW GRAPHS</button></a>");
      client.println("<p></p>");

      for (int n = 0;n<TRIAL_MAX;n++){  
      client.println("<p>"+String(n) + " = " + String(PTmain_results[n])+"ms, "+String(PTsub_results[n])+"ms</p>");

      if (detailed_graphics == 1){
      client.println("  <canvas id=\"canvas"+String(n)+"\" width=\"255\" height=\"255\" style=\"border:1px solid black;\"></canvas>");
      // client.println("  <script>");
      // client.println("    var ctx"+String(n)+" = document.getElementById(\"canvas"+String(n)+"\").getContext(\"2d\");");
      // client.println("  </script>");
      }}
      client.println("</details>");
      client.println("<p></p>");
      if (detailed_graphics == 1){
      File rawfile = LittleFS.open("/raw.txt", "r");  // Open for reading
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
      //Serial.print(raw_x_coord);Serial.print(",");
      //Serial.println(raw_line);
      
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
      //client.println("  <p>This is the hidden text revealed upon clicking.</p>");
      client.println("<p></p>");
      for (int n = 0;n<30;n++){  
      client.println("<form action='/set' method='GET'>");
      client.println("  <input type='number'  name="+String(serial_names[n])+" placeholder="+String(*pointers[n]) +">");
      client.println("  <input type='submit' value="+String(serial_titles[n])+">");
      client.println("</form>");}
      client.println("</details>");
      client.println("<p></p>");

      client.println("<details>");
      client.println("  <summary>Click for test settings</summary>");
      //client.println("  <p>This is the hidden text revealed upon clicking.</p>");
      client.println("<p></p>");
      for (int n = 30;n<length_array;n++){  
      client.println("<form action='/set' method='GET'>");
      client.println("  <input type='number'  name="+String(serial_names[n])+" placeholder="+String(*pointers[n]) +">");
      client.println("  <input type='submit' value="+String(serial_titles[n])+">");
      client.println("</form>");}
      client.println("</details>");
      client.println("<p></p>");


      client.println("<details>");
      client.println("  <summary>Click for screen settings</summary>");
      //client.println("  <p>This is the hidden text revealed upon clicking.</p>");
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

      // Visual heading on the page
      //client.print("  <h1 id='display-heading'>Input Lag Testing</h1>");

      // 3. Send Text Box and Button (Plain HTML, no form submit/reload)

      client.print("  <input type='text' id='titleInput' placeholder='Type new title...'");
      client.print("<style='width: 200px; height: 20px; font-size: 22px; padding: 10px;'>");
      client.print("  <button onclick='updateTitle()'>Update Title</button>");

      client.print("  <script>");
      client.print("    function updateTitle() {");
      client.print("      var newTitle = document.getElementById('titleInput').value;");
      client.print("      if(newTitle.trim() !== '') {");
      client.print("        document.title = newTitle;"); // Changes browser tab title
      client.print("        document.getElementById('display-heading').innerText = newTitle;"); // Updates heading
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
      String ssid = whole_url.substring(startPos,endPos);
      Serial.println("ssid");
      Serial.println(ssid);

      startPos = whole_url.indexOf("val2=")+5;
      endPos = whole_url.indexOf(" ",startPos);
      String password = whole_url.substring(startPos,endPos);
      Serial.println("password");Serial.println(password);

      File password_save = LittleFS.open("/password.txt", "w");  // Open for writing
      password_save.println(ssid);
      password_save.println(password);
      password_save.close();
      }  
      if (whole_url.indexOf("GET /set?") >= 0) {
      Serial.println("Looking");
      int startPos = whole_url.indexOf("GET /set?=") + 10;
      endPos = whole_url.indexOf(" ", startPos);    
      youvegotmail = whole_url[9];          
      youvegotemail = whole_url.substring(startPos+2, endPos);youvegotemail.trim();
      Serial.println((youvegotmail));
      Serial.println((youvegotemail));   
      
      client.println("HTTP/1.1 303 See Other");//will this be needed?
      client.println("Location: /");
      client.println("Connection: close");
      client.println(); // Empty line to end headers
      }
    }
    whole_url = "";
      }}
      else if (c !='\r'){whole_url += c;}
  }
  }

delay(10); // Give the browser brief time to receive data
client.stop(); // Disconnect the client
Serial.println("Client disconnected.");
}


/////HTML LOOP

      inbox();


      if (digitalRead(CANCEL) == 0) { current_button = 1; }
      if (digitalRead(LEFT) == 0) { current_button = 4; }
      if (digitalRead(RIGHT) == 0) { current_button = 6; }
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }

      if((current_button == 4) && (last_button == 1)){current_button = 11;save_mode = !save_mode;delay(300);}
      if((current_button == 6) && (last_button == 1)){current_button = 11;only_one_test = !only_one_test;delay(300);}
      if((current_button == 5) && (last_button == 1)){current_button = 11; fastest_slowest_mode = !fastest_slowest_mode;delay(300);}
      
      if ((digitalRead(CONFIRM) == 0) && (digitalRead(RIGHT) == 0)) { current_button = 10; }
      

      if ((current_button == 6) && (last_button == 4)) {
        initial_cursor_position = cursor_position;
        save_settings();
      }
 



      if ((current_button == 10) && (last_button != 10)) {
        trial_number = max(0, trial_number - 1);
        cursor_position = 1;
      }

      if ((current_button == 1) && (last_button == 0) && (mode == 0)) { cursor_position = 0; }

      if ((current_button == 4) && (last_button == 0)) { cursor_position = max(-13, cursor_position - 1); }
      if ((current_button == 6) && (last_button == 0)) { cursor_position = min(52, cursor_position + 1); }

      if ((current_button == 5) && (last_button == 0) && (cursor_position == 0)) {  //EEPROM_RESET();
        reset_all_values();

        
        last_button = 5;
        cursor_position = initial_cursor_position;
        mode = 0;
      }

      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 3)){calculate_mode = 0;}
      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 4)){calculate_mode = 1;}
      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 9)){trial_number = TRIAL_MAX + 1;}



      if ((current_button == 5) && (last_button == 0) &&  (cursor_position == 10)){connect_sub_board();}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -1)){mode = -1;last_button = 5;}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -11)) { mode = -11; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -12)) { mode = -12; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -13)) { mode = -13; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -2)) { mode = -2; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -3)) { mode = -3; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -4)) {mode = -4;last_button = 5;}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -6)) { mode = -6; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position > 0) && (cursor_position < 10)) {
        reset_OPT_values();
        pinMode(RED_LED, OUTPUT);
        pinMode(GREEN_LED, OUTPUT);
        pinMode(BLUE_LED, OUTPUT);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);        
        if ((cursor_position >=1) && (cursor_position <=8) && (trial_number < TRIAL_MAX)){prepare_summary();}
        time3 = 1 + micros() / 100;
        mode = 1;}


    }

    last_button = current_button;
    time2 = micros() / 100;
  }
}
