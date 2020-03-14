#include <Servo.h>

/*    Config    */
#define ENABLE 1
#define DISABLE 0

#define DEBUG DISABLE
/*    ******    */

/*    Include   */
#include <Servo.h>
#include <MsTimer2.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/*    *******   */

/*    Define    */
#define DHT_Type DHT11
#define DHT_Pin 42

#define LED_Pin 2
#define Peltier_Pin 3

#define Servo_Pin 11

#define RST_Pin 49
#define SS_Pin  53

#define SEC 5         // Timer Interrupt 주기
/*    ******    */

/*    Function    */
void GPIO_Init();
void LCD_Init();
void Door_Init();
void GPS_Init();
void RFID_Init();
void Timer_Init();

void ISR_Func();
void Update_Monitor();
void Input_Keypad();
void State();
void Clear_Monitor(int row);
void Actuating();
/*    ********    */

/*    Variable    */
byte ondo[] = {
  B11000,
  B11000,
  B00000,
  B00110,
  B01001,
  B01000,
  B01001,
  B00110
};

typedef struct {
  volatile bool change;
  volatile bool set;
  volatile bool correct;
  volatile bool door_open;
  volatile bool reset;
  volatile bool cooling;
  volatile bool input;
} Flag;

// DHT
volatile float temperature = 0.0;                    // DTH11 센서 온도 값
volatile float humidity = 0.0;                     // DTH11 센서 습도 값
volatile float setTemp = 30.0;                     // 로터리 엔코더로 온도 설정할 값
String setTemp_buf = "";
volatile int counting = 0;                         // 통신을 위한 counting 값
volatile float prev_hum = 50;
volatile float prev_temp = 25.0;

// LCD
String SetString = "Set Temp:";
String Thermal = "Temp:";
String Humidity = "Humidity:";

// Keypad
int pos = 0;
int count = 0;
int wrong = 0;
int index = 0;
int set_cnt = 0;

// GPS
volatile char c = "";                  // Wn 인지 구분 및 str에 저장.
volatile String str = "";              // \n 전까지 c 값을 저장.
String targetStr = "GPGGA";   // str의 값이 NMEA의 GPGGA 값인지 타겟
volatile double LatF;
volatile float LongF;
/*    ********    */

/*    Initialize    */
Servo Doorlock;
LiquidCrystal_I2C Monitor(0x27, 20, 4);

MFRC522 rfid(SS_Pin, RST_Pin);

DHT dht(DHT_Pin, DHT_Type);

String Password = "1234";
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {32, 33, 34, 35}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {  36, 37, 38}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Flag flag;
/*    **********    */



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  GPIO_Init();
  LCD_Init();
  Door_Init();
  GPS_Init();
  RFID_Init();
  Timer_Init();

}

void loop() {
  // put your main code here, to run repeatedly:
  Input_Keypad();

  State();

  Update_Monitor();

  Actuating();
}

void GPIO_Init() {
  pinMode(LED_Pin, OUTPUT);
  pinMode(Peltier_Pin, OUTPUT);
}

void LCD_Init() {
  Monitor.begin();
  Monitor.backlight();
  Monitor.createChar(0x01, ondo);

  Monitor.clear();
  Monitor.setCursor(0, 0);
  Monitor.print("Operating..");
  Monitor.setCursor(0, 1);
  Monitor.print("Smart Container");
}

void Door_Init() {
  Doorlock.attach(Servo_Pin, 600, 2400);
}

void GPS_Init() {
  Serial1.begin(9600);
}

void RFID_Init() {
  SPI.begin();
  rfid.PCD_Init();
}

void Timer_Init() {
  MsTimer2::set(SEC * 1000, ISR_Func);
  MsTimer2::start();
}
void ISR_Func() {
  counting++;

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity)) {
    humidity = prev_hum;
  }
  if (isnan(temperature)) {
    temperature = prev_temp;
  }
  if (counting == 2) {
    flag.change = true;

    String t;
    String h;
    String sT;

    t = (String)temperature;
    h = (String)humidity;
    sT = (String)setTemp;

    if (flag.door_open && flag.cooling) {
      Serial.print(t);
      Serial.print("|");
      Serial.print(sT);
      Serial.print("|");
      Serial.print("ON");
      Serial.print("|");
      Serial.print(h);
      Serial.print("|");
      Serial.print("UNLOCK");
      Serial.print("|");
      Serial.print(LongF, 6);
      Serial.print("|");
      Serial.println(LatF, 6);
    }
    else if (flag.door_open && !flag.cooling) {

      Serial.print(t);
      Serial.print("|");
      Serial.print(sT);
      Serial.print("|");
      Serial.print("OFF");
      Serial.print("|");
      Serial.print(h);
      Serial.print("|");
      Serial.print("UNLOCK");
      Serial.print("|");
      Serial.print(LongF, 6);
      Serial.print("|");
      Serial.println(LatF, 6);

    }
    else if (!flag.door_open && flag.cooling) {
      Serial.print(t);
      Serial.print("|");
      Serial.print(sT);
      Serial.print("|");
      Serial.print("ON");
      Serial.print("|");
      Serial.print(h);
      Serial.print("|");
      Serial.print("LOCK");
      Serial.print("|");
      Serial.print(LongF, 6);
      Serial.print("|");
      Serial.println(LatF, 6);

    }
    else if (!flag.door_open && !flag.cooling) {
      Serial.print(t);
      Serial.print("|");
      Serial.print(sT);
      Serial.print("|");
      Serial.print("OFF");
      Serial.print("|");
      Serial.print(h);
      Serial.print("|");
      Serial.print("LOCK");
      Serial.print("|");
      Serial.print(LongF, 6);
      Serial.print("|");
      Serial.println(LatF, 6);
    }



    /* Communication code 추가*/
    counting = 0;
  }

  if (temperature > setTemp)
    flag.cooling = true;
  else
    flag.cooling = false;
}

void Update_Monitor() {
  if (flag.change) {
    Clear_Monitor(0);
    Monitor.setCursor(0, 0);
    Monitor.print(Thermal + String(temperature));
    Monitor.write(0x01);

    Clear_Monitor(1);
    Monitor.setCursor(0, 1);
    Monitor.print(Humidity + String(humidity));
    Monitor.print("%");

    if (flag.set) {
      Clear_Monitor(2);
      Monitor.setCursor(0, 2);
      Monitor.print(SetString + String(setTemp));
      Monitor.write(0x01);
    }
    else {
      if (flag.input) {
        String temp_str = "Password: ";
        for (int i = 0; i < index; i++) {
          temp_str += "*";
        }
        Clear_Monitor(2);
        Monitor.setCursor(0, 2);
        Monitor.print(temp_str);

        if (index >= 4) {
          index = 0;
          if (flag.correct) {
            Clear_Monitor(3);
            Monitor.setCursor(0, 3);
            Monitor.print("Authoried access");
          }
          else {
            Clear_Monitor(3);
            Monitor.setCursor(0, 3);
            Monitor.print("Access denied");
          }
          delay(2000);
          flag.input = false;
        }

      }
      else {
        Clear_Monitor(2);
        if (flag.reset) {
          Clear_Monitor(3);
          Monitor.setCursor(0, 3);
          Monitor.print("====== Reset ======");
          flag.reset = false;
        }
        else {
          Clear_Monitor(3);
          Monitor.setCursor(0, 3);
          Monitor.print("Smart Container");
        }
      }
    }

    flag.change = false;
  }
}

void Input_Keypad() {
  char key = keypad.getKey();

#if (DEBUG == ENABLE)
  if (Serial.available()) {
    key = Serial.read();
    Serial.println(key);
  }
#endif
  if (key) {
    flag.change = true;
    flag.input = true;
    if (key == '0') {
      set_cnt++;
    }

    if (flag.set) {
      if (key == '*') {
        setTemp = setTemp_buf.toFloat();
        setTemp_buf = "";
        flag.set = false;
      }
      else
        setTemp_buf += key;
    }

    else {
      if (key == '#') {
        flag.correct = false;
        flag.input = false;
        flag.door_open = false;
        flag.reset = true;
        index = 0;
        count = 0;

        Clear_Monitor(2);
      }
      else if (key == '*') {
        if (!rfid.PICC_IsNewCardPresent()) {
          return;
        }
        if (! rfid.PICC_ReadCardSerial()) {
          return;
        }

        String content = "";

        for (byte i = 0; i < rfid.uid.size; i++) {
          content.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : " "));
          content.concat(String(rfid.uid.uidByte[i], HEX));
        }
        content.toUpperCase();

        if (content.substring(1) == "76 F5 3B F8" || content.substring(1) == "F902 B9 55") {
          flag.door_open = true;
          flag.correct = true;
#if (DEBEG == ENABLE)
          //          if (content.substring(1) == "76 F5 3B F8") {
          //            Serial.println("76 F5 3B F8");
          //          }
          //          else if (content.substring(1) == "F902 B9 55") {
          //            Serial.println("F902 B9 55");
          //          }
#endif
        }
      }
      else if (!flag.door_open) {
        if (key == Password[index]) {
          count++;
          Serial.print("count: ");
          Serial.println(count);
        }
        else {

        }
        index++;
      }

    }
  }
}


void State() {
  if (set_cnt == 4) {
    set_cnt = 0;
    index = 0;
    flag.set = true;
    flag.change = true;
  }

  if (index >= 4) {
    flag.change = true;
    if (count >= 4) {
      flag.correct = true;
      flag.door_open = true;
    }
    else {
      flag.correct = false;
      flag.door_open = false;
    }
    count = 0;
    set_cnt = 0;
  }

}

void Clear_Monitor(int row) {
  Monitor.setCursor(0, row);
  Monitor.print("                    ");
}

void Actuating() {
  if (flag.door_open) {
    Doorlock.write(0);
    digitalWrite(LED_Pin, HIGH);
  }
  else {
    Doorlock.write(90);
    digitalWrite(LED_Pin, LOW);
  }

  if (flag.cooling)
    digitalWrite(Peltier_Pin, HIGH);
  else
    digitalWrite(Peltier_Pin, LOW);
}

void serialEvent1() {
  c = Serial1.read(); // 센서의 값 읽기
  if (c == '\n') { // \n 값인지 구분.
    if (targetStr.equals(str.substring(1, 6))) {
      // NMEA 의 GPGGA 값일시

      // , 를 토큰으로서 파싱.
      int first = str.indexOf(",");
      int two = str.indexOf(",", first + 1);
      int three = str.indexOf(",", two + 1);
      int four = str.indexOf(",", three + 1);
      int five = str.indexOf(",", four + 1);
      // Lat과 Long 위치에 있는 값들을 index로 추출
      String Lat = str.substring(two + 1, three);
      String Long = str.substring(four + 1, five);
      // Lat의 앞값과 뒷값을 구분
      String Lat1 = Lat.substring(0, 2);
      String Lat2 = Lat.substring(2);
      // Long의 앞값과 뒷값을 구분
      String Long1 = Long.substring(0, 3);
      String Long2 = Long.substring(3);
      // 좌표 계산.
      LatF = Lat1.toDouble() + Lat2.toDouble() / 60;
      LongF = Long1.toFloat() + Long2.toFloat() / 60;
    }
    // str 값 초기화
    str = "";
  }
  else { // \n 아닐시, str에 문자를 계속 더하기
    str += c;
  }
}
