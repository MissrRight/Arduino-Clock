/*-----( Import needed libraries )-----*/
#include <Wire.h>
#include <LCD.h> 
#include <LiquidCrystal.h>
#include "RTClib.h"
#include <SCoop.h>
#include <IRremote.h>
/********************设置引脚********************/
#define RECV_PIN  10   //红外线接收器OUTPUT端接在pin 10
#define BEE_PIN 9    //蜂鸣器
#define ARDUINO_PIN  8 //外部连接的另一块arduino板
/********************设置遥控器的按键********************/
#define cancel 0x1FE50AF  //返回
#define OK 0x1FE10EF      //OK
#define clock_time 0x1FE30CF  //暂停
#define s_menu 0x1FEB04F //静音
#define zero 0x1FE807F  //A
#define one 0x1FE40BF   //B
#define two 0x1FEC03F   //C
#define three 0x1FE20DF //D
#define four 0x1FEA05F  //↑
#define five 0x1FE609F  //E
#define six 0x1FEE01F   //←
#define seven 0x1FE906F  // →
#define eight 0x1FED827  //↓
#define nine 0x1FEF807   //＋

//函数声明
String show_s(int a,int b,int c,int d);
void menu();
char set_clock();
char set_clock_hour();
char set_clock_minute();
char set_clock_second();
char input_show(int x);
void formatPrint(int hour, int minute, int second);

IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
RTC_Millis rtc;   // Create a RealTimeClock object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // initialize the library with the numbers of the interface pins


//String time_list[] = ["Year","Month","Day","Hour","Minute","Second"];
char y[4];  //存储year的数值
int i=0;//循环计数
/**闹钟时间**/
int ch = 00;   //hour
int cmi = 00;  //minute
int cs = 00;  //second
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
boolean show_flag = true;    //true是显示，false是不显示
boolean switch_flag = true;    //true是开，false是关
boolean touch_state = false ;  //true是触摸，false是未触摸

/*-----( Declare objects )-----*/
defineTask(Task1) 
void Task1::setup() { 
  Serial.begin(9600);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  irrecv.enableIRIn(); // 启动红外解码
  } 

void Task1::loop() { 
  while(1){
    irrecv.resume();
    Serial.println("task_while");
    delay(1000);
    if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
      Serial.println("task_break");
  //    Serial.println(results.value,HEX);  //调试语句
      break;
    }
  }
    switch(results.value){
      case s_menu:
        Serial.println("s_menu");
        show_flag = !show_flag;
        Serial.print("show_flag1:");
        Serial.println(show_flag);
        menu();
        show_flag = true;
        Serial.print("show_flag2:");
        Serial.println(show_flag);
        irrecv.resume(); // 接收下一个值
        break;
      case clock_time: //显示设定的闹钟时间
        show_flag = !show_flag;
        Serial.print("show_flag3:");
        Serial.println(show_flag);
        lcd.clear();
        lcd.noBlink();
        lcd.setCursor(3,0);
        lcd.print("Alarm Clock"); 
        formatPrint(ch, cmi, cs);
        while(1){
          irrecv.resume();
          Serial.println("alarm_while");
          delay(1000);
            if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
              Serial.println("alarm_break");
          //    Serial.println(results.value,HEX);   //调试语句
              show_flag = true;
              Serial.print("show_flag4:");
              Serial.println(show_flag);
              break;
            }
        }
        Serial.println("switch_break");    
        break;
    }
}

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600); // Set up for Serial Monitor to be able to see this work
//  mySerial_arduino.begin(115200);
  /*----( These lines allow code to work also on Arduino DUE )----- */

  // Put these "//" in front of the line you do NOT want to use
  // following line sets the RTC to the date & time this sketch was compiled
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // May 21, 2015 at 6pm you would call: (use 24 hour time)
  #ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  pinMode(BEE_PIN,OUTPUT); 
  pinMode(ARDUINO_PIN,INPUT);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__))); // Start the RTC library code
  lcd.begin (16,2); // for 16 x 2 LCD module 
  lcd.setBacklightPin(3,POSITIVE); 
  lcd.setBacklight(HIGH); 
  mySCoop.start(); 
}

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{ 
  DateTime now = rtc.now();  // Read data from the RTC Chip
  switch(show_flag){
    case 1:{
      lcd.noBlink();
      lcd.setCursor(1,0);
      lcd.print(show_s(now.year(),now.month(),now.day(),1)+' ' +daysOfTheWeek[now.dayOfTheWeek()]); 
      lcd.setCursor (4,1); // go to start of 2nd line 
      if(switch_flag == 1)
        lcd.print(String(show_s(now.hour(),now.minute(),now.second(),0)+"   o")); 
      else
        lcd.print(String(show_s(now.hour(),now.minute(),now.second(),0)+"   x"));   
      break;
      }
    case -1:{
      break;
      }
  }
  Serial.println(digitalRead(ARDUINO_PIN));
  if(digitalRead(ARDUINO_PIN) == 1){
      touch_state = 1;
      Serial.print( "touch_state2:");
      Serial.println( touch_state);
      switch_flag = !switch_flag;
      digitalWrite(BEE_PIN,LOW);
    }
  else{
    touch_state = 0;
  }
  
  if(now.hour() == ch && now.minute() == cmi && now.second() == cs && switch_flag == 1){
      Serial.print("get_in");   //莫名编译错误
      digitalWrite(BEE_PIN,HIGH); 
    }
  yield();  //本函数必须放在主线的loop循环体中 
}//--(end main loop )---


//显示日期和时间的字符拼合及补零函数
String show_s(int a,int b,int c,int d){
 String s1,s2,s3;
  if(a<10)
    s1 = String('0'+String(a));
  else
    s1 = String(a);
  if(b<10)
    s2 = String('0'+String(b));
  else
    s2 = String(b);
  if(c<10)
    s3 = String('0'+String(c));
  else
    s3 = String(c);
  if(d==1)
    return String(s1+'/'+s2+'/'+s3);
  else
    return String(s1+':'+s2+':'+s3);
  }

//菜单函数
void menu(){
  rmenu:
  char m1,m2;
  lcd.clear();
  Serial.println("lcd.clear");
  lcd.noBlink();
//  Serial.println("lcd.noBlink");   //调试语句
  lcd.setCursor(0,0);
  lcd.print("Set_Clock？"); 
  while(1){
    irrecv.resume();
    Serial.println("while");
    delay(1000);
    if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
      Serial.println("break");
      Serial.println(results.value,HEX);
      break;
    }
  }
      if(results.value==OK){
        Serial.println("receive_OK");
        lcd.clear();
        lcd.home();
        lcd.print("<<<<Set_Clock>>>>"); 
     //   Serial.println("lcd_print_setclock");   //调试语句
        m2 = set_clock();
        Serial.print("m2:");
        Serial.println(m2);
        if(m2 == '#')
          goto rmenu;
      }
      else {
        show_flag = !show_flag; 
        Serial.println("not_rve_OK");
      }
  lcd.clear();
  }



/**---设置闹钟时间---**/
char set_clock(){
char c1,c2,c3;
schour:
  c1 = set_clock_hour();
  if(c1 == '#')
    return '#';
  Serial.print("c1:");
  Serial.println(c1);
scminute:
  c2 = set_clock_minute();
  if(c2 == '#')
    goto schour;
  Serial.print("c2:");
  Serial.println(c2);
  c3 = set_clock_second();
  if(c3 == '#')
    goto scminute;
  Serial.print("c3:");
  Serial.println(c3);
  lcd.clear();
  lcd.setCursor (1,0);
  lcd.print("Press any key!"); 
  lcd.setCursor (3,1);
  lcd.print("Success!"); 
  while(1){
    irrecv.resume();
    Serial.println("while");
    delay(1000);
    if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
      Serial.println("break");
      Serial.println(results.value,HEX);
      break;
    }
  }
  show_flag = true;
}

char set_clock_hour(){
   char k1;
   lcd.clear();
   lcd.home();
   lcd.print("<<<<Set_Clock>>>"); 
   lcd.setCursor (2,1);
   lcd.blink(); 
   lcd.print("Hour:"); 
   for(i=0;i<2;i++){
    cHour:
     y[i] = input_show(i+7);
     Serial.print("y[");
     Serial.print(i);
     Serial.println("]");
     Serial.println(y[i]);
     if(y[i]=='*')
        i = i-2;
     else if(y[i]=='#')
        return '#';
     if(i == 1){
      int out = 1;
      Serial.print("out");
      Serial.println(out);
      while(out){
        while(1){
          irrecv.resume();
          Serial.println("set_hour_while");
          delay(1000);
          if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
            Serial.println("set_hour_break");
//            Serial.println(results.value,HEX);   //调试语句
            break;
          }
        }
        Serial.println(results.value,HEX);
        switch(results.value){
          case OK:
            ch = (int(y[0])-48)*10+(int(y[1])-48);
            out = 0;
            Serial.println("hour_ok_break");
            Serial.print("ch");
            Serial.println(ch);
            break;
          case cancel:
            lcd.setCursor(i+7,1);
            lcd.print(" ");
            lcd.setCursor(i+7,1);
            goto cHour;
            break;
        }
        Serial.println("hour_switch_break");
      }
      Serial.println("hour_out_break");
    }
    Serial.println("hour_if i==1_break");
  }
  
  Serial.println("hour_for_out");
    if(ch > 24 || ch < 0){
      Serial.print("error");
      lcd.clear();
      lcd.setCursor (5,1);
      lcd.print("ERROR!");
      while(1){
        irrecv.resume();
        Serial.println("set_hour_error_while");
        delay(1000);
          if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
            Serial.println("set_hour_error_break");
         //   Serial.println(results.value,HEX);   //调试语句
            break;
          }
        }
      set_clock_hour();
   }  
   else{
    return ch;
   }
}

char set_clock_minute(){
   char k1;
   lcd.clear();
   lcd.home();
   lcd.print("<<<<Set_Clock>>>"); 
   lcd.setCursor (0,1);
   lcd.blink(); 
   lcd.print("Minute:"); 
   for(i=0;i<2;i++){
    cMinute:
     y[i] = input_show(i+7);
     if(y[i]=='*')
        i = i-2;
     else if(y[i]=='#')
        return '#';
     if(i == 1){
        int out = 1;
        while(out){
          while(1){
            irrecv.resume();
            Serial.println("set_minute_while");
            delay(1000);
            if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
              Serial.println("set_minute_break");
//            Serial.println(results.value,HEX);   //调试语句
              break;
            }
          }
          switch(results.value){
            case OK:
              cmi = (int(y[0])-48)*10+(int(y[1])-48);
              out = 0;
              Serial.println("minute_ok_break");
              Serial.print("cmi");
              Serial.println(cmi);
              break;
            case cancel:
              lcd.setCursor(i+7,1);
              lcd.print(" ");
              lcd.setCursor(i+7,1);
              goto cMinute;
              break;
          }
        }
      }
    }
    if(cmi > 60 || cmi < 0){
     Serial.print("error");
     lcd.clear();
     lcd.setCursor (5,1);
     lcd.print("ERROR!");
     while(1){
        irrecv.resume();
        Serial.println("set_minute_error_while");
        delay(1000);
          if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
            Serial.println("set_minute_error_break");
         //   Serial.println(results.value,HEX);    //调试语句
            break;
          }
      }
      set_clock_minute();
   }
   else{
    return cmi;
   }  
}

char set_clock_second(){
   char k1;
   lcd.clear();
   lcd.home();
   lcd.print("<<<<Set_Clock>>>"); 
   lcd.setCursor (0,1);
   lcd.blink(); 
   lcd.print("Second:"); 
   for(i=0;i<2;i++){
    cSecond:
     y[i] = input_show(i+7);
     if(y[i]=='*')
        i = i-2;
     else if(y[i]=='#')
        return '#';
     if(i == 1){
       int out = 1;
        while(out){
          while(1){
            irrecv.resume();
            Serial.println("set_second_while");
            delay(1000);
              if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
                Serial.println("set_second_break");
//              Serial.println(results.value,HEX);   //调试语句
                break;
              }
          }
          switch(results.value){
            case OK:
              out = 0;
              cs = (int(y[0])-48)*10+(int(y[1])-48);
              Serial.println("second_ok_break");
              Serial.print("cs");
              Serial.println(cs);
              break;
            case cancel:
              lcd.setCursor(i+7,1);
              lcd.print(" ");
              lcd.setCursor(i+7,1);
              goto cSecond;
              break;
          }
      }
    }
  }
    if(cs > 60 || cs < 0){
     Serial.print("error");
     lcd.clear();
     lcd.setCursor (5,1);
     lcd.print("ERROR!");
     while(1){
        irrecv.resume();
        Serial.println("set_second_error_while");
        delay(1000);
        if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
            Serial.println("set_second_error_break");
         //   Serial.println(results.value,HEX);   //调试语句
            break;
          }
        }
      set_clock_second();
   }
   else{
    return cs;
   }  
}
/**---设置闹钟时间结束---**/

char input_show(int x){
  while(1){
    irrecv.resume();
    Serial.println("input_show_while");
    delay(1000);
    if(irrecv.decode(&results) && results.value!= 4294967295){  //接收到遥控器的红外信号（排除最后自带的FFFFF）
      Serial.println("input_show_break");
//      Serial.println(results.value,HEX);   //调试语句
      break;
    }
  }
    switch(results.value){
      case cancel:
        if(x-1>6){
          lcd.setCursor(x-1,1);
          lcd.print(" ");
          lcd.setCursor(x-1,1);
          return '*';
        }else{ 
            return '#';
        }
        break;
      case zero:
        lcd.setCursor(x,1);
        lcd.print('0');
        return '0';
        break;
      case one:
        lcd.setCursor(x,1);
        lcd.print('1');
        Serial.println(i);
        return '1';
        break;
      case two:
        lcd.setCursor(x,1);
        lcd.print('2');
        Serial.println(i);
        return '2';
        break;
      case three:
        lcd.setCursor(x,1);
        lcd.print('3');
        Serial.println(i);
        return '3';
        break;
      case four:
        lcd.setCursor(x,1);
        lcd.print('4');
        Serial.println(i);
        return '4';
        break;
      case five:
        lcd.setCursor(x,1);
        lcd.print('5');
        Serial.println(i);
        return '5';
        break;
      case six:
        lcd.setCursor(x,1);
        lcd.print('6');
        Serial.println(i);
        return '6';
        break;
      case seven:
        lcd.setCursor(x,1);
        lcd.print('7');
        Serial.println(i);
        return '7';
        break;
      case eight:
        lcd.setCursor(x,1);
        lcd.print('8');
        Serial.println(i);
        return '8';
        break;
      case nine:
        lcd.setCursor(x,1);
        lcd.print('9');
        Serial.println(i);
        return '9';
        break;
      default: 
        i = i-1;
        Serial.println(i);
    }
}


  void formatPrint(  int hour, int minute, int second){
     lcd.noBlink();
     lcd.setCursor (4,1); // go to start of 2nd line 
     if(hour<10){
      lcd.print(0);
      lcd.setCursor (5,1); // go to start of 2nd line 
      lcd.print(hour);
     }else
      lcd.print(hour);
     lcd.setCursor(6,1);
     lcd.print(":");
     lcd.setCursor(7,1);
     if(minute<10){
      lcd.print(0);
      lcd.setCursor (8,1); // go to start of 2nd line 
      lcd.print(minute);
     }else
      lcd.print(minute);
       lcd.setCursor(9,1);
       lcd.print(":");
       lcd.setCursor(10,1);
     if(second<10){
      lcd.print(0);
      lcd.setCursor(11,1); // go to start of 2nd line 
      lcd.print(second);
     }else
      lcd.print(second);
}
