#include <Timezone.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <leOS2.h>
#include <TimeAlarms.h>
#include <Time.h>
#include <EEPROM.h>
#include <EtherCard.h>


const int light = 7; // pin that start light (digital)
const int lightManuallyChange = A1;
int lightOnTime[3]; //time of start light this value if read form eeprom memory 

leOS2 threads;
int lightState = 0; // initial light state

static byte ntpServer[] = {194,109,6,2}; //ntp.xs4all.nl but you can cange in any public ntp server.
uint8_t ntpMyPort = 123;


// TimeZone : United States Eastern)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, +120};  //UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, +120};   //UTC - 5 hours
Timezone usEastern(usEDT, usEST);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

class Customlcd
{ 
  public:
     Customlcd(){}
     void static displayCurrentTime() {
      lcd.setCursor(0, 1);    
      printDigits(hour());
      lcd.print(":");
      printDigits(minute());
      lcd.print(":");
      printDigits(second());  
    }
    void static printDigits(int digits)
    {
    if(digits < 10)
     lcd.print('0');
    lcd.print(digits);
   }
};
Customlcd customlcd; 
//EtherCard config
#define REQUEST_RATE 5000 // milliseconds

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// remote website name
const char website[] PROGMEM = "google.com";

byte Ethernet::buffer[700];
static long timer;

const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Service Temporarily Unavailable"
  "</title></head>"
  "<body>"
    "<h3>This service is currently unavailable</h3>"
    "<p><em>"
      "The main server is currently off-line.<br />"
      "Please try again later."
    "</em></p>"
  "</body>"
"</html>"
;
//--------------------------------------------------------------

void setup()
{    
  saveLightOnTime(14, 10, 0);
  readLightOnTime(lightOnTime);
  
  pinMode(light, OUTPUT);
  pinMode(lightManuallyChange, INPUT);
  digitalWrite(light, HIGH);

  //Serial.begin(9600);
  Serial.begin(57600);
   
  boolean initialSuccess = initialEthernet();
  if(initialSuccess){
    setTime(getNtpTime());
  }  
  lcd.begin(16, 2);
  lcd.backlight(); // zalaczenie podwietlenia
  lcd.setCursor(0, 0);
  lcd.print("Aquarium controller");
  customlcd = Customlcd();

  // create the alarms
  Serial.print("Lights On: ");
  Serial.print(lightOnTime[0]);
  Serial.print(":");
  Serial.print(lightOnTime[1]);
  Serial.print(":");
  Serial.print(lightOnTime[2]);
  Serial.println("");
  Alarm.alarmRepeat(lightOnTime[0], lightOnTime[1] , lightOnTime[2], lightsON); // start light at right time 

  threads.begin();
  threads.addTask(changeLightState, threads.convertMs(25), SCHEDULED_IMMEDIATESTART);
  threads.addTask(customlcd.displayCurrentTime ,threads.convertMs(1000), SCHEDULED_IMMEDIATESTART);
}

time_t getNtpTime() {   
  unsigned long timeFromNTP;   
  const unsigned long seventy_years = 2208988800UL;      

  ether.ntpRequest(ntpServer, ntpMyPort);   
  while(true) {       
    word length = ether.packetReceive();       
    ether.packetLoop(length);       
    if(length > 0 && ether.ntpProcessAnswer(&timeFromNTP, ntpMyPort)) {
      Serial.print("Time from NTP: ");
      Serial.println(timeFromNTP);
      return usEastern.toLocal(timeFromNTP - seventy_years);
    }
  }
  return 0;
}

void  loop() {  
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
  Alarm.delay(0);
}
