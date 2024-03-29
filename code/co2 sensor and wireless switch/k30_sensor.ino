/* 
*  Simple K30 + WiFi + Arduino sketch
*  Writtent by Marco Schwartz for Open Home Automation
*/

// Include required libraries
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include "kSeries.h"
#include<stdlib.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network (change with your settings !)
#define WLAN_SSID       "**********"        // cannot be longer than 32 characters!
#define WLAN_PASS       "**********"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2


#define WEBSITE      "192.168.1.5"
#define WEBPAGE      "/sensor.php?data=600"
// K30 sensor pins
kSeries K_30(6,7); //Initialize a kSeries Sensor with pin 6 as Rx and 7 as Tx

// Create CC3000 & DHT instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
                                         
// Local server IP, port, and repository (change with your settings !)
uint32_t ip = cc3000.IP2U32(192,168,1,5);
int port = 3000;
String repository = "192.168.1.7/";
                                         
void setup(void)
{
 
  // Init serial  
  Serial.begin(9600);
    Serial.println("ABC");
  // Initialise the CC3000 module
  if (!cc3000.begin())
  {
    Serial.println("ABC1");
    while(1);
  }
Serial.println("ABC2");
  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println("Connected to WiFi network!");
    
  // Check DHCP
  Serial.println(F("Request DHCP"));


  
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }  
  
}

void loop(void)
{
  
    // Measure the CO2 ppm
    double co2 = K_30.getCO2('p');
     
    // Transform to String
    String co2_ppm = String((int) co2);
    
    // Print data
    Serial.print("CO2 ppm: ");
    Serial.println(co2_ppm);
    Serial.println("");
    
    // Send request
    String request = "GET "+ repository + "sensor.php?data=" + co2_ppm + " HTTP/1.1\r\n"+"Host: "+WEBSITE+"\r\n";
   /* String req="/sensor.php?data="+co2_ppm;*/
   //#define req "/sensor.php?data="+co2_ppm
    String  req="/co2level?co2Value="+co2_ppm;
    //Serial.println(request);
    send_request(req);
    
    // Update every second
    delay(1000);
}

// Function to send a TCP request and get the result as a string
void send_request (String request) {
     
    // Connect    
    Serial.println("Starting connection to server...");
     delay(1000); 
     Serial.println(ip);
    Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
  // delay(3000); 
  Serial.println(client.connected());
    
    // Send request
    if (client.connected()) {
      client.fastrprint(F("GET "));
    client.fastrprint(request.c_str());
    client.fastrprint(F(" HTTP/1.1\r\n"));
    client.fastrprint(F("Host: ")); client.fastrprint(WEBSITE); client.fastrprint(F("\r\n"));
    client.fastrprint(F("\r\n"));
    client.println();

      
      //client.println(request);        
    // client.println(F(""));
      Serial.println("Connected & Data sent");
    } 
    else {
      Serial.println(F("Connection failed"));    
    }

    /*while (client.connected()) {
      while (client.available()) {

      // Read answer
      char c = client.read();
      }
    }*/
    Serial.println("Closing connection");
    Serial.println("");
    client.close();
    
}


