


#include <MQUnifiedsensor.h>
#include <Streaming.h>
#include <DHT11.h>
#include <Chrono.h>



#define Board "Arduino Mega"
#define MQ2_PIN A1
#define MQ3_PIN A2
#define MQ4_PIN A6
#define MQ5_PIN A7
#define MQ135_PIN A8
#define MQ6_PIN A9
// #define MQ137_PIN A10
#define MQ8_PIN A11
#define MQ9_PIN A12
#define MQ7_PIN A13
#define A_PIN A5
#define B_PIN A4
#define DHT_PIN 2
#define FLOAT_PIN 3

#define RatioMQ2CleanAir 9.83   //RS / R0 = 9.83 ppm
#define RatioMQ3CleanAir 60     //RS / R0 = 60 ppm
#define RatioMQ4CleanAir 4.4    //RS / R0 = 4.4 ppm
#define RatioMQ5CleanAir 6.5    //RS / R0 = 6.5 ppm
#define RatioMQ6CleanAir 10     //RS / R0 = 10 ppm
#define RatioMQ7CleanAir 27.5   //RS / R0 = 27.5 ppm
#define RatioMQ8CleanAir 70     //RS / R0 = 70 ppm
#define RatioMQ9CleanAir 9.6    //RS / R0 = 9.6 ppm
#define RatioMQ135CleanAir 3.6  //RS / R0 = 3.6 ppm

#define ADC_Bit_Resolution 10     // 10 bit ADC
#define Voltage_Resolution 5      // Volt resolution to calc the voltage
#define Type "Arduino Mega 2560"  //Board used

MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, Type);
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ3_PIN, Type);
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ4_PIN, Type);
MQUnifiedsensor MQ5(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ5_PIN, Type);
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ135_PIN, Type);
MQUnifiedsensor MQ6(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ6_PIN, Type);
// MQ137 MQ137(MQ137_PIN, 2.64f, true);
MQUnifiedsensor MQ8(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ8_PIN, Type);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ9_PIN, Type);
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ7_PIN, Type);

DHT11 dht11(DHT_PIN);

unsigned long oldTime = 0;


int temperature = 0;
int humidity = 0;

int result = dht11.readTemperatureHumidity(temperature, humidity);

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
// Serial<<"BEGIN"<<endl;
  // MQ2.init();
  // MQ2.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ2.setA(574.25);
  // MQ2.setB(-2.222);  // Configure the equation to to calculate LPG concentration
  // MQ2.setR0(9.659574468);

  // MQ3.init();
  // MQ3.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ3.setA(0.3934);
  // MQ3.setB(-1.504);  //Configure the equation to calculate Alcohol concentration value
  // MQ3.setR0(3.86018237);

  // MQ4.init();
  // MQ4.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ4.setA(1012.7);
  // MQ4.setB(-2.786);  // Configure the equation to to calculate CH4 concentration
  // MQ4.setR0(3.86018237);

  // MQ5.init();
  // MQ5.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ5.setA(97124);
  // MQ5.setB(-4.918);  //Configure the equation to calculate Alcohol concentration value
  // MQ5.setR0(71.100304);

  // MQ6.init();
  // MQ6.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ6.setA(2127.2);
  // MQ6.setB(-2.526);  // Configure the equation to to calculate CH4 concentration
  // MQ6.setR0(13.4285714);

  // MQ7.init();
  // MQ7.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ7.setA(99.042);
  // MQ7.setB(-1.518);  // Configure the equation to calculate CO concentration value
  // MQ7.setR0(4);

  // MQ8.init();
  // MQ8.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ8.setA(976.97);
  // MQ8.setB(-0.688);  // Configure the equation to to calculate H2 concentration
  // MQ8.setR0(1);

  // MQ9.init();
  // MQ9.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ9.setA(1000.5);
  // MQ9.setB(-2.186);  // Configure the equation to to calculate LPG concentration
  // MQ9.setR0(9.42857143);

  // MQ135.init();
  // MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
  // MQ135.setA(102.2);
  // MQ135.setB(-2.473);
  // MQ135.setR0(9.42857143);

  // MQ137.begin();
  // //Calibration
  // Serial << "Calibrating please wait.";
  // float MQ2calcR0 = 0,
  //       MQ3calcR0 = 0,
  //       MQ4calcR0 = 0,
  //       MQ5calcR0 = 0,
  //       MQ6calcR0 = 0,
  //       MQ7calcR0 = 0,
  //       MQ8calcR0 = 0,
  //       MQ9calcR0 = 0,
  //       MQ135calcR0 = 0;


  // for (int i = 1; i <= 10; i++) {
  //   MQ2.update();
  //   MQ3.update();
  //   MQ4.update();
  //   MQ5.update();
  //   MQ6.update();
  //   MQ7.update();
  //   MQ8.update();
  //   MQ9.update();
  //   MQ135.update();

  //   MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
  //   MQ3calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
  //   MQ4calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
  //   MQ5calcR0 += MQ5.calibrate(RatioMQ5CleanAir);
  //   MQ6calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
  //   MQ7calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
  //   MQ8calcR0 += MQ8.calibrate(RatioMQ8CleanAir);
  //   MQ9calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
  //   MQ135calcR0 += MQ135.calibrate(RatioMQ135CleanAir);

  //   Serial << ".";
  // }
  // MQ2.setR0(MQ2calcR0 / 10);
  // MQ3.setR0(MQ3calcR0 / 10);
  // MQ4.setR0(MQ4calcR0 / 10);
  // MQ5.setR0(MQ5calcR0 / 10);
  // MQ6.setR0(MQ6calcR0 / 10);
  // MQ7.setR0(MQ7calcR0 / 10);
  // MQ8.setR0(MQ8calcR0 / 10);
  // MQ9.setR0(MQ9calcR0 / 10);
  // MQ135.setR0(MQ135calcR0 / 10);

  // Serial << "  done!." << endl;


  // if (isinf(MQ2calcR0) || isinf(MQ3calcR0) || isinf(MQ4calcR0) || isinf(MQ5calcR0) || isinf(MQ6calcR0) || isinf(MQ7calcR0) || isinf(MQ8calcR0) || isinf(MQ9calcR0) || isinf(MQ135calcR0)) {
  //   Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
  //   while (1)
  //     ;
  // }
  // if (MQ2calcR0 == 0 || MQ3calcR0 == 0 || MQ4calcR0 == 0 || MQ5calcR0 == 0 || MQ6calcR0 == 0 || MQ7calcR0 == 0 || MQ8calcR0 == 0 || MQ9calcR0 == 0 || MQ135calcR0 == 0) {
  //   Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
  //   while (1)
  //     ;
  // }

  // pinMode(A_PIN, INPUT);
  // pinMode(B_PIN, INPUT);
  // pinMode(FLOAT_PIN, INPUT_PULLUP);

  Serial3.begin(9600);  //PMS
  Serial1.begin(9600);  //GSM
}


struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

void loop() {
  oldTime = millis();
  while (millis() - oldTime <= (60 * 1000)) {
    // VH 5 Volts
    analogWrite(5, 255);  // 255 is DC 5V output
    readAllMQSensors();
    delay(500);
  }
  90s cycle
  // Serial<<"isMistEmpty :: "<<isMistEmpty()<<endl;
}

void getPMSValue() {
  if (readPMSdata(&Serial3)) {
    // reading data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: ");
    Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: ");
    Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: ");
    Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: ");
    Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:");
    Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:");
    Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:");
    Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:");
    Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:");
    Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:");
    Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
  }
}


boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }

  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }

  uint8_t buffer[32];
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }

  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}



String getVOCValue() {
  if (!digitalRead(A_PIN) && !digitalRead(A_PIN)) {
    return "Clear";
  } else if (!digitalRead(A_PIN) && digitalRead(A_PIN)) {
    return "Slight Pollution";
  } else if (digitalRead(A_PIN) && !digitalRead(A_PIN)) {
    return "Middle Pollution";
  } else if (digitalRead(A_PIN) && digitalRead(A_PIN)) {
    return "Heavy Pollution";
  }
  return "UNKNOWN";
}

void readAllMQSensors() {
  //Update the voltage Values
  MQ2.update();
  MQ3.update();
  MQ4.update();
  MQ5.update();
  MQ6.update();
  MQ7.update();
  MQ8.update();
  MQ9.update();
  MQ135.update();
  //Read the sensor and print in serial port
  float MQ2Lecture = MQ2.readSensor();
  float MQ3Lecture = MQ3.readSensor();
  float MQ4Lecture = MQ4.readSensor();
  float MQ5Lecture = MQ5.readSensor();
  float MQ6Lecture = MQ6.readSensor();
  float MQ7Lecture = MQ7.readSensor();
  float MQ8Lecture = MQ8.readSensor();
  float MQ9Lecture = MQ9.readSensor();
  float MQ135Lecture = MQ135.readSensor();
  // float MQ137Lecture = MQ137.getPPM();


  Serial.print("| ");
  Serial.print(MQ2Lecture);
  Serial.print("   |   ");
  Serial.print(MQ3Lecture);
  Serial.print("   |   ");
  Serial.print(MQ4Lecture);
  Serial.print("   |   ");
  Serial.print(MQ5Lecture);
  Serial.print("   |   ");
  Serial.print(MQ6Lecture);
  Serial.print("   |   ");
  Serial.print(MQ7Lecture);
  Serial.print("   |   ");
  Serial.print(MQ8Lecture);
  Serial.print("   |   ");
  Serial.print(MQ9Lecture);
  Serial.print("|");
  Serial.print(MQ135Lecture);
  Serial.println("|");
}


void sendSMS(String phoneNumber, String message) {
  Serial1 << "AT+CMGF=1" << endl;  //sms mode to text
  delay(100);
  Serial1 << "AT+CMGS=\"" << phoneNumber << "\"" << endl;
  delay(100);
  Serial1 << message << endl;
  delay(100);
  Serial1 << (char)26 << endl;
  delay(100);
  Serial1 << endl;
}

bool isMistEmpty() {
  return !digitalRead(FLOAT_PIN);
}

void readDHT() {
  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    // Print error message based on the error code.
    Serial.println(DHT11::getErrorString(result));
  }
}