#include "MQTT.h"
#include "oled-wing-adafruit.h"

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;

double latitude = 0.00;
double longitude = 0.00;

String eTopicOne = "testtopic/EpicTopic2";
String eTopicTwo = "testtopic/EpicTopic3";

void setup()
{
    Serial.begin(9600);
    display.setup();
    resetDisplay();
    display.display();

    client.subscribe("testtopic/EpicTopic2");
    client.subscribe("testtopic/EpicTopic3");
}
void loop()
{
    display.loop();
    if (client.isConnected())
    {
        client.loop();
        client.publish("testtopic/EpicTopic1", "Send");
        delay(10000);
    }
    else
    {
        client.connect(System.deviceID());
        client.subscribe("testtopic/EpicTopic2");
        client.subscribe("testtopic/EpicTopic3");
        Serial.println("DIDN'T WORK");
    }
   
}
void resetDisplay()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
}
void callback(char *topic, byte *payload, unsigned int length)
{
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String s = p;
    double value = s.toFloat();

    String callbackTopic = String(topic);

    Serial.printf("%s", p);
    Serial.println();
    Serial.println(value);
    if (callbackTopic.compareTo(eTopicOne))
    {
        Serial.println("LAT");
        latitude = value;
    }
    if (callbackTopic.compareTo(eTopicTwo))
    {
        Serial.println("LONG");
        longitude = value;
    }
    resetDisplay();
    display.println(latitude);
    display.println(longitude);
    display.display();
}
void caller()
{
    client.publish("testtopic/EpicTopic1", "Send");
}