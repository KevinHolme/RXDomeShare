


#include <SPI.h>
#include <RH_RF69.h>
#include <Wire.h>
#include <RHReliableDatagram.h>
#define LED 13
#include <Adafruit_PWMServoDriver.h>
//#include <EasyTransferI2C.h>




// called this way, it uses the default address 0x40

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
//uint8_t servonum = 0;




//----------------------------------------Time--------------
unsigned long CurrentMillis = 0;
unsigned long PreviousDisplayMillis = 0;
unsigned long PreviousReadMillis = 0;
unsigned long PreviousSendMillis = 0;
unsigned long PreviousShowTimeMillis = 0;
unsigned long PreviousHistoryMillis = 0;
unsigned long PreviousRadioMillis = 0;
unsigned long PreviousCheckMillis = 0;
unsigned long PreviousStanceMillis = 0;
unsigned long PreviousMoveMillis = 0;
unsigned long PreviousShowMillis = 0;
unsigned long PreviousLoopMillis = 0;
unsigned long ShowTime = 1;
const long DisplayInterval = 1000;
const long ReadInterval = 100;
const long SendInterval = 200; 
const long ResetHistoryInterval = 250; 
const long RadioInterval = 250; 
const long CheckInterval = 500; 
const long StanceInterval = 100; 
const long MoveInterval = 75;
const long LoopInterval = 100;
const long ShowInterval = 250; 
const long ShowTimeInterval = 200; 
/************ Radio Setup ***************/


  #define RF69_FREQ 919.0
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
// Where to send packets to!
#define DEST_ADDRESS 255

// who am i? (server address)
#define MY_ADDRESS 6

struct dataStruct{
byte n1;
byte n2;
byte n3;

}RadioPacket;

 byte buf[sizeof(RadioPacket)] = {0};
 
// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);
//-----------------------------Constant (Never Change)---------------------------


#define PANELCOUNT 16
#define I2CADDRESS 6
#define SERVOI2CADDRESS 0x44
#define OEPin 12   



const int Pie1 = 0;
const int Pie2 = 1;
const int Pie3 = 2;
const int Pie4 = 3;
const int One = 4;
const int Two = 5;
const int Three = 6;
const int Four = 7;
const int Seven = 8;
const int Ten = 9;
const int Eleven = 10;
const int Thirteen = 11;
const int Blank2 = 12;
const int CPU = 13;
const int Zap = 14;
const int CPUTip = 15;

const int Pie1C = 445;
const int Pie1O = 300;
const int Pie1M = 375;
const int Pie2C = 440;
const int Pie2O = 290;
const int Pie2M = 360;
const int Pie3C = 450;
const int Pie3O = 320;
const int Pie3M = 390;
const int Pie4C = 455;
const int Pie4O = 280;
const int Pie4M = 360;
const int OneC = 450;
const int OneO = 270;
const int OneM = 350;
const int TwoC = 260;
const int TwoO = 510;
const int TwoM = 390;
const int ThreeC = 485;
const int ThreeO = 250;
const int ThreeM = 375;
const int FourO = 280;
const int FourC = 450;
const int FourM = 360;
const int SevenO = 250;
const int SevenC = 470;
const int SevenM = 375;
const int TenC = 440;
const int TenO = 280;
const int TenM = 350;
const int ElevenC = 430;
const int ElevenO = 260;
const int ElevenM = 360;
const int ThirteenC = 460;
const int ThirteenO = 250;
const int ThirteenM = 350;
const int CPUAirIn = 450;
const int CPUAirOut = 340;
const int ZapAirIn = 320;
const int ZapAirOut = 430;
const int CPUExtOut = 200;
const int CPUExtIn = 500;
//============================== variable (will Change)==============
String Recieved;
String Incoming1;
String Incoming2;
String Incoming3;
String IncomingMsg;
int IncomingInt;
int LastNum;
int CurrentNumber;
int CurrentShow;
int Num3Int;
int CPUOut = 0;
int CPUWorking = 0;
int ZapOut = 0;
//------------------------------------------------------------------------------------------------
//==========================================================Setup=============================



void setup(){
  delay(500);
  Serial.begin(115200);
 pinMode(8, INPUT_PULLUP);
 pwm.begin();  
 pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 delay(10);
// Wire.begin();

pinMode( OEPin, OUTPUT );   
digitalWrite(OEPin, HIGH);  

//==================================================Radio==================================     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
//  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
RadioPacket.n1 = 0;
RadioPacket.n2 = 0;
RadioPacket.n3 = 0;

  delay(500);
digitalWrite(OEPin, LOW); 
delay(100);
 pwm.setPWM(0, 0, Pie1C); 
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(10, 0, ElevenC); 
 pwm.setPWM(11, 0, ThirteenC);
 delay(300);
digitalWrite(OEPin, HIGH);  




  
}
//---------------------------------------------------Wave 1-------------------------
//counterClockwise around till all open, Then Closed in same order
void Wave1(){ 
 
    if (ShowTime >= 1 && ShowTime <= 29) {
     digitalWrite(OEPin, LOW);
  } 
if (ShowTime >= 1 && ShowTime <= 14) { 
  pwm.setPWM(0, 0, Pie1O);  
}
if (ShowTime >= 2 && ShowTime <= 14) { 
  pwm.setPWM(1, 0, Pie2O);
}
if (ShowTime >= 3 && ShowTime <= 14) { 
  pwm.setPWM(2, 0, Pie3O);
}
if (ShowTime >= 4 && ShowTime <= 14) { 
   pwm.setPWM(3, 0, Pie4O);
   }
if (ShowTime >= 5 && ShowTime <= 14) { 
   pwm.setPWM(4, 0, OneO); 
}
if (ShowTime >= 6 && ShowTime <= 14) { 
  pwm.setPWM(5, 0, TwoO);  
}
if (ShowTime >= 7 && ShowTime <= 14) { 
 pwm.setPWM(6, 0, ThreeO);
}
if (ShowTime >= 8 && ShowTime <= 14) { 
   pwm.setPWM(7, 0, FourO);
     }
if (ShowTime >= 9 && ShowTime <= 14) { 
 pwm.setPWM(8, 0, SevenO); 
}
if (ShowTime >= 10 && ShowTime <= 14) { 
  pwm.setPWM(9, 0, TenO);  
}
if (ShowTime >= 11 && ShowTime <= 14) { 
  pwm.setPWM(10, 0, ElevenC);
}
if (ShowTime >= 12 && ShowTime <= 14) { 
  pwm.setPWM(5, 0, ThirteenC);
}
if (ShowTime >= 17) { 
   pwm.setPWM(0, 0, Pie1C);
}
if (ShowTime >= 18) { 
   pwm.setPWM(1, 0, Pie2C);
}
if (ShowTime >= 19) { 
  pwm.setPWM(2, 0, Pie3C); 
}
if (ShowTime >= 20) { 
 pwm.setPWM(3, 0, Pie4C);
}
if (ShowTime >= 21) { 
 pwm.setPWM(4, 0, OneC);
}
if (ShowTime >= 22) { 
 pwm.setPWM(5, 0, TwoC); 
}
if (ShowTime >= 23) { 
 pwm.setPWM(6, 0, ThreeC); 
}
if (ShowTime >= 24) { 
 pwm.setPWM(7, 0, FourC); 
}
if (ShowTime >= 25) { 
 pwm.setPWM(8, 0, SevenC); 
}
if (ShowTime >= 26) { 
 pwm.setPWM(9, 0, TenC); 
}
if (ShowTime >= 27) { 
 pwm.setPWM(10, 0, ElevenC); 
}
if (ShowTime >= 28) { 
 pwm.setPWM(11, 0, ThirteenC); 
}

if (ShowTime >= 30) {
  digitalWrite(OEPin, HIGH);  
  CurrentShow = 0;
  Shows();
} 
}
//---------------------------------------------------Wave 2-------------------------
//open from center out, and then closed in same order
void Wave2(){
  if (ShowTime >= 1 && ShowTime <= 28) {
digitalWrite(OEPin, LOW);
  } 
if (ShowTime >= 0 && ShowTime <= 14) { 
  pwm.setPWM(1 ,0, Pie2O);  
}
if (ShowTime >= 1 && ShowTime <= 14) { 
  pwm.setPWM(2 ,0, Pie3O);
}
if (ShowTime >= 3 && ShowTime <= 14) { 
  pwm.setPWM(11 ,0, ThirteenO);
}
if (ShowTime >= 4 && ShowTime <= 14) { 
   pwm.setPWM(4 ,0, OneO);
}
if (ShowTime >= 5 && ShowTime <= 14) { 
   pwm.setPWM(10 ,0, ElevenO); 
}
if (ShowTime >= 6 && ShowTime <= 14) { 
  pwm.setPWM(5 ,0, TwoO);  
 }
if (ShowTime >= 7 && ShowTime <= 14) { 
 pwm.setPWM(9 ,0, TenO);
}
if (ShowTime >= 8 && ShowTime <= 14) { 
 pwm.setPWM(6 ,0, ThreeO);
}
if (ShowTime >= 9 && ShowTime <= 14) { 
 pwm.setPWM(0 ,0, Pie1O); 
}
if (ShowTime >= 10 && ShowTime <= 14)  { 
 pwm.setPWM(3 ,0, Pie4O);     
}
if (ShowTime >= 11 && ShowTime <= 14)  { 
pwm.setPWM(7 ,0, FourO); 
}
if (ShowTime >= 12 && ShowTime <= 14)  { 
   pwm.setPWM(1 ,0, Pie2C);  
}
if (ShowTime >= 14 && ShowTime <= 27) { 
  pwm.setPWM(2 ,0, Pie3C);
}
if (ShowTime >= 15 && ShowTime <= 27) { 
   pwm.setPWM(11 ,0, ThirteenC);
}
if (ShowTime >= 16 && ShowTime <= 27) { 
   pwm.setPWM(4 ,0, OneC);
}
if (ShowTime >= 17 && ShowTime <= 27) { 
  pwm.setPWM(10 ,0, ElevenC); 
}
if (ShowTime >= 18 && ShowTime <= 27) { 
 pwm.setPWM(5 ,0, TwoC);  
}
if (ShowTime >= 19 && ShowTime <= 27) { 
 pwm.setPWM(9 ,0, TenC);
}
if (ShowTime >= 20 && ShowTime <= 27) {
 pwm.setPWM(6 ,0, ThreeC);
}
if (ShowTime >= 21 && ShowTime <= 27) {
 pwm.setPWM(0 ,0, Pie1C); 
}
if (ShowTime >= 22 && ShowTime <= 27) {
 pwm.setPWM(3 ,0, Pie4C);     
}
if (ShowTime >= 23 && ShowTime <= 27) {
pwm.setPWM(7 ,0, FourC); 
}
if (ShowTime >= 24 && ShowTime <= 27) {
 pwm.setPWM(0, 0, Pie1C);
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(10, 0, ElevenC); 
 pwm.setPWM(11, 0, ThirteenC); 
}

if (ShowTime >= 28) {
  digitalWrite(OEPin, HIGH);  
  CurrentShow = 0;
  Shows();
} 
}

//---------------------------------------------------Wave 3-------------------------
//All open, then close from center out
void Wave3(){
 if (ShowTime >= 1 && ShowTime <= 18) {
digitalWrite(OEPin, LOW);
  }
 pwm.setPWM(0, 0, Pie1O); 
 pwm.setPWM(1, 0, Pie2O);
 pwm.setPWM(2, 0, Pie3O);
 pwm.setPWM(3, 0, Pie4O); 
 pwm.setPWM(4, 0, OneO);
 pwm.setPWM(5, 0, TwoO); 
 pwm.setPWM(6, 0, ThreeO);
 pwm.setPWM(7, 0, FourO); 
 pwm.setPWM(8, 0, SevenO);
 pwm.setPWM(9, 0, TenO);
 pwm.setPWM(10, 0, ElevenO); 
 pwm.setPWM(11, 0, ThirteenO); 

if (ShowTime >= 3 && ShowTime <= 15)  { 
   pwm.setPWM(1 ,0, Pie2C);  
}
if (ShowTime >= 5 && ShowTime <= 15) { 
  pwm.setPWM(2 ,0, Pie3C);
}
if (ShowTime >= 6 && ShowTime <= 15) { 
   pwm.setPWM(11 ,0, ThirteenC);
}
if (ShowTime >= 7 && ShowTime <= 15) { 
   pwm.setPWM(4 ,0, OneC);
}
if (ShowTime >= 8 && ShowTime <= 15) { 
  pwm.setPWM(10 ,0, ElevenC); 
}
if (ShowTime >= 9 && ShowTime <= 15) { 
 pwm.setPWM(5 ,0, TwoC);  
}
if (ShowTime >= 10 && ShowTime <= 15) { 
 pwm.setPWM(9 ,0, TenC);
}
if (ShowTime >= 11 && ShowTime <= 15) {
 pwm.setPWM(6 ,0, ThreeC);
}
if (ShowTime >= 12 && ShowTime <= 15) {
 pwm.setPWM(0 ,0, Pie1C); 
}
if (ShowTime >= 13 && ShowTime <= 15) {
 pwm.setPWM(3 ,0, Pie4C);     
}
if (ShowTime >= 14 && ShowTime <= 15) {
pwm.setPWM(7 ,0, FourC); 
}
if (ShowTime >= 15 && ShowTime <= 16) {
pwm.setPWM(8 ,0, SevenC); 
}
if (ShowTime >= 16 && ShowTime <= 17) {
 pwm.setPWM(0, 0, Pie1C);
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(10, 0, ElevenC); 
 pwm.setPWM(11, 0, ThirteenC); 
}
if (ShowTime >= 17) {
  digitalWrite(OEPin, HIGH);  
  CurrentShow = 0;
  Shows();
} 
}
//---------------------------------------------------Wave 4-------------------------

void Wave4(){
  if (ShowTime <= 44) {
digitalWrite(OEPin, LOW);
  } 
if (ShowTime >= 3 && ShowTime <= 8) { 
  OddPanelsOpen();
}
if (ShowTime >= 8 && ShowTime <= 13) { 
  EvenPanelsOpen();
  OddPanelsClosed();
}
if (ShowTime >= 13 && ShowTime <= 18) { 
   EvenPanelsClosed();
   OddPanelsOpen();  
}
if (ShowTime >= 18 && ShowTime <= 23) { 
    EvenPanelsOpen();
    OddPanelsClosed();
}
if (ShowTime >= 23 && ShowTime <= 28) { 
   EvenPanelsClosed();
   OddPanelsOpen();  
}

if (ShowTime >= 28 && ShowTime <= 33) { 
    EvenPanelsOpen();
    OddPanelsClosed();
}
if (ShowTime >= 33 && ShowTime <= 38) { 
   EvenPanelsClosed();
  OddPanelsOpen();
}
if (ShowTime >= 38 && ShowTime <= 43) { 
   AllPanelsClosed(); 
}

if (ShowTime >= 44) {
  digitalWrite(OEPin, HIGH);  
  CurrentShow = 0;
  Shows();
} 
}
//---------------------------------------------------Scared-------------------------

void Scared(){
  if (ShowTime <= 17) {
digitalWrite(OEPin, LOW);
  } 
if (ShowTime >= 3 && ShowTime <= 4) { 
AllPanelsOpen();
}
if (ShowTime >= 4 && ShowTime <= 5) { 
AllPanelsMid();
}
if (ShowTime >= 5 && ShowTime <= 6) { 
AllPanelsOpen();
}
if (ShowTime >= 6 && ShowTime <= 7) { 
AllPanelsMid();
}
if (ShowTime >= 7 && ShowTime <= 8) { 
AllPanelsOpen();
}
if (ShowTime >= 8 && ShowTime <= 9) { 
AllPanelsMid();
}
if (ShowTime >= 9 && ShowTime <= 10) { 
AllPanelsOpen();
}
if (ShowTime >= 10 && ShowTime <= 11) { 
AllPanelsMid();
}
if (ShowTime >= 11 && ShowTime <= 12) { 
AllPanelsOpen();
}
if (ShowTime >= 12 && ShowTime <= 13) { 
AllPanelsMid();
}
if (ShowTime >= 13 && ShowTime <= 14) { 
AllPanelsOpen();
}
if (ShowTime >= 14 && ShowTime <= 18) { 
AllPanelsClosed();
}
if (ShowTime >= 18) {
  digitalWrite(OEPin, HIGH);  
  CurrentShow = 0;
  Shows();
} 
}


//_________________________________________________SHOWS________________________________
void Shows(){
  if (CurrentShow == 0){

  return;
}
switch(CurrentShow){
  case 52:// WAVE 1
  Serial.print(" Wave 1 ");
  Serial.println(ShowTime);
 Wave1();
break;
  case 53:// WAVE 3
  Serial.print(" Wave 3 ");
  Serial.println(ShowTime);
 Wave3();
break;
  case 54:// Scared
  Serial.print(" Scared ");
  Serial.println(ShowTime);
 Scared();
break;
  case 56:// WAVE 2
  Serial.print(" Wave 2 ");
  Serial.println(ShowTime);
 Wave2();
break;
  case 57:// WAVE 4
  Serial.print(" Wave 4 ");
  Serial.println(ShowTime);
 Wave4();
break;
  case 62:// WAVE 1
  Serial.print(" Wave 1 ");
  Serial.println(ShowTime);
 Wave1();
break;
  case 63:// WAVE 3
  Serial.print(" Wave 3 ");
  Serial.println(ShowTime);
 Wave3();
break;
  case 64:// Scared
  Serial.print(" Scared ");
  Serial.println(ShowTime);
 Scared();
break;
  case 66:// WAVE 2
  Serial.print(" Wave 2 ");
  Serial.println(ShowTime);
 Wave2();
break;
  case 67:// WAVE 4
  Serial.print(" Wave 4 ");
  Serial.println(ShowTime);
 Wave4();
break;


 


 default:
 case 00:// No Show
 break;
 }
}






  

//////////////////////////////////////////////////////////////////////////////////////////
///*****                     Dome Panel Movement Functions                        *****///
//////////////////////////////////////////////////////////////////////////////////////////

void closeAllPanels() {
  if (ShowTime <= 1) { 
digitalWrite(OEPin, LOW); 
  }
  Serial.println("CLOSE ALL");
delay(100);
 pwm.setPWM(0, 0, Pie1C);
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(10, 0, ElevenC); 
 pwm.setPWM(11, 0, ThirteenC);
 delay(500);
  if (ShowTime >= 2) {
digitalWrite(OEPin, HIGH);  
}
}
void openAllPanels() {
   if (ShowTime <= 1) {
digitalWrite(OEPin, LOW); 
   }
  Serial.println("OPEN ALL");

 pwm.setPWM(0, 0, Pie1O); 
 pwm.setPWM(1, 0, Pie2O);
 pwm.setPWM(2, 0, Pie3O);
 pwm.setPWM(3, 0, Pie4O); 
 pwm.setPWM(4, 0, OneO);
 pwm.setPWM(5, 0, TwoO); 
 pwm.setPWM(6, 0, ThreeO);
 pwm.setPWM(7, 0, FourO); 
 pwm.setPWM(8, 0, SevenO);
 pwm.setPWM(9, 0, TenO);
 pwm.setPWM(10, 0, ElevenO); 
 pwm.setPWM(11, 0, ThirteenO);
 delay(500);
  if (ShowTime >= 2) {
digitalWrite(OEPin, HIGH);  
}}

void AllPanelsOpen(){
  pwm.setPWM(0, 0, Pie1O); 
 pwm.setPWM(1, 0, Pie2O);
 pwm.setPWM(2, 0, Pie3O);
 pwm.setPWM(3, 0, Pie4O); 
 pwm.setPWM(4, 0, OneO);
 pwm.setPWM(5, 0, TwoO); 
 pwm.setPWM(6, 0, ThreeO);
 pwm.setPWM(7, 0, FourO); 
 pwm.setPWM(8, 0, SevenO);
 pwm.setPWM(9, 0, TenO);
 pwm.setPWM(10, 0, ElevenO); 
 pwm.setPWM(11, 0, ThirteenO); 
}


void AllPanelsMid(){
 pwm.setPWM(0, 0, Pie1M); 
 pwm.setPWM(1, 0, Pie2M);
 pwm.setPWM(2, 0, Pie3M);
 pwm.setPWM(3, 0, Pie4M); 
 pwm.setPWM(4, 0, OneM);
 pwm.setPWM(5, 0, TwoM); 
 pwm.setPWM(6, 0, ThreeM);
 pwm.setPWM(7, 0, FourM); 
 pwm.setPWM(8, 0, SevenM);
 pwm.setPWM(9, 0, TenM);
 pwm.setPWM(10, 0, ElevenM); 
 pwm.setPWM(11, 0, ThirteenM);
}
void AllPanelsClosed(){
 pwm.setPWM(0, 0, Pie1C);
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(10, 0, ElevenC); 
 pwm.setPWM(11, 0, ThirteenC);  
}

void EvenPanelsClosed(){
 pwm.setPWM(1, 0, Pie2C);
 pwm.setPWM(3, 0, Pie4C); 
 pwm.setPWM(5, 0, TwoC); 
 pwm.setPWM(7, 0, FourC); 
 pwm.setPWM(9, 0, TenC);
 pwm.setPWM(11, 0, ThirteenC);  
}
void EvenPanelsOpen(){
 pwm.setPWM(1, 0, Pie2O);
 pwm.setPWM(3, 0, Pie4O); 
 pwm.setPWM(5, 0, TwoO); 
 pwm.setPWM(7, 0, FourO); 
 pwm.setPWM(9, 0, TenO);
 pwm.setPWM(11, 0, ThirteenO);  
}
void OddPanelsClosed(){
 pwm.setPWM(0, 0, Pie1C);
 pwm.setPWM(2, 0, Pie3C);
 pwm.setPWM(4, 0, OneC);
 pwm.setPWM(6, 0, ThreeC);
 pwm.setPWM(8, 0, SevenC);
 pwm.setPWM(10, 0, ElevenC); 
 }
void OddPanelsOpen(){
 pwm.setPWM(0, 0, Pie1O);
 pwm.setPWM(2, 0, Pie3O);
 pwm.setPWM(4, 0, OneO);
 pwm.setPWM(6, 0, ThreeO);
 pwm.setPWM(8, 0, SevenO);
 pwm.setPWM(10, 0, ElevenO); 
}
//-----------------------------------------Apply Incoming Message---------------------
void ApplyIncoming(){

  
//  Serial.print (IncomingInt);
//   Serial.println (CurrentNumber);
   
switch(CurrentNumber){
  case 51:
    openAllPanels();
    break;
  case 52:
 CurrentShow = 52;
  Shows();
    break;
    case 53:
    CurrentShow = 53;
  Shows();
    break;
    case 54:
 CurrentShow = 54;
  Shows();
  break;
  case 55:
    closeAllPanels();
     break;
   case 56:
 CurrentShow = 56;
  Shows();
   break;
  case 57:
 CurrentShow = 57;
  Shows();
    break;
   case 58:
   CurrentShow = 58;
  Shows();
    break; 

  case 61:
    openAllPanels();
    break;
  case 62:
 CurrentShow = 62;
  Shows();
    break;
    case 63:
    CurrentShow = 63;
  Shows();
    break;
    case 64:
 CurrentShow = 64;
  Shows();
  break;
  case 65:
    closeAllPanels();
     break;
   case 66:
 CurrentShow = 66;
  Shows();
   break;
  case 67:
 CurrentShow = 67;
  Shows();
    break;
   case 68:
   CurrentShow = 68;
  Shows();
    break; 
    

}

}
 
  


//--------------------------------------------------We Got One-----------------
void WeGotOne(){
//if(LastNum != IncomingInt){ 
Serial.println("WE GOT ONE");
CurrentNumber = IncomingInt;
 ShowTime = 0;
 LastNum = CurrentNumber;
 ApplyIncoming(); 
    
}
//-------------------------------------------------Radio-------------------------------
void Radio(){
  if (rf69_manager.available()) {
    // Wait for a message addressed to us from the client
    uint8_t buf[sizeof(RadioPacket)];
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len,2000, &from))
    {
      memcpy(&RadioPacket, buf, sizeof(RadioPacket));
      if (!len) return;
      buf[len] = 0;
      Incoming1 = String(RadioPacket.n1);
      Incoming2 = String(RadioPacket.n2);
      Incoming3 = String(RadioPacket.n3);
     

IncomingMsg = Incoming1 + Incoming2 + Incoming3;
   
IncomingInt = IncomingMsg.toInt();
Num3Int = Incoming3.toInt();
//Serial.print("Incoming3: ");
//Serial.println(Incoming3);
if (1 <= Num3Int && Num3Int <=8){
WeGotOne();
    }
  }
}
}

//---------------------------------------------------Debug Radio
void DebugRadio(){
     Serial.print("RSSI: ");
      Serial.println(rf69.lastRssi(), DEC);

    Serial.print(" Incoming Int ");
   Serial.println(IncomingInt);
    Serial.print(" LastNum ");
   Serial.println(LastNum);
    Serial.print(" CurrentNumber ");
   Serial.println(CurrentNumber);
     Serial.print(" CurrentShow ");
   Serial.println(CurrentShow);
}     
      
    





void loop(){ 

   CurrentMillis = millis();
 
 
    if (millis() - PreviousDisplayMillis >= DisplayInterval){
    PreviousDisplayMillis = CurrentMillis;
    DebugRadio();
    }
  
    if (millis() - PreviousRadioMillis >= RadioInterval){
    PreviousRadioMillis = CurrentMillis;
    Radio();
    ApplyIncoming();
    }

   if (millis() - PreviousLoopMillis >= LoopInterval){
    PreviousLoopMillis = CurrentMillis;
    Shows();
      }
     if (millis() - PreviousShowTimeMillis >= ShowTimeInterval){
            PreviousShowTimeMillis = CurrentMillis;
            ShowTime++;
     }
     
  delay(100);
 
}
int mapPulselength(double microseconds) {
  double pulselength;
  int pulse;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  pulselength /= 4096;  // 12 bits of resolution
  pulse = microseconds/pulselength;
  return pulse;
}
