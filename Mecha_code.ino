#include <Wire.h>
#include <Servo.h>

int motor = 6;
int pwm ;

int16_t Acc_rawX, Acc_rawY, Acc_rawZ, Gyr_rawX, Gyr_rawY, Gyr_rawZ;


float Acceleration_angle[2];
float Gyro_angle[2];
float Total_angle[2];




float elapsedTime, time, timePrev;
int i;
float rad_to_deg = 180 / 3.141592654;





void setup() {
  Wire.begin(); //begin the wire comunication
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
//  Serial.begin(9600);
  pinMode(motor, OUTPUT);

  time = millis(); //Start counting time in milliseconds
  delay(4000); 
}

void loop(){



  Wire.beginTransmission(0x68);
  Wire.write(0x3B); //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);

  Acc_rawX = Wire.read() << 8 | Wire.read(); //each value needs two registres
  Acc_rawY = Wire.read() << 8 | Wire.read();
  Acc_rawZ = Wire.read() << 8 | Wire.read();


  Acceleration_angle[0] = atan((Acc_rawY / 16384.0) / sqrt(pow((Acc_rawX / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;

  Acceleration_angle[1] = atan(-1 * (Acc_rawX / 16384.0) / sqrt(pow((Acc_rawY / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;

  Wire.beginTransmission(0x68);
  Wire.write(0x43); //Gyro data first adress
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 4, true); //Just 4 registers

  Gyr_rawX = Wire.read() << 8 | Wire.read(); //Once again we shif and sum
  Gyr_rawY = Wire.read() << 8 | Wire.read();



  /---X---/
  Gyro_angle[0] = Gyr_rawX / 131.0;
  /---Y---/
  Gyro_angle[1] = Gyr_rawY / 131.0;


  Total_angle[0] = 0.98 * (Total_angle[0] + Gyro_angle[0] * elapsedTime) + 0.02 * Acceleration_angle[0];

  Total_angle[1] = 0.98 * (Total_angle[1] + Gyro_angle[1] * elapsedTime) + 0.02 * Acceleration_angle[1];



  if (Total_angle[1] >= 5) {
    if (Total_angle[1] >= 16) {
          pwm = 70;
    }
   else{
       pwm = map(Total_angle[1], 5, 16, 100, 120);  
    }
 
  }
  else {
    pwm = 70;
  }
//      Serial.print(pwm);
//      Serial.print(",");
//      Serial.println(Total_angle[1]);
      analogWrite(motor, pwm);
      
   

}//end of loop void
