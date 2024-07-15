const int e18_sensor = 7;
int input_Val = 0;


void setup() {
Serial.begin(9600);
pinMode (e18_sensor, INPUT);
}

void loop() {
input_Val = digitalRead(e18_sensor);
Serial.println(input_Val);
delay(10);
}