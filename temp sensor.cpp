#include <SPI.h>
#include <Ethernet.h>
#include "DHT_NEW.h"
DHT _dht1;

byte ethernet_mac [] = {0x78, 0xAC, 0xC0, 0xAB, 0x63, 0x78};
IPAddress ethernet_ip(192, 168, 0, 228);
byte ethernet_dns [] = {192, 168, 0, 254};
byte ethernet_gateway [] = {192, 168, 0, 254};
byte ethernet_subnet [] = {255, 255, 252, 0};
EthernetServer _tspWebServer(80);
EthernetClient _tspWebServer_client;
unsigned long _dht1LRT = 0UL;
unsigned long _dht1Tti = 0UL;
void setup()
{
  Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);
delay(1000);
_tspWebServer.begin();
_dht1.setup(7);
_dht1LRT = millis();
_dht1Tti = millis(); 
}
void loop()
{_tspWebServer_client = _tspWebServer.available();
if (_tspWebServer_client) {
 boolean _WSCLineIsBlank = true;
String _WSCReqest="";
bool _WSCIsFirsLine=1;
bool _WSCIsParse=0;
int _WSCPageNumber=0;
while (_tspWebServer_client.connected()) 
{ while( _tspWebServer_client.available())  
{char _tempWebServerChar = _tspWebServer_client.read(); 
if(_WSCIsFirsLine) {_WSCReqest+= _tempWebServerChar;}
 if (_tempWebServerChar == '\n' && _WSCLineIsBlank) {_sendWebServerPage(_WSCPageNumber); break;}
if (_tempWebServerChar == '\n') {_WSCLineIsBlank = true; _WSCIsFirsLine=0; if (!_WSCIsParse){ _WSCPageNumber=_parseWebServerReqest(_WSCReqest); _WSCIsParse=1;} } else if (_tempWebServerChar != '\r') { _WSCLineIsBlank = false;}
}}}





//Плата:1
if(_isTimer(_dht1Tti, 1000)) {
if(_isTimer(_dht1LRT,( _dht1.getMinimumSamplingPeriod()))) {
_dht1.readSensor();
_dht1LRT = millis(); 
_dht1Tti = millis(); 
}
}




}
bool _isTimer(unsigned long startTime, unsigned long period )
  {
  unsigned long currentTime;
currentTime = millis();
if (currentTime>= startTime) {return (currentTime>=(startTime + period));} else {return (currentTime >=(4294967295-startTime+period));}
  }
void _sendWebServerPage(int sendPageNumber)
{_tspWebServer_client.println("HTTP/1.1 200 OK");
_tspWebServer_client.println("Connection: close");  
 if ( (sendPageNumber==1)) {if(sendPageNumber==1){_tspWebServer_client.println("Refresh: 5");}
}
_tspWebServer_client.println();
_tspWebServer_client.println("<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 4.01 Transitional//EN"">");
_tspWebServer_client.println("<html><head>");
_tspWebServer_client.println("<META content=""text/html; charset=utf-8"" http-equiv=""Content-Type"">");
_tspWebServer_client.println("<<title>Temp sensor</title>");
_tspWebServer_client.println("<style> p {font-family: Verdana, Arial, Helvetica, sans-serif;");
_tspWebServer_client.println("font-size: 14pt;}</style>");
_tspWebServer_client.println("</head><body><p>");
if (sendPageNumber ==1) {_sendWebServerPage1();}
_tspWebServer_client.println("</p></body></html>");
 delay(1); _tspWebServer_client.stop();}
int _parseWebServerReqest(String reqestAddres)
{
int index;
int result=0;
index=reqestAddres.indexOf("/");
  reqestAddres =_stringWithoutCharWithIndex(reqestAddres,0,(index));
index=reqestAddres.indexOf(" ");
reqestAddres =_stringWithoutCharWithIndex(reqestAddres,index,(reqestAddres.length()-index));
if (reqestAddres==""){result= 1;}
return result;
}
String _stringWithoutCharWithIndex(String value, int index,int count)
  {
    String result="";
    for (int i=0; i <= value.length(); i++){ 
      if((i<index) ||(i>(index+count))){
        result+=value.charAt(i);
      }
    } 
    return result; 
  }


void _sendWebServerPage1(void)
{
_tspWebServer_client.println("Temperature:  ");
_tspWebServer_client.println(_dht1.temperature);
_tspWebServer_client.println("*C <br>");
_tspWebServer_client.println("Humidity:  ");
_tspWebServer_client.println(_dht1.humidity);
_tspWebServer_client.println(" %<br>");

}