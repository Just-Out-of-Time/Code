#include <ESP8266WiFi.h>

void setup()
{  
  // Start Serial (to display results on the Serial monitor)
  Serial.begin(115200);

  pinMode(A0, INPUT);
  //pinMode(12, INPUT);
  //pinMode(10, INPUT);


}
void loop() {

  // Read GPIO 5 and print it on Serial port
  Serial.print("Read out of A0: ");
  Serial.println(analogRead(A0));
  //Serial.print("Read out of 10: ");
  //Serial.println(digitalRead(10));

  // Wait 1 second
  delay(1000);
}
