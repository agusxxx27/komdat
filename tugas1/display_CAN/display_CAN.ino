#include <SPI.h>
#include "mcp_can.h"

#define SPI_CS_PIN  10
#define LED_ERR     13
#define NODE_ID     2

MCP_CAN CAN(SPI_CS_PIN);                                    

void setup()
{
  pinMode(LED_ERR, 1); // LED Error Indicator 
  digitalWrite(LED_ERR, 0); // Default off
  Serial.begin(9600); // Inisialisasi Serial Port Untuk Menampilkan data
  // Inisialisasi CAN Bus dengan baudrate 500 Kbps
  // Jika error nyalakan LED
  while (CAN_OK != CAN.begin(CAN_500KBPS))              
  {
    digitalWrite(LED_ERR, 1);
    delay(100);
  }
}

void loop()
{
  // Variabel untuk menampung ukuran data yg akan diterima
  unsigned char dataLength = 0; 
  unsigned char dataLengthpot = 0;
  // Variabel yg digunakan untuk menampung data (2 byte data yg akan diterima)
  unsigned char dataBuffer[2]; 
  unsigned char dataBufferpot[2]; 

  // Check jika ada data yg masuk
  if (CAN_MSGAVAIL == CAN.checkReceive())           
  {
    // Membaca data dari CAN Bus
    // Simpan ke variabel dataBuffer
    CAN.readMsgBuf(&dataLength, dataBuffer);    
    // Membaca ID dari pengirim data
    unsigned char senderID = CAN.getCanId();
    // Jika data yang diterima dari ID = 1
    if (senderID == 1) {
      // 2 byte data yg terpisah digabungkan menjadi int (2 byte)
      int adcFromNode = word(dataBuffer[1], dataBuffer[0]);
      // Perhitungan sensor suhu
      float temp = ((float)adcFromNode * 500.0) / 1023.0;
      // Kirim data ke serial port
      Serial.print("ID: ");
      Serial.print(senderID);
      Serial.print(" Data: ");
      Serial.print(dataBuffer[1], HEX);
      Serial.print(" ");
      Serial.print(dataBuffer[0], HEX);
      Serial.print(" Temp: ");
      Serial.print(temp);
      Serial.print(" C   ");
    }

    // Membaca data dari CAN Bus
    // Simpan ke variabel dataBuffer
    CAN.readMsgBuf(&dataLengthpot, dataBufferpot);    
    // Membaca ID dari pengirim data
    unsigned char senderID2 = CAN.getCanId();
    // Jika data yang diterima dari ID = 1
    if (senderID2 == 3) {
      // 2 byte data yg terpisah digabungkan menjadi int (2 byte)
      int adcFromNodepot = word(dataBufferpot[1], dataBufferpot[0]);
      // Perhitungan sensor suhu
      float pot = ((float)adcFromNodepot * 5.0) / 1023.0;
      // Kirim data ke serial port
      Serial.print("ID: ");
      Serial.print(senderID2);
      Serial.print(" Data: ");
      Serial.print(dataBufferpot[1], HEX);
      Serial.print(" ");
      Serial.print(dataBufferpot[0], HEX);
      Serial.print(" Temp: ");
      Serial.print(pot);
      Serial.println(" V");
    }
    delay(500);
  }
}
