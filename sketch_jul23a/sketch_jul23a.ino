#include <WiFi.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include "HX711.h"

#define ARRAYSIZE 6
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
#define calibration_factor 4500.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 13;
const int LOADCELL_SCK_PIN = 14;
#define A1 32
#define A2 33
#define A3 34
#define A4 35
#define A5 36
#define THR 1500
#define buzzerPin 25

const int RMotor1 = 27;
const int RMotor2 = 15;
const int LMotor1 = 26;
const int LMotor2 = 12;
byte speedCar = 200;

HX711 scale;
HardwareSerial BCSerial(2);
HTTPClient http;
String BCdataIn,BCdataDel;
String results[ARRAYSIZE]="0";
int i=0,c1=0,c2=0,c3=0,c4=0,m=0,pnum,f=0,h=0,l=0;
String BClcd[ARRAYSIZE];
String tok1[4]=" ",tok2[6]=" ",tok3[6]=" ",tok4[6]=" ";
bool back=false,next=false;
int weight=0,del_index;
int reading1, reading2, reading3, reading4, reading5;
int arrsize2,flag=0;
int buzzerFlag=0;
long timer=0;

String bcodes[5]={"TL002733","TL000100","TL002555","TL000010","TL000225"};
String pname[5]={"pringles","KitKat","Mars","Lays","Tigers"};
float price[5]={10.0,24.0,24.0,3.5,2.0};
const char* ssid = "Ayoo"; //NetworksLab
const char* password = "ayaqartallo"; //Networks123456
String HOST_NAME = "https://192.168.43.228"; // REPLACE WITH YOUR PC's IP ADDRESS
String PHP_FILE_NAME = "/search.php";

int arrsize=(int)sizeof(bcodes)/sizeof(bcodes[0]);

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 16 column and 2 rows

void page0(String n, float p)
{
    lcd.setCursor(0,0);
    lcd.print("Product:  Price:");
  if(c1==0){ //to print in the second line
    lcd.setCursor(0,1);
    lcd.print(n);
    lcd.setCursor(10,1);
    lcd.print(String(p)+"NIS");
  }else{//to print in the third line
    lcd.setCursor(0,2);
    lcd.print(n);
    lcd.setCursor(10,2);
    lcd.print(String(p)+"NIS");
  }
  lcd.setCursor(17,3);
  lcd.print("1/4");
  pnum=0;
  c1++;
}

void page1(String n, float p)
{
  if(c2==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(n);
    lcd.setCursor(10,0);
    lcd.print(String(p)+"NIS");
  }else if(c2==1){
    lcd.setCursor(0,1);
    lcd.print(n);
    lcd.setCursor(10,1);
    lcd.print(String(p)+"NIS");
  }else{
    lcd.setCursor(0,2);
    lcd.print(n);
    lcd.setCursor(10,2);
    lcd.print(String(p)+"NIS");
   }  
   lcd.setCursor(17,3);
  lcd.print("2/4");
  pnum=1;
   c2++;
}

/*void page2(String n, float p)
{
  if(c3==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(n);
    lcd.setCursor(10,0);
    lcd.print(String(p)+"NIS");
  }else if(c3==1){
    lcd.setCursor(0,1);
    lcd.print(n);
    lcd.setCursor(10,1);
    lcd.print(String(p)+"NIS");
  }else{
    lcd.setCursor(0,2);
    lcd.print(n);
    lcd.setCursor(10,2);
    lcd.print(String(p)+"NIS");
   }
  lcd.setCursor(17,3);
  lcd.print("3/4");
  pnum=2;
  c3++;
}

void page3(String n, float p)
{
  if(c4==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(n);
    lcd.setCursor(10,0);
    lcd.print(String(p)+"NIS");
  }else if(c4==1){
    lcd.setCursor(0,1);
    lcd.print(n);
    lcd.setCursor(10,1);
    lcd.print(String(p)+"NIS");
  }else{
    lcd.setCursor(0,2);
    lcd.print(n);
    lcd.setCursor(10,2);
    lcd.print(String(p)+"NIS");
   }
   lcd.setCursor(17,3);
   lcd.print("4/4");
   pnum=3;
  c4++;
}
*/

void printDescription(String dta){
  for(int g=0;g<arrsize;g++){
       bcodes[g].trim();
       dta.trim();
      if(bcodes[g].equals(dta)){
        if(c1==0||c1==1){ // c1 counter for page1 which contain 2 lines for print
          tok1[f]=pname[g];//stored name to print(retrieve) it when i press next or back
          f++;
          tok1[f]=price[g];//stored price to print(retrieve) it when i press next or back
          f++;
          page0(pname[g],price[g]); //print in page 1
          }
          else if(c1>1&&c2<3){// c2 counter for page2 which contain 3 lines for print
            tok2[f]=pname[g];
            f++;
            tok2[f]=price[g];
            f++;
            page1(pname[g],price[g]);//print in page 2
            }
            /*else if(c1>1&&c2>2&&c3<3){// c3 counter for page3 which contain 3 lines for print
               tok3[f]=pname[g];
                f++;
                tok3[f]=price[g];
                f++;
              page2(pname[g],price[g]);//print in page 3
            }else{
                 tok4[f]=pname[g];
                  f++;
                  tok4[f]=price[g];
                  f++;
                page3(pname[g],price[g]);//print in page 4
              }*/
      }
    }

}

void deleteItems(){
   results[del_index]=" ";
   Serial.println("Item deleted");
  /*for(int t=0;t<arrsize2;t++){
    Serial.print(results[t]);
    printDescription(results[t]);
  }*/
  lcd.setCursor(0,2);
  lcd.print("                    ");
}
void clearArray(){
  for(int s=0;s<arrsize;s++){
    results[s]=" "; 
  }
}
void sendToServer(){
  for(int q=0;q<ARRAYSIZE;q++){
      results[q].trim();
  }
  String server = HOST_NAME + PHP_FILE_NAME +"?idcart=1"+"&barcode1="+results[1]+"&barcode2="+results[2]+"&barcode3="+results[3]+"&barcode4="+results[4]+"&barcode5="+results[5]+"&weight="+weight;
  http.begin(server); 
  Serial.println(server);
  int httpCode = http.GET();

  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
     
    } else {
      Serial.printf("HTTP GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("HTTP GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
}

void goForword() {
      buzzerFlag=1;
    timer=millis();
  digitalWrite(buzzerPin, 0);
  ledcWrite(4, 0);
  ledcWrite(3, speedCar);
  ledcWrite(6, speedCar);
  ledcWrite(5, 0);
}

void goBack() {
  digitalWrite(buzzerPin, 0);
  ledcWrite(3, speedCar);
  ledcWrite(4, 0);
  ledcWrite(5, 0);
  ledcWrite(6, speedCar);

}

void goRight() {
  digitalWrite(buzzerPin, 0);
  ledcWrite(4, 0);
  ledcWrite(3, speedCar);
  ledcWrite(6, 0);
  ledcWrite(5, speedCar);
}

void goLeft() {
  digitalWrite(buzzerPin, 0);
  ledcWrite(4, speedCar);
  ledcWrite(3, 0);
  ledcWrite(6, speedCar);
  ledcWrite(5, 0);
}



void stopRobot() {
  ledcWrite(3, 0);
  ledcWrite(4, 0);
  ledcWrite(5, 0);
  ledcWrite(6, 0);
}

void initMotors()
{
  /* Configuring motor PWM functionalities to generate the signal */
  /* ledcSetup(Channel, Frequency, Resolution) */
  ledcSetup(3, 2000, 8); /* 2000 hz PWM, 8-bit resolution and range from 0 to 255 */
  ledcSetup(4, 2000, 8); /* 2000 hz PWM, 8-bit resolution and range from 0 to 255 */
  ledcSetup(5, 2000, 8); /* 2000 hz PWM, 8-bit resolution and range from 0 to 255 */
  ledcSetup(6, 2000, 8); /* 2000 hz PWM, 8-bit resolution and range from 0 to 255 */

  /* Attaching the channel to the GPIO to be controlled */
  /* ledcAttachPin(GPIO, Channel) */
  ledcAttachPin(RMotor1, 3);
  ledcAttachPin(RMotor2, 4);
  ledcAttachPin(LMotor1, 5);
  ledcAttachPin(LMotor2, 6);

}

void setup() {
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  lcd.backlight();
  BCSerial.begin(115200, SERIAL_8N1, 16, 17); // the GPRS/GSM baud rate
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  initMotors();
  pinMode(buzzerPin, OUTPUT);
 
 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
 
    char key = keypad.getKey();
  if (key){
      if(key=='7'){
        flag=1;
      }
      if(key=='3'){
        digitalWrite(buzzerPin, 0);
        stopRobot();
    Serial.println("stopRobot");
       lcd.setCursor(0,0);
  lcd.print("Product:  Price:");
  lcd.setCursor(17,3);
  lcd.print("1/4");
  if(BCSerial.available()>0){//read the barcode
    BCdataIn=BCSerial.readString();
    //Serial.println(BCdataIn);
    results[i]=BCdataIn;
    results[i].trim();
    
      Serial.println(results[i]); 
      printDescription(BCdataIn); //from index=0 start print barcodes in LCD
    //Serial.print(results[i]);
    
    i++;
    }
    if (scale.is_ready()) {
       
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    weight=round(scale.get_units(5) * 10);
    Serial.println(weight);
  } else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
        flag=0;
      }
      if(key=='6'){
        WiFi.begin(ssid, password);
         while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  lcd.setCursor(0,0);
  lcd.print("Product:  Price:");
  lcd.setCursor(17,3);
  lcd.print("1/4");
      }
    Serial.println(key);
  }
    if(flag==1){
        reading1 = analogRead(A1);
  reading2 = analogRead(A2);
  reading3 = analogRead(A3);
  reading4 = analogRead(A4);
  reading5 = analogRead(A5);
  Serial.println("reading1: " + String(reading1) + " reading2: " + String(reading2) + " reading3: " + String(reading3) + " reading4: " + String(reading4) + " reading5: " + String(reading5));

  if (reading1 < THR && reading2 < THR && reading3 < THR && reading5 < THR && reading5 < THR) {
    stopRobot();
    if(buzzerFlag==1){
      if(millis()-timer>2000){
    digitalWrite(buzzerPin, 1);
    //Serial.println("stopRobot");
    Serial.println("signal lost>>buzzer on");
    
      }
    }
  }




    if (reading3 > 4090 ) {
    stopRobot();
    Serial.println("stopRobot");
    buzzerFlag=1;
    timer=millis();
  }
    if (reading3 > 3000 && reading4 < 1000 && reading2 < 1000) {
    stopRobot();
    Serial.println("stopRobot");
  }
  else if (reading3 > 3000 && reading4 > 3000 && reading2 > 3000) {

    stopRobot();
    Serial.println("stopRobot");
  }
  else if (reading3 > 3000 && reading4 < 2000 && reading2 < 2000) {

    speedCar = 150;
    goForword();
    Serial.println("goForword150");
  }
  else if (reading3 > 1000 && reading4 < 1000 && reading2 < 1000) {

    speedCar = 150;
    goForword();
    Serial.println("goForword150");
  }

  else if ((reading3 > 3500) && (reading3 - 1000 > reading4  && reading3 - 1000 > reading2)) {

    speedCar = 150;
    goForword();
    Serial.println("goForword150");
  }

  else if ((reading3 > 3500) && (reading3 - 1000 > reading4  && reading3 - 1000 > reading2)) {

    speedCar = 150;
    goForword();
    Serial.println("goForword150");
  }









  else if (reading3 > 3500 && reading4 > 3000 && reading2 < 3000) {

    speedCar = 150;
    goLeft();
    Serial.println("goLeft150");
  }
  else if (reading3 > 3500 && reading4 < 3000 && reading2 > 3000) {

    speedCar = 150;
    goRight();
    Serial.println("goRight150");
  }
  else if (reading1 > 3500) {
    speedCar = 250;
    goRight();
    Serial.println("goRight250");
  }

  else if (reading5 > 3500) {
    speedCar = 250;
    goLeft();
    Serial.println("goLeft250");
  }







  else if ((reading4 > 1000 || reading5 > 1000 ) && reading4 - 200 > reading3 || reading5 - 200 > reading3) {

    speedCar = 250;
    goLeft();
    Serial.println("goLeft250");
  }

  else if ((reading1 > 1000 || reading2 > 1000 ) && reading1 - 200 > reading3 || reading2 - 200 > reading3) {

    speedCar = 250;
    goRight();
    Serial.println("goRight250");
  }








  else {

  }
      }
    if(flag==0){
      if(key=='8'){ //confirm
        sendToServer();
      }
      if(key=='9'){//delete
        arrsize2=(int)sizeof(results)/sizeof(results[0]);
          BCdataDel=BCdataIn;
          Serial.println(BCdataIn);
          del_index=i-1;
         deleteItems();
      }
      if(key=='5'){//disconnect
        delay(10);
        lcd.clear();
        for(int w=0;w<ARRAYSIZE;w++){
          results[w]=" ";
        }
        for(int w=0;w<ARRAYSIZE;w++){
          Serial.println(results[w]);
        }
        http.end();
        Serial.println("WiFi Disconnected..");
      }

      }
}
