#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN  10
#define LED_ERR     13
#define TEMP_SENSOR A0
#define pot A1
#define NODE_ID     1

MCP_CAN CAN(SPI_CS_PIN);

void setup()
{  
  pinMode(LED_ERR, 1); // LED Error Indicator  
  pinMode(TEMP_SENSOR, 0); // LM35 Sensor
  digitalWrite(LED_ERR, 0); // Default off
  // Inisialisasi CAN Bus dengan Baudrate 500 Kbps
  // Jika error nyalakan LED
  while (CAN_OK != CAN.begin(CAN_500KBPS))              
  {
    digitalWrite(LED_ERR, 1);
    delay(100);
  }
}

void loop() {
  // Variabel untuk menampung 2 byte data yang akan dikirimkan
  unsigned char tempNodeData[2] = {0 , 0};
  unsigned char tempNodepot[2] = {0 , 0};

  // Baca nilai ADC dari sensor LM35
  // Nilai ADC default-nya 10 bit
  // Tipe data int berukuran 16 bit (2 byte) pada arduino uno
  int adcValue = analogRead(TEMP_SENSOR);
  int potensio =analogRead(pot);

  // Tipe data unsigned char berukuran 8 bit (1 byte)
  // adcValue (2 byte data) dipecah menjadi 2 bagian
  // Disimpan pada 2 indeks array dgn tipe data unsigned char (1 byte)
  tempNodeData[0] = lowByte(adcValue);
  tempNodeData[1] = highByte(adcValue);
  tempNodepot[0] = lowByte(potensio);
  tempNodepot[1] = highByte(potensio);
  

  // Kirim data via CAN Bus
  // NODE_ID      : 1 (ID dari node telah didefinisikan diatas)
  // Mode 0       : Menggunakan Standar CAN 2.0 A (11 bit ID)
  // Data Length  : 2 byte
  // Data         : tempNodeData
  CAN.sendMsgBuf(NODE_ID, 0, 2, tempNodeData);
  CAN.sendMsgBuf(3, 0, 2, tempNodepot);

  // Delay 50ms antar pengiriman data
  delay(50);
}

