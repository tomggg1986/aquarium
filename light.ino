//change light state by sound
void changeLightState() {
  int state = analogRead(lightManuallyChange);
  if (state < 300) {
    Serial.print("SENSOR: ");
    Serial.println(state);
    if (lightState == LOW) {
      lightState = HIGH;
    } else {
      lightState = LOW;
    }
    digitalWrite(light, lightState);
  }
}
void lightsON() {
  Serial.println("Light ON");
  lightState = LOW;
  digitalWrite(light, lightState);
}

void lightsOFF() {
  Serial.println("Light OFF");
  lightState = HIGH;
  digitalWrite(light, lightState);
}

class A
{
  public:
   int x;
   virtual void f() { x=1; }
};
