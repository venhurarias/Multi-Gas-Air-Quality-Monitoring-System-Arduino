
// https://davidhoulding.blogspot.com/2014/03/
// https://diyprojectslab.com/gas-sensor-with-arduino/
#include <Streaming.h>
#include <DHT11.h>
#include <MQUnifiedsensor.h>
#include <Chrono.h>
#include <MQ137.h>
#include <LCDWIKI_GUI.h>  //Core graphics library
#include <LCDWIKI_SPI.h>  //Hardware-specific library
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include "Plantower_PMS7003.h"

#define Board "Arduino Mega"
#define MQ2_PIN A1
#define MQ3_PIN A2
#define MQ4_PIN A6
#define MQ5_PIN A7
#define MQ135_PIN A8
#define MQ6_PIN A9
#define MQ137_PIN A10
#define MQ8_PIN A11
#define MQ9_PIN A12
#define MQ7_PIN A13

#define A_PIN A15
#define B_PIN A14
#define DHT_PIN 2
#define FLOAT_PIN 3
#define MISTING_PIN 22
#define CS_PIN 44
#define PIR_PIN 8
#define VOLT_7_PIN 6
#define VOLT_9_PIN 7

#define MODEL ILI9488_18
#define CS A5
#define CD A3
#define RST A4
#define LED A0

#define ADC_Bit_Resolution 10  // 10 bit ADC
#define Voltage_Resolution 5   // Volt resolution to calc the voltage
#define Type "Arduino Mega 2560"

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFC00


#define RatioMQ2CleanAir 9.83   //RS / R0 = 9.83 ppm
#define RatioMQ3CleanAir 60     //RS / R0 = 60 ppm
#define RatioMQ4CleanAir 4.4    //RS / R0 = 4.4 ppm
#define RatioMQ5CleanAir 6.5    //RS / R0 = 6.5 ppm
#define RatioMQ6CleanAir 10     //RS / R0 = 10 ppm
#define RatioMQ7CleanAir 27.5   //RS / R0 = 27.5 ppm
#define RatioMQ8CleanAir 70     //RS / R0 = 70 ppm
#define RatioMQ9CleanAir 9.6    //RS / R0 = 9.6 ppm
#define RatioMQ135CleanAir 3.6  //RS / R0 = 3.6 ppm

DHT11 dht11(DHT_PIN);

MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, Type);
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ3_PIN, Type);
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ4_PIN, Type);
MQUnifiedsensor MQ5(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ5_PIN, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ135_PIN, Type);
MQUnifiedsensor MQ6(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ6_PIN, Type);
MQ137 MQ137(MQ137_PIN, 30.31f, true);
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ8_PIN, Type);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ9_PIN, Type);
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ7_PIN, Type);

LCDWIKI_SPI mylcd(MODEL, CS, CD, RST, LED);
RTC_DS3231 rtc;

Chrono myChrono, chronoDisplay, chronoVoltage, chronoSD;
int pm10, pm25, pm100;

int temperature = 0;
int humidity = 0;

float MQ2Lecture;
float MQ3Lecture;
float MQ4Lecture;
float MQ5Lecture;
float MQ6Lecture;
float MQ7Lecture;
float MQ8Lecture;
float MQ9Lecture;
float MQ135Lecture;
float MQ137Lecture;

DateTime now;
int displayMode;

File myFile;

int prevHighValue;

const int maxSize = 12;
int myWarningStr[maxSize];
float myWarningVal[maxSize];
int arraySize = 0;
bool mistEmpty;

bool in5V;
bool isSMSSend;

char output[256];
Plantower_PMS7003 pms7003 = Plantower_PMS7003();

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);


  // Serial << ("Hi") << endl;
  pinMode(FLOAT_PIN, INPUT_PULLUP);
  pinMode(MISTING_PIN, OUTPUT), mistingOff();
  pinMode(A_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(VOLT_7_PIN, OUTPUT);
  pinMode(VOLT_9_PIN, OUTPUT);


  Serial3.begin(9600);  //PMS
  pms7003.init(&Serial3);

  Serial1.begin(9600);  //GSM
  MQ7_5V();
  MQ9_5V();

  MQ2.init();
  MQ2.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ2.setA(574.25);
  MQ2.setB(-2.222);  // Configure the equation to to calculate LPG concentration
                     // MQ2.setR0(9.659574468);
                     //  MQ2.setR0(7.71);
  MQ2.setR0(2.57);

  MQ3.init();
  MQ3.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ3.setA(7585.3);
  MQ3.setB(-2.849);  //Configure the equation to calculate Alcohol concentration value
                     // MQ3.setR0(3.86018237);
                     //  MQ3.setR0(0.83);
  MQ3.setR0(0.41);

  MQ4.init();
  MQ4.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ4.setA(1012.7);
  MQ4.setB(-2.786);  // Configure the equation to to calculate CH4 concentration
                     // MQ4.setR0(3.86018237);
                     //  MQ4.setR0(12.76);
  MQ4.setR0(5.06);

  MQ5.init();
  MQ5.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ5.setA(97124);
  MQ5.setB(-4.918);  //Configure the equation to calculate Alcohol concentration value
                     // MQ5.setR0(71.100304);
  //  MQ5.setR0(24.50);
  MQ5.setR0(5.35);

  MQ135.init();
  MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ135.setA(102.2);
  MQ135.setB(-2.473);
  // MQ135.setR0(9.42857143);
  //  MQ135.setR0(16.29);
  MQ135.setR0(4.80);

  MQ6.init();
  MQ6.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ6.setA(2127.2);
  MQ6.setB(-2.526);  // Configure the equation to to calculate CH4 concentration
                     // MQ6.setR0(13.4285714);
  //  MQ6.setR0(6.74);
  MQ6.setR0(2.45);

  MQ8.init();
  MQ8.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ8.setA(976.97);
  MQ8.setB(-0.688);  // Configure the equation to to calculate H2 concentration
                     // MQ8.setR0(1);
                     //  MQ8.setR0(0.49);
  MQ8.setR0(0.26);

  MQ9.init();
  MQ9.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ9.setA(1000.5);
  MQ9.setB(-2.186);  // Configure the equation to to calculate LPG concentration
                     // MQ9.setR0(9.42857143);
                     //  MQ9.setR0(71);
  MQ9.setR0(100.08);


  MQ7.init();
  MQ7.setRegressionMethod(1);  //_PPM =  a*ratio^b
  MQ7.setA(99.042);
  MQ7.setB(-1.518);  // Configure the equation to calculate CO concentration value
                     // MQ7.setR0(4);
                     //  MQ7.setR0(1.44);
  MQ7.setR0(2.29);

  MQ137.begin();
  //   delay(60000);
  //   //Calibration
  //   Serial << "Calibrating please wait.";
  //   delay(900000L);
  //   float MQ2calcR0 = 0,
  //         MQ3calcR0 = 0,
  //         MQ4calcR0 = 0,
  //         MQ5calcR0 = 0,
  //         MQ6calcR0 = 0,
  //         MQ7calcR0 = 0,
  //         MQ8calcR0 = 0,
  //         MQ9calcR0 = 0,
  //         MQ135calcR0 = 0;
  //
  //
  //   for (int i = 1; i <= 10; i++) {
  //     MQ2.update();
  //     MQ3.update();
  //     MQ4.update();
  //     MQ5.update();
  //     MQ6.update();
  //     MQ7.update();
  //     MQ8.update();
  //     MQ9.update();
  //     MQ135.update();
  //
  //     MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
  //     MQ3calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
  //     MQ4calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
  //     MQ5calcR0 += MQ5.calibrate(RatioMQ5CleanAir);
  //     MQ6calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
  //     MQ7calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
  //     MQ8calcR0 += MQ8.calibrate(RatioMQ8CleanAir);
  //     MQ9calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
  //     MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  //
  //     Serial << ".";
  //   }
  //
  //   Serial << "MQ2 : " << MQ2calcR0 / 10 << endl;
  //   Serial << "MQ3 : " << MQ3calcR0 / 10 << endl;
  //   Serial << "MQ4 : " << MQ4calcR0 / 10 << endl;
  //   Serial << "MQ5 : " << MQ5calcR0 / 10 << endl;
  //   Serial << "MQ6 : " << MQ6calcR0 / 10 << endl;
  //   Serial << "MQ7 : " << MQ7calcR0 / 10 << endl;
  //   Serial << "MQ8 : " << MQ8calcR0 / 10 << endl;
  //   Serial << "MQ9 : " << MQ9calcR0 / 10 << endl;
  //   Serial << "MQ135 : " << MQ135calcR0 / 10 << endl;
  //
  //   Serial << "  done!." << endl;
  //    if (isinf(MQ2calcR0) || isinf(MQ3calcR0) || isinf(MQ4calcR0) || isinf(MQ5calcR0) || isinf(MQ6calcR0) || isinf(MQ7calcR0) || isinf(MQ8calcR0) || isinf(MQ9calcR0) || isinf(MQ135calcR0)) {
  //       Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
  //       while (1)
  //         ;
  //     }
  //     if (MQ2calcR0 == 0 || MQ3calcR0 == 0 || MQ4calcR0 == 0 || MQ5calcR0 == 0 || MQ6calcR0 == 0 || MQ7calcR0 == 0 || MQ8calcR0 == 0 || MQ9calcR0 == 0 || MQ135calcR0 == 0) {
  //       Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
  //       while (1)
  //         ;
  //     }


  mylcd.Init_LCD();
  mylcd.Fill_Screen(BLACK);
  mylcd.Set_Rotation(1);

  if (!rtc.begin()) {
    Serial.println("Error in RTC");

    while (true) {
    }
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  ppmDisplayLabel("MQ-2", "MQ-3", "MQ-4", "MQ-5", true, true);
  chronoVoltage.restart();
  in5V = true;

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
}




void loop() {
  if (in5V) {
    if (chronoVoltage.hasPassed(60000)) {
      chronoVoltage.restart();
      MQ9_15V();
      MQ7_14V();
      in5V = false;
      delay(100);
      MQ7.update();

      if (!isinf(MQ7.readSensor())) {
        MQ7Lecture = MQ7.readSensor();
      }
    }
  } else {
    if (chronoVoltage.hasPassed(90000)) {
      chronoVoltage.restart();
      MQ7_5V();
      MQ9_5V();
      delay(100);
      MQ9.update();
      if (!isinf(MQ9.readSensor())) {
        MQ9Lecture = MQ9.readSensor();
      }
      in5V = true;
    }
  }


  //  Serial << digitalRead(PIR_PIN) << endl;
  // testing();
  normalProcess();
  // test();


  //  Serial<<"isMistEmpty :: "<<isMistEmpty()<<endl;
  //  Serial << " TEMP :: " << temperature << "\t\thumidity :: " << humidity << endl;
  //  Serial<<"VOC :: "<<getVOCValue()<<endl;


  // displayTest();
  // while (Serial1.available()) {
  //   Serial.write(Serial1.read());
  // }
  // if (Serial1.available()) {
  //   Serial.write(Serial1.read());
  // }
  // if (Serial.available()) {
  //   Serial1.write(Serial.read());
  // }
}

bool isWithPeople() {
  return digitalRead(PIR_PIN);
}

void MQ7_5V() {
  analogWrite(VOLT_7_PIN, 255);
}

void MQ7_14V() {
  analogWrite(VOLT_7_PIN, 71);
}

void MQ9_5V() {
  analogWrite(VOLT_9_PIN, 255);
}

void MQ9_15V() {
  analogWrite(VOLT_9_PIN, 77);
}



void warnDisplay(String title, String number, int x, int y) {
  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String(title, 20 + (230 * y), 60 + (40 * x));

  mylcd.Set_Text_colour(ORANGE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String(number, 135 + (230 * y), 60 + (40 * x));
}

void sendSMS(String phoneNumber, String message) {
  Serial1.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);

  Serial1.print("AT+CMGS=\"");
  Serial1.print(phoneNumber);
  Serial1.println("\"");
  delay(1000);

  Serial1.print(message);
  delay(100);
  Serial1.write(0x1A);  // Ctrl+Z to end the message
  delay(1000);
}

void updateSerial() {
  delay(500);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}
void clearArray() {
  arraySize = 0;
}

void appendToArray(int str, float value) {
  // Check if the array is not full
  if (arraySize < maxSize) {
    // Append the value to the end of the array
    myWarningStr[arraySize] = str;
    myWarningVal[arraySize] = value;
    // Increment the array size
    arraySize++;
  } else {
    // Print an error message if the array is full
    Serial.println("Error: Array is full");
  }
}
String getSensorName(int x) {
  if (x == 1) return "MQ2:";
  else if (x == 2) return "MQ3:";
  else if (x == 3) return "MQ4:";
  else if (x == 4) return "MQ5:";
  else if (x == 5) return "MQ6:";
  else if (x == 6) return "MQ7:";
  else if (x == 7) return "MQ8:";
  else if (x == 8) return "MQ9:";
  else if (x == 9) return "M135:";
  else if (x == 10) return "M137:";
  else if (x == 11) return "VOC:";
  else if (x == 12) return "PM: ";
}

void normalProcess() {
  getPMSValue();
  if (isMistEmpty()) {
    if (!mistEmpty) {
      sendSMS("+639984271960", "Fragrance needs refill");
      mistEmpty = true;
    }
  } else {
    mistEmpty = false;
  }
  if (myChrono.hasPassed(1000)) {
    myChrono.restart();
    readAllMQSensors();

    getRTC();

    readDHT11();

    int highValues = 0;
    bool trigger = false;
    int voc = getVOCValue();
    int sec = now.second();
    // Serial << sec << " " << endl;
    if (chronoSD.hasPassed(30000)) {
      chronoSD.restart();
       saveInfo(voc);
    }
   

    clearArray();
    //mq2 severe 400
    if (MQ2Lecture > 200) {
      highValues++;
      appendToArray(1, MQ2Lecture);
    }
    //mq3 severe 400
    if (MQ3Lecture > 180) {
      highValues++;
      appendToArray(2, MQ3Lecture);
    }
    //mq4 severe 4000
    if (MQ4Lecture > 2000) {
      highValues++;
      appendToArray(3, MQ4Lecture);
    }
    // mq5 starts with severe
    if (MQ5Lecture > 250) {
      highValues++;
      appendToArray(4, MQ5Lecture);
    }

    if (MQ6Lecture > 400) {
      highValues++;
      appendToArray(5, MQ6Lecture);
    }
    //mq7 severe 100
    if (MQ7Lecture > 50) {
      highValues++;
      appendToArray(6, MQ7Lecture);
    }
    //mq8 = 2 true
    //mq8 severe testing 100
    if (MQ8Lecture > 100) {
      highValues++;
      appendToArray(7, MQ8Lecture);
    }
    //mq9 512
    if (MQ9Lecture > 342) {
      highValues++;
      appendToArray(8, MQ9Lecture);
    }
    //mq135 severe 700
    if (MQ135Lecture > 400) {
      highValues++;
      trigger = true;
      appendToArray(9, MQ135Lecture);
    }

    //mq137 severe 111
    if (MQ137Lecture > 30) {
      highValues++;
      trigger = true;
      appendToArray(10, MQ137Lecture);
    }
    if (voc > 0) {
      highValues++;
      trigger = true;
      appendToArray(11, voc);
    }
    //pm10 severe 100
    if (pm10 > 50) {
      highValues++;
      appendToArray(12, pm10);
    }
    if (trigger) {
      mistingOn();
      if (!isSMSSend) {
        sendSMS("+639984271960", "Bathroom is smelly");
        isSMSSend = true;
      }
    } else {
      mistingOff();
      isSMSSend = false;
    }

    if (highValues != 0) {
      if (prevHighValue == 0) {

        mylcd.Fill_Screen(BLACK);
        mylcd.Set_Draw_color(WHITE);
        mylcd.Draw_Rectangle(0, 40, mylcd.Get_Display_Width() - 1, mylcd.Get_Display_Height() - 1);

        mylcd.Set_Text_colour(WHITE);
        mylcd.Set_Text_Back_colour(BLACK);
        mylcd.Set_Text_Size(4);
        mylcd.Print_String("WARNING", 145, 5);
      }

      for (int i = 0; i <= 11; i++) {
        int col = 0;
        if (i > 5) col = 1;
        // Serial << "arraySize :: " << arraySize << endl;
        if (arraySize >= i + 1) {
          if (myWarningStr[i] == 11) {
            int h = myWarningVal[i];
            String g;
            if (h == 0) {
              g = "CLEAN    ";
            } else if (h == 1) {
              g = "SP      ";
            } else if (h == 2) {
              g = "MP      ";
            } else if (h == 3) {
              g = "HP      ";
            }

            warnDisplay("VOC: ", g, i % 6, col);

          } else if (myWarningStr[i] == 9 || myWarningStr[i] == 3 || myWarningStr[i] == 4) {
            if (myWarningStr[i] > 12) {
              warnDisplay("     ", "      ", i % 6, col);
            } else {
              warnDisplay(getSensorName(myWarningStr[i]), String((int)myWarningVal[i]), i % 6, col);
            }

          } else {
            if (myWarningStr[i] > 12) {
              warnDisplay("     ", "      ", i % 6, col);
            } else {
              warnDisplay(getSensorName(myWarningStr[i]), String(myWarningVal[i], 1), i % 6, col);
            }
          }


        } else
          warnDisplay("     ", "      ", i % 6, col);
      }
    } else {
      if (prevHighValue != 0) {
        mylcd.Fill_Screen(BLACK);
      }
      setTime();

      if (chronoDisplay.hasPassed(5000)) {
        chronoDisplay.restart();
        displayMode++;
        clearSlot();
        if (displayMode > 4) {
          displayMode = 0;
        }
        switch (displayMode) {
          case 0:
            ppmDisplayLabel("MQ-2  ", "MQ-3    ", "MQ-4    ", "MQ-5    ", true, true);
            break;

          case 1:
            ppmDisplayLabel("MQ-6  ", "MQ-7    ", "MQ-8    ", "MQ-9    ", true, true);
            break;

          case 2:
            ppmDisplayLabel("MQ-135", "MQ-137  ", "        ", "        ", true, false);
            break;

          case 3:
            ppmDisplayLabel("VOC   ", "ENV Unit", "ENV Unit", "ENV Unit", false, false);
            break;

          case 4:
            tempDisplayLabel();
            break;
        }
      }
      switch (displayMode) {
        case 0:
          ppmDisplayValue(String(MQ2Lecture, 1), String(MQ3Lecture, 1), String(MQ4Lecture, 1), String(MQ5Lecture, 1));
          break;

        case 1:
          ppmDisplayValue(String(MQ6Lecture, 1), String(MQ7Lecture, 1), String(MQ8Lecture, 1), String(MQ9Lecture, 1));
          break;

        case 2:
          ppmDisplayValue(String(MQ135Lecture, 1), String(MQ137Lecture, 1), "    ", "    ");
          break;

        case 3:
          pmsDisplayValue(voc);
          break;

        case 4:
          ppmDisplayValue(String(temperature), String(humidity), "    ", "    ");
          break;
      }
    }
    prevHighValue = highValues;
  }
}


void saveInfo(int x) {
  String myVoc;

  if (x == 0) {
    myVoc = "Clean Air";
  } else if (x == 1) {
    myVoc = "Slight Polluted";


  } else if (x == 2) {
    myVoc = "Middle Polluted";

  } else if (x == 3) {
    myVoc = "Heavy Polluted";
  }
  String directory = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  if (!SD.exists(directory)) {
    SD.mkdir(directory);
  }

  myFile = SD.open(directory + "/" + String(now.hour()) + ".csv", FILE_WRITE);
  if (myFile) {

    myFile << MQ2Lecture << F(",") << MQ3Lecture << F(",")
           << MQ4Lecture << F(",") << MQ5Lecture << F(",") << MQ6Lecture << F(",")
           << MQ7Lecture << F(",") << MQ8Lecture << F(",") << MQ9Lecture << F(",")
           << MQ135Lecture << F(",") << MQ137Lecture << F(",") << myVoc << F(",")
           << pm10 << F(",") << pm25 << F(",") << pm100 << F(",") << temperature << F(",") << humidity << endl;
  }
  myFile.close();

  // Serial << MQ2Lecture << F(",") << MQ3Lecture << F(",")
  //        << MQ4Lecture << F(",") << MQ5Lecture << F(",") << MQ6Lecture << F(",")
  //        << MQ7Lecture << F(",") << MQ8Lecture << F(",") << MQ9Lecture << F(",")
  //        << MQ135Lecture << F(",") << MQ137Lecture << F(",") << myVoc << F(",")
  //        << pm10 << F(",") << pm25 << F(",") << pm100 << endl;
}


void clearSlot() {
  mylcd.Set_Draw_color(BLACK);
  mylcd.Fill_Rectangle(10, 80, 230, 170);
  mylcd.Fill_Rectangle((mylcd.Get_Display_Width() / 2) + 10, 80, (mylcd.Get_Display_Width() / 2) + 230, 170);
  mylcd.Fill_Rectangle(10, (mylcd.Get_Display_Height() / 2) + 60, 230, (mylcd.Get_Display_Height() / 2) + 150);
  mylcd.Fill_Rectangle((mylcd.Get_Display_Width() / 2) + 10, (mylcd.Get_Display_Height() / 2) + 60, (mylcd.Get_Display_Width() / 2) + 230, (mylcd.Get_Display_Height() / 2) + 150);
}

void setTime() {
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);

  mylcd.Set_Text_Size(3);
  String dateString = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  String timeString = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  // Concatenate date and time strings
  String dateTimeString = dateString + " " + timeString;
  mylcd.Print_String(dateTimeString, 30, 5);

  // Serial.print(now.year(), DEC);
  // Serial.print('/');
  // Serial.print(now.month(), DEC);
  // Serial.print('/');
  // Serial.print(now.day(), DEC);
  // Serial.print(" ");
  // Serial.print(now.hour(), DEC);
  // Serial.print(':');
  // Serial.print(now.minute(), DEC);
  // Serial.print(':');
  // Serial.print(now.second(), DEC);
  // Serial.println();
}


void pmsDisplayValue(int x) {
  if (x == 0) {
    mylcd.Set_Text_colour(GREEN);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("  CLEAN  ", 20, 100);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("   AIR   ", 20, 135);
  } else if (x == 1) {
    mylcd.Set_Text_colour(ORANGE);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String(" SLIGHT ", 20, 100);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("POLLUTED", 20, 135);
  } else if (x == 2) {
    mylcd.Set_Text_colour(ORANGE);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String(" MIDDLE ", 20, 100);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("POLLUTED", 20, 135);
  } else if (x == 3) {
    mylcd.Set_Text_colour(RED);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String(" HEAVY  ", 20, 100);
    mylcd.Set_Text_Size(4);
    mylcd.Print_String("POLLUTED", 20, 135);
  }

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(String(pm10), (mylcd.Get_Display_Width() / 2) + 20, 120);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(" PM ", (mylcd.Get_Display_Width() / 2) + 160, 115);
  mylcd.Print_String("1.0", (mylcd.Get_Display_Width() / 2) + 160, 140);


  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(String(pm25), 20, (mylcd.Get_Display_Height() / 2) + 100);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(" PM", 160, (mylcd.Get_Display_Height() / 2) + 95);
  mylcd.Print_String("2.5", 160, (mylcd.Get_Display_Height() / 2) + 120);

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(String(pm100), (mylcd.Get_Display_Width() / 2) + 20, (mylcd.Get_Display_Height() / 2) + 100);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("PM ", (mylcd.Get_Display_Width() / 2) + 160, (mylcd.Get_Display_Height() / 2) + 95);
  mylcd.Print_String("10 ", (mylcd.Get_Display_Width() / 2) + 160, (mylcd.Get_Display_Height() / 2) + 120);
}

void ppmDisplayValue(String val1, String val2, String val3, String val4) {
  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(val1, 20, 120);

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(val2, (mylcd.Get_Display_Width() / 2) + 20, 120);

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(val3, 20, (mylcd.Get_Display_Height() / 2) + 100);

  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String(val4, (mylcd.Get_Display_Width() / 2) + 20, (mylcd.Get_Display_Height() / 2) + 100);
}

void tempDisplayLabel() {
  mylcd.Set_Draw_color(GREEN);
  mylcd.Draw_Rectangle(0, 40, mylcd.Get_Display_Width() - 1, mylcd.Get_Display_Height() - 1);
  mylcd.Draw_Rectangle(1, 41, mylcd.Get_Display_Width() - 2, mylcd.Get_Display_Height() - 2);
  mylcd.Draw_Rectangle(0, 40, mylcd.Get_Display_Width() / 2, (mylcd.Get_Display_Height() + 40) / 2);
  mylcd.Draw_Rectangle(mylcd.Get_Display_Width() / 2, ((mylcd.Get_Display_Height() + 40) / 2), mylcd.Get_Display_Width() - 1, mylcd.Get_Display_Height() - 1);
  mylcd.Set_Text_Mode(0);

  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Temp   ", 10, 55);

  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("o", 175, 140);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("C", 190, 140);



  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Humid   ", (mylcd.Get_Display_Width() / 2) + 10, 55);


  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("%", (mylcd.Get_Display_Width() / 2) + 180, 140);



  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("        ", 10, (mylcd.Get_Display_Height() / 2) + 35);


  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("   ", 180, (mylcd.Get_Display_Height() / 2) + 120);



  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("         ", (mylcd.Get_Display_Width() / 2) + 10, (mylcd.Get_Display_Height() / 2) + 35);


  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("   ", (mylcd.Get_Display_Width() / 2) + 180, (mylcd.Get_Display_Height() / 2) + 120);
}
void ppmDisplayLabel(String label1, String label2, String label3, String label4, bool withUnit1, bool withUnit2) {
  mylcd.Set_Draw_color(GREEN);
  mylcd.Draw_Rectangle(0, 40, mylcd.Get_Display_Width() - 1, mylcd.Get_Display_Height() - 1);
  mylcd.Draw_Rectangle(1, 41, mylcd.Get_Display_Width() - 2, mylcd.Get_Display_Height() - 2);
  mylcd.Draw_Rectangle(0, 40, mylcd.Get_Display_Width() / 2, (mylcd.Get_Display_Height() + 40) / 2);
  mylcd.Draw_Rectangle(mylcd.Get_Display_Width() / 2, ((mylcd.Get_Display_Height() + 40) / 2), mylcd.Get_Display_Width() - 1, mylcd.Get_Display_Height() - 1);
  mylcd.Set_Text_Mode(0);

  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(label1, 10, 55);

  if (withUnit1) {
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("PPM", 180, 140);
  }


  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(label2, (mylcd.Get_Display_Width() / 2) + 10, 55);

  if (withUnit1) {
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("PPM", (mylcd.Get_Display_Width() / 2) + 180, 140);
  }

  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(label3, 10, (mylcd.Get_Display_Height() / 2) + 35);

  if (withUnit2) {
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("PPM", 180, (mylcd.Get_Display_Height() / 2) + 120);
  }


  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String(label4, (mylcd.Get_Display_Width() / 2) + 10, (mylcd.Get_Display_Height() / 2) + 35);

  if (withUnit2) {
    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("PPM", (mylcd.Get_Display_Width() / 2) + 180, (mylcd.Get_Display_Height() / 2) + 120);
  }
}


void getRTC() {
  now = rtc.now();
}




void displayTest() {
  mylcd.Set_Text_Mode(0);
  //display 1 times string
  mylcd.Fill_Screen(0x0000);
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(1);
  mylcd.Print_String("Hello World!", 0, 0);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 8, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 16, 0, ' ', 16);
  //mylcd.Print_String("DEADBEF", 0, 16);

  //display 2 times string
  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("Hello World!", 0, 40);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 56, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 72, 0, ' ', 16);
  //mylcd.Print_String("DEADBEEF", 0, 72);

  //display 3 times string
  mylcd.Set_Text_colour(BLUE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Hello World!", 0, 104);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 128, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 152, 0, ' ', 16);
  // mylcd.Print_String("DEADBEEF", 0, 152);

  //display 4 times string
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String("Hello!", 0, 192);

  //display 5 times string
  mylcd.Set_Text_colour(YELLOW);
  mylcd.Set_Text_Size(5);
  mylcd.Print_String("Hello!", 0, 224);

  //display 6 times string
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Size(6);
  mylcd.Print_String("Hello!", 0, 266);

  delay(3000);
}


void readAllMQSensors() {
  //Update the voltage Values
  MQ2.update();
  MQ3.update();
  MQ4.update();
  MQ5.update();
  MQ6.update();
  MQ8.update();
  MQ9.update();
  MQ135.update();

  //Read the sensor and print in serial port
  MQ2Lecture = MQ2.readSensor();
  MQ3Lecture = MQ3.readSensor();
  MQ4Lecture = MQ4.readSensor();
  MQ5Lecture = MQ5.readSensor();
  MQ6Lecture = MQ6.readSensor();
  MQ8Lecture = MQ8.readSensor();
  MQ135Lecture = MQ135.readSensor();
  MQ137Lecture = MQ137.getPPM();
}

int getVOCValue() {
  if (!digitalRead(A_PIN) && !digitalRead(A_PIN)) {
    return 0;
  } else if (!digitalRead(A_PIN) && digitalRead(A_PIN)) {
    return 1;
  } else if (digitalRead(A_PIN) && !digitalRead(A_PIN)) {
    return 2;
  } else if (digitalRead(A_PIN) && digitalRead(A_PIN)) {
    return 3;
  }
  //  return "UNKNOWN";
  return 12;
}

void readDHT11() {
  dht11.readTemperatureHumidity(temperature, humidity);
}

void testing() {
  if (Serial.available()) {
    String reading = Serial.readString();
    Serial << reading << endl;
    reading.trim();
    if (reading == "0") {
      Serial << "MISTING OFF" << endl;
      mistingOff();
    } else if (reading == "q") {
      Serial << "MISTING ON" << endl;
      mistingOn();
    } else if (reading == "w") {
      Serial << "Send SMS" << endl;
      sendSMS("+639984271960", "Hello");
    }
  }
}

void getPMSValue() {
  pms7003.updateFrame();
  if (pms7003.hasNewData()) {
    pm10 = pms7003.getPM_1_0();
    pm25 = pms7003.getPM_2_5();
    pm100 = pms7003.getPM_10_0();
  }
}

bool isMistEmpty() {
  return !digitalRead(FLOAT_PIN);
}

void mistingOn() {
  if (isWithPeople() || isMistEmpty()) {
    digitalWrite(MISTING_PIN, HIGH);
  } else {
    digitalWrite(MISTING_PIN, LOW);
  }
}

//void mistingOn() {
//  digitalWrite(MISTING_PIN, HIGH);
//  Serial.println("pin" + MISTING_PIN);
//}


void mistingOff() {
  digitalWrite(MISTING_PIN, HIGH);
}

void test() {
  // Test misting pin
  //  digitalWrite(MISTING_PIN, HIGH); // Turn on misting
  //  if (MISTING_PIN == LOW){
  //    Serial.println("LOW");
  //  }else{
  //    Serial.println("HIGH");
  //  }
  ////  delay(2000); // Wait for misting
  ////  digitalWrite(MISTING_PIN, LOW); // Turn off misting

  // Test PIR pin
  bool pirState = digitalRead(PIR_PIN);
  if (pirState == HIGH) {
    Serial.println("PIR Sensor is working.");
  } else {
    Serial.println("PIR Sensor is not working.");
  }
}
