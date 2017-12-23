#include <Wire.h>                                  //调用库 
#include <SHT2x.h> 
#include "./ESP8266.h"
#include <Arduino.h>


#define TOUCH_PIN 6
#define ARDUINO_PIN 10
#define TEM_PIN A0

#define SSID           "Miss.Right"                   // cannot be longer than 32 characters!
#define PASSWORD       "12456789"
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "20426624"
#define PROJECTID "104141"
#define HOST_PORT   (80)
String apiKey="1RPD03nLPWrEVYbhC=r994bFctE=";
char buf[10];

String mCottenData;
String jsonToSend;
String postString; 
float sensor_tem;
char  sensor_tem_c[7];

#include <SoftwareSerial.h>
SoftwareSerial mySerial_wifi(2, 3); /* RX:(插到D3）, TX:D2 */
ESP8266 wifi(mySerial_wifi);

void setup(void)     //初始化函数  
{       
  //初始化串口波特率  
    Wire.begin();
    Serial.begin(9600); 
//    mySerial_arduino.begin(115200);  
    while(!Serial);
    pinMode(TOUCH_PIN, INPUT);
    pinMode(ARDUINO_PIN, OUTPUT);

   //ESP8266初始化
    Serial.print("setup begin\r\n");   

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {      //加入无线网
    Serial.print("Join AP success\r\n");  
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }
  Serial.print("setup end\r\n");  
}
void loop(void)     //循环函数  
{   
  boolean touch_state = 0;   //0是未触摸，1是触摸
  touch_state = get_touch();
  Serial.println(touch_state); 
  if (touch_state==1)                  //发送数据时间间隔
  { 
    digitalWrite(ARDUINO_PIN,HIGH);
//    Serial.println("TOUCH");    //调试语句
//    Serial.println("arduino"); 
    sensor_tem = analogRead(TEM_PIN) ;
//    Serial.println("get tem");    //调试语句
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);
//    Serial.println("dtostrf");    //调试语句
    Serial.println(touch_state);   
    Serial.println(sensor_tem);             
    updateSensorData();                //将数据上传到服务器的函数  
  }
  else{
    digitalWrite(ARDUINO_PIN,LOW);
 //   Serial.println(touch_state); 
  }   
}

boolean get_touch(){
  boolean touch_state; 
  touch_state=digitalRead(TOUCH_PIN);
  return touch_state;
}

void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

jsonToSend="{\"Temperature\":";
    dtostrf(sensor_tem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";

    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
  
}
