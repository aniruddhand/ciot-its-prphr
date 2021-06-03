#include <SPI.h>
#include <mcp2515.h>
#include <SimpleDHT.h>

#define DHT_DATA_PIN A0

struct can_frame can_msg;
MCP2515 mcp2515(10);

SimpleDHT11 dht11(DHT_DATA_PIN);

void setup() {
  while (!Serial);
  
  SPI.begin();
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  int temperature = read_temperature();

  if (temperature != -1) {
    populate_CAN_message(temperature);
    mcp2515.sendMessage(&can_msg);    
  }

  delay(1500);
}

int read_temperature() {
  byte temperature = 0;
  byte humidity = 0;

  int err = SimpleDHTErrSuccess;
  
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Error reading temperature from sensor!");    
    return -1;
  }

  return (int) temperature;
}

void populate_CAN_message(int temperature) {
  can_msg.can_id  = 0x036;
  can_msg.can_dlc = 8;
  can_msg.data[0] = temperature;
  can_msg.data[1] = 0x00;
  can_msg.data[2] = 0x00;
  can_msg.data[3] = 0x00;
  can_msg.data[4] = 0x00;
  can_msg.data[5] = 0x00;
  can_msg.data[6] = 0x00;
  can_msg.data[7] = 0x00;
}
