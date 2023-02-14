#include "HX711.h"
#define LOADCELL_DOUT_PIN         D4
#define LOADCELL_SCK_PIN          D5

HX711 scale;
#define K   7050.0 // calibration_factor



#include <SoftwareSerial.h>
const int Rx = D0; // HC 모듈의 TX와 연결된 핀
const int Tx = D9; // HC 모듈의 RX와 연결된 핀
SoftwareSerial BTserial(Tx, Rx); //Rx, Tx

String cmd = "";
boolean isNetworkEnd = false;



void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(K);
  delay(2000);
  scale.tare();
}



void loop() {
  while (BTserial.available()) {
    byte data = BTserial.read();

    // 한 번 통신 종료
    if (BTserial.available() == 0) {
      isNetworkEnd = true;
    }
    cmd = cmd + (char)data;
  }



  if (isNetworkEnd) {
    Serial.println(cmd);

    // 0점 조절
    if (cmd == "zero") {
      Serial.println("OK");
      delay(2000);
      scale.tare();
    }

    isNetworkEnd = false;
    cmd = "";
    
  }



  float g1 = scale.get_units();
  Serial.print("gram 5.6: "); Serial.println(g1 * 5.6, 0);
  float gramValue = g1 * 5.6;

  String tempStr = String(gramValue);

  char tempStrChar[32] = {0, };
  tempStr.toCharArray(tempStrChar, sizeof(tempStrChar));
  Serial.print("tempStrChar: "); Serial.println(tempStrChar);

  BTserial.write(tempStrChar);



  delay(1000);
}
