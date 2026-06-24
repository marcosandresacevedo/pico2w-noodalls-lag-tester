//https://arduino-pico.readthedocs.io/en/latest/install.html#installing-via-arduino-boards-manager
// make sure you follow here to install the PICO2W board

//in Tools I have IP/Bluetooth stack set to IPv4 + Bluetooth
//in Tools I have Flash size set to 4MB(Sketch 2MB FS 2MB )

//Tested version of board is here https://oshpark.com/shared_projects/scDfyYnd
//Tested version of phototransistor board is hered https://oshpark.com/shared_projects/rQflS270


//Beta version of board is here https://oshpark.com/shared_projects/tErOh6uE
//Beta phototransistor board is here https://oshpark.com/shared_projects/tjg3CdkS

//playlist showing soldering and setup is here
//https://www.youtube.com/playlist?list=PLevqZFjlWIdexYMa2yQZgw0dfLVjzN8m7


#include "LittleFS.h"
#include <SerialBT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int mode = 0;
int cursor_position = 0;
int initial_cursor_position = 1;


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

int calculate_mode = 0;//0 = Phototransistor known, 1 = phototransistor calc, 2 = phototransistor compare
int fastest_slowest_mode = 0;
int only_one_test = 0;

float upper = 92;
float lower = 92;
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
int QUIT_TIME = 499;

int OPT1 = 28;
int OPT2 = 26;

float OPT1_time = 0;
float OPT2_time = 0;
int OPT1_min = 255;
int OPT1_max = 0;
int OPT2_min = 255;
int OPT2_max = 0;

int P1_wins = 0;
int P2_wins = 0;
int P1_P2_draw = 0;

//future - pin 6 is red and left, pin 7 is green and right, pin 8 is blue and confirm, pin 9 is cancel // need to go back and confirm



int LEFT = 6;
int RIGHT = 7;
int CONFIRM = 8;
int CANCEL = 9;

int BLUE_LED = 6;
int GREEN_LED = 7;
int RED_LED = 8;


int PPS = 22;

int external_trigger = 0;

int current_button = 0;
int last_button = 0;

String results = "";



//reset
//A-5 B-5 C-5 D-5 E-5 F-5 G-5 H-5 I-5 J-5 K-5 L-5 M100 N101
//a0 b0 c0 d0 e0 f0 g0 h0 i0 j0 k0 l0 m50 n50

int OUTPUT_ON_TIME[] = { -5, -5, -5, -5,  100, -5, -5, -5, -5, -5, -5, -5, 100, 101 };
int output_off_time[] = { 0, 0, 0, 0, 133, 0, 0, 0, 0, 0, 0, 0, 50, 50 };
int OUTPUT_ON_STATE[]    = {0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1};
int output_off_state[]   = {1,1,1,1, 1,1,1,1, 1,1,1,1, 0,0};
int output_reset_state[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1};

int OUTPUT_BUTTONS[] = { 10, 21, 11, 20, 19, 12, 18, 13, 14, 17, 15, 16, 2, 3 };
// U,


String serial_titles[] = { "UP_ON", "DOWN_ON", "RIGHT_ON", "LEFT_ON", "1P_ON", "1P_ON", "3P_ON", "4P_ON", "1K_ON", "2K_ON", "3K_ON", "4K_ON", "OPT1_OFF", "OPT2_OFF",
                           "up_off", "down_off", "right_off", "left_off", "1p_off", "2p_off", "3p_off", "4p_off", "1k_off", "2k_off", "3k_off", "4k_off", "opt1_on", "opt2_on",
                           "trial_number", "TRIAL_MAX", "read_time", "REPORT_TIME", "query_time", "QUIT_TIME", "over", "under", "players", "1P2PCOMP", "Initial cursor", "fastest_slowest_mode","only_one_test" };

String float_serial_titles[] = {
  "upper_value",
  "lower_value",
  "top_of_screen",
  "bottom_of_screen",
  "screen_hz",
  "screen_time_ms",
};

char float_serial_names[] = {
  'w',
  'W',
  's',
  'S',
  'x',
  'X',
};

char serial_names[] = {
  'A',
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'a',
  'b',
  'c',
  'd',
  'e',
  'f',
  'g',
  'h',
  'i',
  'j',
  'k',
  'l',
  'm',
  'n',
  't',
  'T',
  'r',
  'R',
  'q',
  'Q',
  'o',
  'O',
  'p',
  'P',
  'z',
  '!',
  '@'
};



int min_list[] = { -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
                   -1, -1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, -5, 0, 0 };

int max_list[] = { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999,
                   999, 999, 99, 99, 1999, 1999, 1999, 1999, 50, 50, 2, 0, 54, 1, 1 };


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

  &trial_number,
  &TRIAL_MAX,
  &read_time,
  &REPORT_TIME,
  &query_time,
  &QUIT_TIME,
  &over,
  &UNDER,
  &player,
  &PLAY1P2P,
  &initial_cursor_position,
  &fastest_slowest_mode,
  &only_one_test
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
    OPT1_summary[variable] = 0;
    OPT2_summary[variable] = 0;
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

void save_summary() {
  File writesummary = LittleFS.open("/summary.txt", "a");
  writesummary.print("u");
  writesummary.print(",");
  writesummary.print(trial_number);
  writesummary.print(",");
  for (int variable = 0; variable < 255; variable++) {
    writesummary.print(OPT1_summary[variable]);
    writesummary.print(",");
  }
  writesummary.println();
  writesummary.print("v");
  writesummary.print(",");
  writesummary.print(trial_number);
  writesummary.print(",");
  for (int variable = 0; variable < 255; variable++) {
    writesummary.print(OPT2_summary[variable]);
    writesummary.print(",");
  }
  writesummary.println();


  writesummary.close();


  for (int variable = 0; variable < 255; variable++) {
    OPT1_summary[variable] = 0;
    OPT2_summary[variable] = 0;
  }
}


// void turn_off_buttons() {

//   if (cursor_position < 0) {
//     for (int variable = 0; variable < 12; variable++) {
//       digitalWrite(OUTPUT_BUTTONS[variable], HIGH);
//     }
//     for (int variable = 12; variable < 14; variable++) {
//       digitalWrite(OUTPUT_BUTTONS[variable], HIGH);
//     }
//   }


//   if (cursor_position > 0) {

//     for (int variable = 0; variable < 12; variable++) {
//       if (output_off_time[variable] >= 0) { digitalWrite(OUTPUT_BUTTONS[variable], HIGH); }
//       if (output_off_time[variable] < 0) { digitalWrite(OUTPUT_BUTTONS[variable], LOW); }
//     }

//     for (int variable = 12; variable < 14; variable++) {
//       if (output_off_time[variable] >= 0) { digitalWrite(OUTPUT_BUTTONS[variable], LOW); }
//       if (output_off_time[variable] < 0) { digitalWrite(OUTPUT_BUTTONS[variable], HIGH); }
//     }
//   }
//   if (cursor_position == 0) {
//     for (int variable = 0; variable < 12; variable++) {
//       digitalWrite(OUTPUT_BUTTONS[variable], HIGH);
//     }
//     for (int variable = 12; variable < 14; variable++) {
//       digitalWrite(OUTPUT_BUTTONS[variable], LOW);
//     }
//   }
// }

void setColorBGR(int _blue, int _green, int _red) {
  digitalWrite(BLUE_LED, _blue);
  digitalWrite(GREEN_LED, _green);
  digitalWrite(RED_LED, _red);
}

void calculate_screen_time() {//consider if upper == lower then set calculate_mode to 2

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

void calculate_winners() {
  P1_wins = 0;
  P2_wins = 0;
  P1_P2_draw = 0;


  for (int variable = 0; variable < 1000; variable++) {
    if ((OPT1_results[variable] != 0) && (OPT2_results[variable] != 0)) {
      if ((OPT2_results[variable] - OPT1_results[variable]) > PLAY1P2P) {
        P1_wins++;
      } else if ((OPT1_results[variable] - OPT2_results[variable]) > PLAY1P2P) {
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
  if (cursor_position == -11) { display.println("Controller variables"); }
  if (cursor_position == -12) { display.println("Integer variables"); }
  if (cursor_position == -13) { display.println("Float variables"); }

  if (cursor_position == 0) { display.println("RESET"); }

  if (cursor_position == 1) { display.print("OPT1"); }
  if (cursor_position == 2) { display.print("OPT2"); } 
  if (cursor_position == 3) { display.println("Screen time known"); }
  if (cursor_position == 4) { display.println("Screen time calc"); }
  if (cursor_position == 5) { display.println("Compare 1P2P"); }  
  if (cursor_position == 6) { display.println("OPT1+2 Graph"); }
  if (cursor_position == 7) { display.println("OPT1+2 Summary"); }
  if (cursor_position == 8) { display.println("Non PT tests"); }
  if (cursor_position == 9) { display.println("Test_max"); }




  if (fastest_slowest_mode == 1){display.setCursor(120,0);display.print("!");}
    if (only_one_test == 1){display.setCursor(116,0);display.print("@");}

  if (cursor_position == -1) {
    display.setCursor(0, 56);display.print(trial_number);display.print("/");display.print(TRIAL_MAX);    
    display.setCursor(90, 16);display.print(OPT1_fastest);
    display.setCursor(90, 24);display.print(OPT1_slowest);
    display.setCursor(90, 32);display.print(OPT2_fastest);
    display.setCursor(90, 40);display.print(OPT2_slowest);
    if (mode == -1){
    display.setTextSize(2);
    display.setCursor(0, 16);display.print(OPT1_results[trial_number]);
    display.setCursor(0, 32);display.print(OPT2_results[trial_number]);
    display.setTextSize(1);}
    if (mode != -1){
    display.setTextSize(2);
    display.setCursor(0, 16);display.print(OPT1_avg);
    display.setCursor(0, 32);display.print(OPT2_avg);
    display.setTextSize(1);
    }
  }


  if (cursor_position == 1) {

    display.setCursor(0, 8);
    display.print(OPT1_avg);
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);

    display.setCursor(90, 8);
    display.print(OPT1_current);

    display.setTextSize(3);
    if (OPT1_results[trial_number] < 100) { display.setTextSize(4); }

    display.setCursor(0, 16);
    display.print(OPT1_results[trial_number]);
    display.setTextSize(1);
  }

  if (cursor_position == 2) {

        display.setCursor(110, 8);
    display.print(OPT2_current);

    display.setCursor(0, 8);
    display.print(OPT2_avg);

    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);

    display.setTextSize(3);
    if (OPT2_results[trial_number] < 100) {
      display.setTextSize(4);
    }

    display.setCursor(0, 16);
    display.print(OPT2_results[trial_number]);
    display.setTextSize(1);
  }


  if (cursor_position == 3||cursor_position == 4||cursor_position == 5) {
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);
    
    display.setCursor(64, 56);
    display.print("(");
    display.print(OPT1_count);
    display.print("/");
    display.print(OPT2_count);
    display.print(")");


    display.setCursor(0, 16);
    display.print(OPT1_results[trial_number]);
    display.print("/");
    display.print(OPT1_avg);
    display.setCursor(96, 16);
    display.print("P1/av");
    display.setCursor(0, 24);
    display.print(OPT2_results[trial_number]);
    display.print("/");
    display.print(OPT2_avg);
    display.setCursor(96, 24);
    display.print("P2/av");

    //display.setCursor(80, 56);display.print(OPT1_current);
    //display.setCursor(104, 56);display.print(OPT2_current);
    display.setCursor(90, 8);
    display.print(OPT1_current);
    display.setCursor(110, 8);
    display.print(OPT2_current);
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
    display.print(OPT1_top_a);
    display.print("/");
    display.print(OPT1_bottom_a);
    display.setCursor(0, 40);
    display.print(OPT2_top_a);
    display.print("/");
    display.print(OPT2_bottom_a);
    display.setCursor(0, 48);
    display.print(screen_hz_known);
    display.print("/");
    display.print(screen_draw_time_known);
  }


  if  (cursor_position == 4) {
    display.setCursor(0, 32);
    display.print(OPT1_top_b);
    display.print("/");
    display.print(OPT1_bottom_b);
    display.setCursor(0, 40);
    display.print(OPT1_top_b);
    display.print("/");
    display.print(OPT1_bottom_b);
    display.setCursor(0, 48);
    display.print(screen_hz_calculated);
    display.print("/");
    display.print(screen_draw_time_calculated);
  }

  if  (cursor_position == 5) {

    display.setCursor(0, 32);
    display.print(OPT1_top_a - OPT2_top_a);  //should this be average?

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




  if ((cursor_position >= 6) && (cursor_position <= 9)) {
    display.setCursor(0, 56);
    display.print(trial_number);
    display.print("/");
    display.print(TRIAL_MAX);
  }

  if (cursor_position ==6||cursor_position == 7){
    display.setCursor(0, 8);display.print(OPT1_results[trial_number]);display.print("/");display.print(OPT1_avg);
    display.setCursor(0, 16);display.print(OPT2_results[trial_number]);display.print("/");display.print(OPT2_avg);}


  if  (cursor_position == 6){
  for (int variable = 0; variable < 128; variable++){
    display.drawPixel(variable,64-int(OPT1_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
  for (int variable = 0; variable < 128; variable++){
    display.drawPixel(variable,64-int(OPT2_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
 
  }
  if (cursor_position ==7){
for (int n = 0;n<32;n++){
  if ((trial_number - n) >= 0){if(OPT1_results[trial_number-n] < 128){display.setCursor(int(OPT1_results[trial_number-n]),56-n);display.print(".");}}
  if ((trial_number - n) >= 0){if(OPT2_results[trial_number-n] < 128){display.setCursor(int(OPT2_results[trial_number-n]),56-n);display.print(".");}}

}}

    //display.drawPixel(20,20,SSD1306_WHITE);
    // display.setCursor(100, min(56,64-int(OPT1_decimal[query_time*10 + 1000]/4)));
    // display.println("OPT1");
    // if (cursor_position == 12||cursor_position == 13||cursor_position == 14){
    // display.setCursor(0, 8);display.print(OPT2_results[trial_number]);display.print("/");display.print(OPT2_avg);
    //       display.setCursor(100, min(56,64-int(OPT2_decimal[query_time*10 + 1000]/4)));
    // display.println("OPT2");
    // for (int variable = 0; variable < 128; variable++){
    //   display.drawPixel(variable,64-int(OPT2_decimal[query_time*10 + variable*10]/4), SSD1306_WHITE);}
    // }


  
  
  
  if (cursor_position == -11) {
    //display.clearDisplay();
    display.setCursor(0, 8);
    for (int variable = 0; variable < 4; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 16);
    for (int variable = 4; variable < 8; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 24);
    for (int variable = 8; variable < 12; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 32);
    for (int variable = 14; variable < 18; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 40);
    for (int variable = 18; variable < 22; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 48);
    for (int variable = 22; variable < 26; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    display.setCursor(0, 56);
    for (int variable = 12; variable < 14; variable++) {
      display.print(serial_names[variable]);
      display.print(*pointers[variable]);
      display.print(" ");
    }
    for (int variable = 26; variable < 28; variable++) {
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


  display.display();
}





void calculate_times() {
  OPT1_min = 255;
  OPT1_max = 0;
  OPT1_time = 0.0;
  OPT2_min = 255;
  OPT2_max = 0;
  OPT2_time = 0.0;

  for (int variable = 0; variable < read_time * 10; variable++) {
    if ((OPT1_decimal[variable] < OPT1_min) && (OPT1_decimal[variable] != 0)) { OPT1_min = OPT1_decimal[variable]; }
    if ((OPT2_decimal[variable] < OPT2_min) && (OPT2_decimal[variable] != 0)) { OPT2_min = OPT2_decimal[variable]; }
    if ((OPT1_decimal[variable] > OPT1_max) && (OPT1_decimal[variable] != 0)) { OPT1_max = OPT1_decimal[variable]; }
    if ((OPT2_decimal[variable] > OPT2_max) && (OPT2_decimal[variable] != 0)) { OPT2_max = OPT2_decimal[variable]; }
  }

  for (int variable = query_time * 10; variable < REPORT_TIME * 10; variable++) {

    if ((over >= 0) && (OPT1_time == 0.0) && (OPT1_decimal[variable] > OPT1_max + over) && (OPT1_decimal[variable + 1] > OPT1_max + over) && (OPT1_decimal[variable + 2] > OPT1_max + over) && (OPT1_decimal[variable] != 0))  //***add for all?
    {
      OPT1_time = variable;
      OPT1_results[trial_number] = float((OPT1_time) / 10 - read_time);
    }

    if ((over >= 0) && (OPT2_time == 0.0) && (OPT2_decimal[variable] > OPT2_max + over) && (OPT2_decimal[variable + 1] > OPT2_max + over) && (OPT2_decimal[variable + 2] > OPT2_max + over) && (OPT2_decimal[variable] != 0)) {
      OPT2_time = variable;
      OPT2_results[trial_number] = float((OPT2_time) / 10 - read_time);
    }

    if ((UNDER >= 0) && (OPT1_time == 0.0) && (OPT1_decimal[variable] < OPT1_min - UNDER) && (OPT1_decimal[variable + 1] < OPT1_min - UNDER) && (OPT1_decimal[variable + 2] < OPT1_min - UNDER) && (OPT1_decimal[variable] != 0)) {
      OPT1_time = variable;
      OPT1_results[trial_number] = float((OPT1_time) / 10 - read_time);
    }

    if ((UNDER >= 0) && (OPT2_time == 0.0) && (OPT2_decimal[variable] < OPT2_min - UNDER) && (OPT2_decimal[variable + 1] < OPT2_min - UNDER) && (OPT2_decimal[variable + 2] < OPT2_min - UNDER) && (OPT2_decimal[variable] != 0)) {
      OPT2_time = variable;
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
  
  for (int variable = 0; variable < TRIAL_MAX; variable ++){
if (OPT1_results[variable] > OPT1_fastest){OPT1_fastest = OPT1_results[variable];}
      if ((OPT1_results[variable] != 0) && (OPT1_results[variable] < OPT1_slowest)){OPT1_slowest = OPT1_results[variable];}
      if (OPT2_results[variable] > OPT2_fastest){OPT2_fastest = OPT2_results[variable];}
      if ((OPT2_results[variable] != 0) && (OPT2_results[variable] < OPT2_slowest)){OPT2_slowest = OPT2_results[variable];}
  }


  for (int variable = 0; variable < 1000; variable++) {
      
    
    if (OPT1_results[variable] != 0) {
      OPT1_sum = OPT1_sum + OPT1_results[variable];
      OPT1_count++;

    }
    if (OPT2_results[variable] != 0) {
      OPT2_sum = OPT2_sum + OPT2_results[variable];
      OPT2_count++;
    }}

    if (fastest_slowest_mode == 0){
    if (OPT1_count != 0) { OPT1_avg = (float(OPT1_sum) / float(OPT1_count)); }
    if (OPT2_count != 0) { OPT2_avg = (float(OPT2_sum) / float(OPT2_count)); }
    }
    if (fastest_slowest_mode == 1){
    if (OPT1_count != 0) { OPT1_avg = ((OPT1_fastest+OPT1_slowest)/2); }
    if (OPT2_count != 0) { OPT2_avg = ((OPT2_fastest+OPT2_slowest)/2); }
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
    results = results + OPT1_top_b;
    results = results + ",";
    results = results + OPT1_top_b;
    results = results + ",";
  } else {
    results = results + OPT1_top_a;
    results = results + ",";
    results = results + OPT2_top_a;
    results = results + ",";
  }

  results = results + OPT1_avg;
  results = results + ",";
  results = results + OPT2_avg;
  results = results + ",";

  results = results + OPT1_count;
  results = results + ",";
  results = results + OPT2_count;
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
  results = results + OPT1_top_a;
  results = results + ",";
  results = results + OPT1_bottom_a;
  results = results + ",";
  results = results + OPT2_top_a;
  results = results + ",";
  results = results + OPT2_bottom_a;
  results = results + ",";

  results = results + screen_hz_calculated;
  results = results + ",";
  results = results + screen_draw_time_calculated;
  results = results + ",";
  results = results + OPT1_top_b;
  results = results + ",";
  results = results + OPT1_bottom_b;
  results = results + ",";

  results = results + P1_wins;
  results = results + ",";
  results = results + P2_wins;
  results = results + ",";
  results = results + P1_P2_draw;
  results = results + ",";
}



void reset_all_values() {
  trial_number = 0;
  for (int variable = 0; variable < 10000; variable++) {
    OPT1_decimal[variable] = 0;
    OPT2_decimal[variable] = 0;
  }
  for (int variable = 0; variable < 1000; variable++) {
    OPT1_results[variable] = 0;
    OPT2_results[variable] = 0;
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

void setup() {

  for (int variable = 0; variable < 14; variable++) {
    pinMode(OUTPUT_BUTTONS[variable], OUTPUT);
    digitalWrite(OUTPUT_BUTTONS[variable], HIGH);
  }

  Serial.begin(115200);
  delay(1000);

  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(CONFIRM, INPUT_PULLUP);
  pinMode(CANCEL, INPUT_PULLUP);

  pinMode(OPT1, INPUT);  /////
  pinMode(OPT2, INPUT);  /////
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(A3, INPUT);
  LittleFS.begin();

  if (digitalRead(CANCEL) == 0) { save_settings(); }  // by doing this we can reset to default
  
  load_settings();
  if (digitalRead(LEFT) == 0) {initial_cursor_position = -4;save_settings();}//why did I add this?
  
  if (digitalRead(RIGHT)==0){
    OUTPUT_ON_STATE[0] = 1; OUTPUT_ON_STATE[1] = 1; OUTPUT_ON_STATE[2] = 1;
output_off_state[0]=0; output_off_state[1]=0; output_off_state[2]=0;
output_reset_state[0]=1; output_reset_state[1]=1; output_reset_state[2]=1;

    }
// OUTPUT_ON_STATE = {1,1,1,0, 0,0,0,0, 0,0,0,0, 1,1};
// output_off_state = {0,0,0,1, 1,1,1,1, 1,1,1,1, 0,0};
// output_reset_state = {0,0,0,1, 1,1,1,1, 1,1,1,1, 1,1};
//     int OUTPUT_ON_STATE[]    = {0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1};
// int output_off_state[]   = {1,1,1,1, 1,1,1,1, 1,1,1,1, 0,0};
// int output_reset_state[] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1};
  
  
  cursor_position = initial_cursor_position;

  Serial.print(length_array);

  SerialBT.setName("EVO-TESTER3  00:00:00:00:00:00");
  SerialBT.setName("noodalls  00:00:00:00:00:00");
  SerialBT.begin();
  //SerialBT.setTimeout(10000);




  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(2);
  trial_number = 0;


  for (int variable = 0; variable < 10000; variable++) {
    OPT1_decimal[variable] = 0;
    OPT2_decimal[variable] = 0;
  }
  for (int variable = 0; variable < 255; variable++) {
    OPT1_summary[variable] = 0;
    OPT2_summary[variable] = 0;
  }
}


void loop() {

  time1 = micros() / 100;
  if ((time1) != (time2)) {

    OPT1_current = analogRead(OPT1) / 4;
    OPT2_current = analogRead(OPT2) / 4;
    elapsed_time = (time1 - time3);
    if (OPT1_summary[OPT1_current] == 0) { OPT1_summary[OPT1_current] = elapsed_time; }
    if (OPT2_summary[OPT2_current] == 0) { OPT2_summary[OPT2_current] = elapsed_time; }
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

      OPT1_decimal[elapsed_time] = OPT1_current;
      OPT2_decimal[elapsed_time] = OPT2_current;

      for (int variable = 0; variable < 14; variable++) {
        if (elapsed_time_ms == OUTPUT_ON_TIME[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }    //buttons on
        if (elapsed_time_ms == output_off_time[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], output_off_state[variable]); }  //buttons_off
        //for Brook pins, HIGH is unpressed, LOW is pressed
      }


      if (elapsed_time_ms < read_time) {
        if (OPT1_current > OPT1_max) { OPT1_max = OPT1_current; }
        if (OPT1_current < OPT1_min) { OPT1_min = OPT1_current; }
        if (OPT2_current > OPT2_max) { OPT2_max = OPT2_current; }
        if (OPT2_current < OPT2_min) { OPT2_min = OPT2_current; }
      }

      if (elapsed_time_ms == read_time) {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
      }

      if (elapsed_time_ms > query_time) {
        if ((cursor_position >= 1) && (cursor_position <= 14)) {
          if ((over >= 0) && (OPT1_current > (OPT1_max + over))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
          }

          if ((over >= 0) && (OPT2_current > (OPT2_max + over))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
          }

          if ((UNDER >= 0) && (OPT1_current < (OPT1_min - UNDER))) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, HIGH);
          }

          if ((UNDER >= 0) && (OPT2_current < (OPT2_min - UNDER))) {
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
          save_summary();
        }
        if (cursor_position == 5) {//PT comp
          if ((OPT1_results[trial_number] != 0) && (OPT2_results[trial_number] != 0)) {
            if ((OPT2_results[trial_number] - OPT1_results[trial_number]) > PLAY1P2P) {
              digitalWrite(GREEN_LED, LOW);
              digitalWrite(BLUE_LED, HIGH);
              digitalWrite(RED_LED, LOW);
            }
            if ((OPT1_results[trial_number] - OPT2_results[trial_number]) > PLAY1P2P) {
              digitalWrite(GREEN_LED, LOW);
              digitalWrite(BLUE_LED, LOW);
              digitalWrite(RED_LED, HIGH);
            }
          }
        }

        display_screen();
        trial_number = trial_number + 1;

        if (!SerialBT.available())
          ;
        {
          char abc = SerialBT.read();
          if (abc == '|') {
            mode = 0;
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            pinMode(LEFT, INPUT_PULLUP);
            pinMode(RIGHT, INPUT_PULLUP);
            pinMode(CONFIRM, INPUT_PULLUP);
          }
        }

        if (!Serial.available())
          ;
        {
          char abc = Serial.read();
          if (abc == '|') {
            mode = 0;
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(BLUE_LED, LOW);
            pinMode(LEFT, INPUT_PULLUP);
            pinMode(RIGHT, INPUT_PULLUP);
            pinMode(CONFIRM, INPUT_PULLUP);
          }
        }
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
          mode = 0;
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
            writesummary.print(OPT1_results[variable]);
            writesummary.print(",");
          }
          writesummary.println();
          writesummary.print("V");
          writesummary.print(",");
          for (int variable = 0; variable < TRIAL_MAX; variable++) {
            writesummary.print(OPT2_results[variable]);
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
          mode = 0;
        // }
      }
    }

    if (mode == -1) {
      //trial_number = min (trial_number, TRIAL_MAX -1);
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
      if (digitalRead(LEFT) == 0) { current_button = 4; }
      if (digitalRead(RIGHT) == 0) { current_button = 6; }
      if (digitalRead(CONFIRM) == 0) { current_button = 5; }
      if (digitalRead(CANCEL) == 0) { current_button = 1; }
      if ((digitalRead(CONFIRM) == 0) && (digitalRead(RIGHT) == 0)) { current_button = 10; }

      if ((current_button == 6) && (last_button == 4)) {
        initial_cursor_position = cursor_position;
        save_settings();
      }

      // turn_off_buttons();
  if (cursor_position < 1) {
    for (int variable = 0; variable < 14; variable++) {
      digitalWrite(OUTPUT_BUTTONS[variable], output_reset_state[variable]);
    }}


  if (cursor_position > 0) {

    for (int variable = 0; variable < 14; variable++) {
      if (output_off_time[variable] >= 0) { digitalWrite(OUTPUT_BUTTONS[variable], output_off_state[variable]); }
      if (output_off_time[variable] < 0) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }
      if (output_off_time[variable] < OUTPUT_ON_TIME[variable]) { digitalWrite(OUTPUT_BUTTONS[variable], OUTPUT_ON_STATE[variable]); }
    }}




      if (!SerialBT.available())
        ;
      {
        char abc = SerialBT.read();
        if (abc == 'y') {
          cursor_position = SerialBT.parseInt();
          SerialBT.print("y");
          SerialBT.println(cursor_position);
        }
        if (abc == 'Y') {
          cursor_position = SerialBT.parseInt();
          current_button = 5;
          SerialBT.print("Y");
          SerialBT.println(cursor_position);
        }
        if (abc == '<') { current_button = 4; }
        if (abc == '>') { current_button = 6; }
        if (abc == '/') { current_button = 5; }
        if (abc == '|') { current_button = 1; }

        if (abc == '?') {
          for (int variable = 0; variable < length_array; variable++) {
            SerialBT.print(serial_names[variable]);
            SerialBT.println(*pointers[variable]);
          }

          for (int variable = 0; variable < float_length_array; variable++) {
            SerialBT.print(float_serial_names[variable]);
            SerialBT.println(*float_pointers[variable]);
          }
        }




        for (int variable = 0; variable < 14; variable++) {
          if (abc == serial_names[variable]) {
            *pointers[variable] = SerialBT.parseInt();
            SerialBT.print(serial_names[variable]);
            SerialBT.println(*pointers[variable]);
            if (*pointers[variable] == 0) {
              *pointers[variable + 14] = -5;
              SerialBT.print(serial_names[variable + 14]);
              SerialBT.println(*pointers[variable + 14]);
            }
            save_settings();
          }
        }

        for (int variable = 14; variable < 28; variable++) {
          if (abc == serial_names[variable]) {
            *pointers[variable] = SerialBT.parseInt();
            SerialBT.print(serial_names[variable]);
            SerialBT.println(*pointers[variable]);
            if (*pointers[variable] == 0) {
              *pointers[variable - 14] = -5;
              SerialBT.print(serial_names[variable - 14]);
              SerialBT.println(*pointers[variable - 14]);
            }
            save_settings();
          }
        }

        for (int variable = 28; variable < length_array; variable++) {  /////check this length is right
          if (abc == serial_names[variable]) {
            *pointers[variable] = SerialBT.parseInt();
            SerialBT.print(serial_names[variable]);
            SerialBT.println(*pointers[variable]);

            save_settings();
          }
        }

        for (int variable = 0; variable < float_length_array; variable++) {  /////check this length is right
          if (abc == float_serial_names[variable]) {
            *float_pointers[variable] = SerialBT.parseFloat();
            if (upper < 0) { upper = 100 * (abs(upper) - top_of_screen) / (bottom_of_screen - top_of_screen); }
            if (lower < 0) { lower = 100 * (abs(lower) - top_of_screen) / (bottom_of_screen - top_of_screen); }
            SerialBT.print(float_serial_names[variable]);
            SerialBT.println(*float_pointers[variable]);
            save_settings();
          }
        }
      }


      if (!Serial.available())
        ;
      {
        char abc = Serial.read();
        if (abc == 'y') {
          cursor_position = Serial.parseInt();
          Serial.print("y");
          Serial.println(cursor_position);
        }
        if (abc == 'Y') {
          cursor_position = Serial.parseInt();
          current_button = 5;
          Serial.print("Y");
          Serial.println(cursor_position);
        }
        if (abc == '<') { current_button = 4; }
        if (abc == '>') { current_button = 6; }
        if (abc == '/') { current_button = 5; }
        if (abc == '|') { current_button = 1; }

        if (abc == '?') {
          for (int variable = 0; variable < length_array; variable++) {
            Serial.print(serial_names[variable]);
            Serial.println(*pointers[variable]);
          }

          for (int variable = 0; variable < float_length_array; variable++) {
            Serial.print(float_serial_names[variable]);
            Serial.println(*float_pointers[variable]);
          }
        }





        for (int variable = 0; variable < 14; variable++) {
          if (abc == serial_names[variable]) {
            *pointers[variable] = Serial.parseInt();
            Serial.print(serial_names[variable]);
            Serial.println(*pointers[variable]);
            if (*pointers[variable] == 0) {
              *pointers[variable + 14] = -5;
              Serial.print(serial_names[variable + 14]);
              Serial.println(*pointers[variable + 14]);
            }
            save_settings();
          }
        }

        for (int variable = 14; variable < 28; variable++) {
          if (abc == serial_names[variable]) {
            *pointers[variable] = Serial.parseInt();
            Serial.print(serial_names[variable]);
            Serial.println(*pointers[variable]);
            if (*pointers[variable] == 0) {
              *pointers[variable - 14] = -5;
              Serial.print(serial_names[variable - 14]);
              Serial.println(*pointers[variable - 14]);
            }
            save_settings();
          }
        }




        for (int variable = 28; variable < length_array; variable++) {  /////check this length is right
          if (abc == serial_names[variable]) {
            *pointers[variable] = Serial.parseInt();
            Serial.print(serial_names[variable]);
            Serial.println(*pointers[variable]);
            save_settings();
          }
        }

        for (int variable = 0; variable < float_length_array; variable++) {  /////check this length is right
          if (abc == float_serial_names[variable]) {
            *float_pointers[variable] = Serial.parseFloat();
            if (upper < 0) { upper = 100 * (abs(upper) - top_of_screen) / (bottom_of_screen - top_of_screen); }
            if (lower < 0) { lower = 100 * (abs(lower) - top_of_screen) / (bottom_of_screen - top_of_screen); }
            Serial.print(float_serial_names[variable]);
            Serial.println(*float_pointers[variable]);
            save_settings();
          }
        }
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
      if ((current_button == 5) &&(last_button ==0) && (cursor_position == 5)){calculate_mode = 2;}

      if ((current_button == 5) && (last_button == 0) && (cursor_position > 0)) {
        reset_OPT_values();
        pinMode(RED_LED, OUTPUT);
        pinMode(GREEN_LED, OUTPUT);
        pinMode(BLUE_LED, OUTPUT);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        time3 = 1 + micros() / 100;
        if ((cursor_position >=1) && (cursor_position <=8) && (trial_number < TRIAL_MAX)){prepare_summary();}
        if (cursor_position == 9){trial_number = TRIAL_MAX+1;}
        mode = 1;
      }


      if ((current_button == 5) && (last_button == 0) && (cursor_position == -1)){mode = -1;last_button = 5;}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -11)) { mode = -11; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -12)) { mode = -12; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -13)) { mode = -13; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -2)) { mode = -2; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -3)) { mode = -3; }
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -4)) {mode = -4;last_button = 5;}
      if ((current_button == 5) && (last_button == 0) && (cursor_position == -6)) { mode = -6; }
    }

    last_button = current_button;
    time2 = micros() / 100;
  }
}
