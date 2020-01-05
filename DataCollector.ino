/*Lib Declar*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BLE_UART.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


/* BLE pin define*/
#define ADAFRUITBLE_REQ 10  //D10 pin REQ
#define ADAFRUITBLE_RDY 2   //D2 pin to RDY
#define ADAFRUITBLE_RST 9    //D9 pin to RST


/*Magic*/
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;//Set Freshrate
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);//Check I2C Address
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED; //Set status


/*Magic Data */
#define MAX (7)
#define Size (25)

char Xbuffer[MAX] = {0,};//Magic data buffer that holds data 
char Ybuffer[MAX] = {0,};
char Zbuffer[MAX] = {0,};
char Sum[Size] = {0,};


/***********************************************/








/*************************************************************
    Configure the Arduino and start advertising with the radio
**************************************************************/

void setup(void)
{ 
  Serial.begin(9600);
  while(!Serial); 
  Serial.println(F("Alert for Big News "));
  BTLEserial.setDeviceName("1Alert");
  BTLEserial.begin();
  if (!bno.begin())
  {
    //Bad Connection 
    Serial.print("Check BNO Wiring");
    while (1);
  }
 
  bno.setExtCrystalUse(true); //provide better timing accurcy
}

/************************************
    Constantly pulling for new events
    Do loopy loop
*************************************/
void loop()
{
  // Get up and do work.
  BTLEserial.pollACI();
 /***********************************************/
  //BNO Reading
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  
 double x_accel = euler.x();
 double y_accel = euler.y();
 double z_accel = euler.z();
/***********************************************/




/***********************************************/
  // Ask what is  current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {

    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Advertising started"));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    //  set the last status change to this one
    laststatus = status;
  }
/***********************************************/

  if (status == ACI_EVT_CONNECTED) {
    
/***********************************************/
    /* Receive Data
    if (BTLEserial.available()) {
    Serial.print("Recived Data:");}
    while (BTLEserial.available()) { 
      char c = BTLEserial.read(); //Read 1 Char each time under while 
      Serial.print(c);
    }*/
/***********************************************/
      Serial.setTimeout(100); // 100 millisecond timeout 
      dtostrf(x_accel,6, 2, Xbuffer);//Magically convert to ASCII
      dtostrf(y_accel,6, 2, Ybuffer);
      dtostrf(z_accel,6, 2, Zbuffer);
      strcpy(Sum,Xbuffer);
      strcat(Sum,",");
      strcat(Sum,Ybuffer);
      strcat(Sum,",");
      strcat(Sum,Zbuffer);
      strcat(Sum,";");
     //The data set has the size of 6 "+/-dd.dd" with two decimal places and dot
     
     Serial.print(F("\n* Sending ---> \"")); Serial.print(Sum); Serial.println("\"");
      // We need to convert the line to bytes, no more than 20 at this time
      char buffersize = min(20, strlen(Sum));
      // write the data
      BTLEserial.write((uint8_t *)Sum,buffersize);
      delay(BNO055_SAMPLERATE_DELAY_MS);
  }
}
