#include <mcp_can.h>
#include <SPI.h>

#define SPI_CS_PIN  10
#define LED_ERR     13
#define TEMP_SENSOR A0
//#define TEMP_SENSOR A1
#define NODE_ID     1
#define NODE_IDpot     3


MCP_CAN CAN(SPI_CS_PIN);

void setup()
{
  pinMode(LED_ERR, 1); // LED Error Indicator
  pinMode(TEMP_SENSOR, 0); // LM35 Sensor
  // pinMode(pot, 1); // LM35 Sensor
  digitalWrite(LED_ERR, 0); // Default off
  Serial.begin(9600);
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
  unsigned char tempNodeData[4] = {0 , 0, 0, 0};
  // Variabel untuk menampung ukuran data yg akan diterima
  unsigned char dataLength = 0;
  // Variabel yg digunakan untuk menampung data (2 byte data yg akan diterima)
  unsigned char dataBuffer[1];

  // Variabel untuk menampung 2 byte data yang akan dikirimkan
  unsigned char tempNodeDatapot[2] = {0 , 0};
  // Variabel untuk menampung ukuran data yg akan diterima
  unsigned char dataLengthpot = 0;
  // Variabel yg digunakan untuk menampung data (2 byte data yg akan diterima)
  unsigned char dataBufferpot[1];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&dataLength, dataBuffer);
    unsigned char senderID = CAN.getCanId();
    Serial.print("ID : ");
    Serial.print(senderID);
    Serial.print(" Data : ");
    Serial.println(dataBuffer[0], DEC);
    // Check ID pengirim
    if (senderID == 2) {
      delay(100);
      if (dataBuffer[0] == 100) {
        // Baca nilai ADC dari sensor LM35
        // Nilai ADC default-nya 10 bit
        // Tipe data int berukuran 16 bit (2 byte) pada arduino uno
        int adcValue = analogRead(TEMP_SENSOR);
        int adcValuepot = analogRead(A1);

        // Tipe data unsigned char berukuran 8 bit (1 byte)
        // adcValue (2 byte data) dipecah menjadi 2 bagian
        // Disimpan pada 2 indeks array dgn tipe data unsigned char (1 byte)
        

        // Tipe data unsigned char berukuran 8 bit (1 byte)
        // adcValue (2 byte data) dipecah menjadi 2 bagian
        // Disimpan pada 2 indeks array dgn tipe data unsigned char (1 byte)
        tempNodeData[0] = lowByte(adcValue);
        tempNodeData[1] = highByte(adcValue);
        tempNodeData[2] = lowByte(adcValuepot);
        tempNodeData[3] = highByte(adcValuepot);

        // Kirim data via CAN Bus
        // NODE_ID      : 1 (ID dari node telah didefinisikan diatas)
        // Mode 0       : Menggunakan Standar CAN 2.0 A (11 bit ID)
        // Data Length  : 2 byte
        // Data         : tempNodeData
        if (CAN.sendMsgBuf(NODE_ID, 0, 4, tempNodeData) == CAN_OK) {
          Serial.println("Send ok");
        }
      }
    }
  }
}
