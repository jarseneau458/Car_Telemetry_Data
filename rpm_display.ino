#include <SPI.h>
#include <mcp_can.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>


// Set SPI CS Pin to Pin 9
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);
LiquidCrystal lcd(7,8,5,2,4,3);

void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  
  if(CAN_OK == CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ)) {
    lcd.setCursor(0,1);
    lcd.print("CAN Bus Ok");
  } else {
    lcd.setCursor(0,1);
    lcd.print("CAN Bus Failed");
    while(1); //Stop if it fails 
  }
  
  // Set the module to normal mode to communicate with the car
  CAN.setMode(MCP_NORMAL); 
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rpm:");

}


void loop() { 
  //  Engine RPM (PID 0x0C)
  // Format: {Num Data Bytes, Service, PID, Padding...}
  byte request[8] = {0x02, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00};
  
  // Send the request to the address 0x7DF
  // 0 = standard frame, 8 = data length
  CAN.sendMsgBuf(0x7DF, 0, 8, request);
  
  
  long startTime=millis();

  while(millis() - startTime<50){
    // Check if there is received a response
    if(CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int rxId;
    unsigned char len = 0;
    //char array to store the data coming in from the car
    unsigned char rxBuf[8];
    
    // Reads the incoming message from the ECU and writes the corresponding data into memory 
    CAN.readMsgBuf(&rxId, &len, rxBuf);

     // ECU replies on ID 0x7E8
     // Check for ECU reply on ID 0x7E8 for Service 01 (0x41) and PID of RPM (0x0C)
    if(rxId == 0x7E8 && rxBuf[1]==0x41 && rxBuf[2]== 0x0C) {
        handleRPM(rxBuf);
      }
    }
  } 
  // Wait before asking again to avoid flooding the CAN bus
  delay(150);

}

void handleRPM(unsigned char rxBuf[]){
  //16 bit rpm value split across 2 bytes: rxBuf[3](high byte), rxBuf[4](low byte)
        //formula to calulate RPM: ((A * 256)+B)/ 4
        int rpm = ((rxBuf[3] * 256) + rxBuf[4]) / 4;
        char rpmBuf[6];
        sprintf(rpmBuf, "%-4d", rpm); 
        lcd.setCursor(0, 1);
        lcd.print(rpmBuf);
}       




