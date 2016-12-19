#include <SoftwareSerial.h> //블루투스 통신을 위한 헤더
#include <Servo.h>  //서보모터 사용을 위한 헤더


#define BLUE_TX 12 //블루투스 TX
#define BLUE_RX 13 //블루투스 RX
#define LEFT_A 2 //왼쪽 바퀴
#define LEFT_B 3 //왼쪽 바퀴
#define RIGHT_A 4 //오른쪽 바퀴
#define RIGHT_B 5 //오른쪽 바퀴
#define TRIG_1 6  //왼쪽 초음파센서 트리거
#define ECHO_1 7  //왼쪽 초음파센서 에코
#define TRIG_2 8  //중간 초음파센서 트리거
#define ECHO_2 9  //중간 초음파센서 에코
#define TRIG_3 10  //오른쪽 초음파센서 트리거
#define ECHO_3 11  //오른쪽 초음파센서 에코
#define SERVO A0 //서보모터

SoftwareSerial mySerial(BLUE_TX, BLUE_RX);  //시리얼 통신을 위한 객체선언
Servo servo;  //서보 객체
char value=0; //블루투스에서 받는 값
int button1 = -1; //버튼값 초기화
int button2 = -1; //버튼값 초기화
char a [1000];
int count = 0;

void forward(); //전진
void backward();  //후진
void left();  //좌회전
void right(); //우회전
void leftback();  //좌후진
void rightback(); //우후진
void halt(); //정지
void freedrive(long cm1,long cm2, long cm3);  //자율주행
long duration(int a, int b);  //시간측정
long cm1, cm2, cm3; //거리
long microsecondsToCentimeters(long microseconds);  //시간을 거리로 바꿔주는 함수



void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); //블루투스 시리얼 개방
  pinMode(TRIG_1,OUTPUT);  // 센서1 Trig 핀
  pinMode(ECHO_1,INPUT);   // 센서1 Echo 핀
  pinMode(TRIG_2,OUTPUT);  // 센서2 Trig 핀
  pinMode(ECHO_2,INPUT);   // 센서2 Echo 핀
  pinMode(TRIG_3,OUTPUT);  // 센서3 Trig 핀
  pinMode(ECHO_3,INPUT);   // 센서3 Echo 핀
  pinMode(LEFT_A, OUTPUT);  //왼쪽바퀴 A
  pinMode(LEFT_B, OUTPUT);  //왼쪽바퀴 B
  pinMode(RIGHT_A, OUTPUT); //오른쪽바퀴 A
  pinMode(RIGHT_B, OUTPUT); //오른쪽바퀴B 
  servo.attach(SERVO); 
}

void loop()
{
 
   while(mySerial.available())  //mySerial에 전송된 값이 있으면
  {
  value = mySerial.read(); //전송값 읽음
  //Serial.print(value);  //전송값 출력
 // Serial.println();
  if(value == '7')  //전송값이 7이면
  {
    button1 = true; //자율주행 켜기
    Serial.print(value); 
  }
  if(value == '8')  //전송값이 8이면 
  {
    button1 = false;  //자율주행 끄기
    Serial.print(value); 
  }
  if(value == 'A')  //전송값이 A이면
  {
    button2 = true; //경로탐색 켜기
    Serial.print(value); 
  }
  if(value == '9')  //전송값이 9이면 
  {
    button2 = false;  //경로탐색 끄기
    Serial.print(value); 
  }
   if(value == '1')  //전송값이 1이면 
  {
    forward();  //전진
    a[count] = value; 
    count++;
    Serial.print(value); 
  }
   if(value == '2')  //전송값이 2이면 
  {
   backward();  //후진
   a[count] = value; 
   count++;
   Serial.print(value); 
  }
    if(value == '3')  //전송값이 3이면 
  {
    left();  //좌회전
    a[count] = value; 
    count++;
    Serial.print(value); 
  }
    if(value == '4')  //전송값이 4이면 
  {
   right();  //우회전
     a[count] = value; 
    count++;
    Serial.print(value); 
  }
   if(value == '0')  //전송값이 0이면 
  {
  halt();  //멈춤
  }
 
  
  } 
 
  if(button1 == true)  //버튼이 true이면 자율주행 시작
  {
  cm1 = microsecondsToCentimeters(duration(6, 7));  //왼쪽 초음파센서 측정
  
  Serial.print(cm1); //거리 출력
  Serial.print("cm1");
  Serial.println();

  cm2 = microsecondsToCentimeters(duration(8, 9));  //중간 초음파센서 측정
  
  Serial.print(cm2); //거리 출력
  Serial.print("cm2");
  Serial.println();

  cm3 = microsecondsToCentimeters(duration(10, 11));  //오른쪽 초음파센서 측정
  
  Serial.print(cm3); //거리 출력
  Serial.print("cm3");
  freedrive(cm1, cm2, cm3); //초음파센서 자율주행
  //forward();  //전진
  Serial.println();
  delay(100);  // 0.1초 대기 후 다시 측정
  }
 

   if(button2 == true)  //버튼이 1이면 경로돌아가기 시작
  {
    for(int i = count-1; i >= 0; i--){
    Serial.print(a[i]);
    if(a[i] == '1') // 전진이면 후진
    {
      servo.write(90);
      digitalWrite(LEFT_A, LOW);
      digitalWrite(LEFT_B, HIGH);
      digitalWrite(RIGHT_A, LOW);
      digitalWrite(RIGHT_B, HIGH);
      delay(70);
      
    }
    else if(a[i] == '2')  //후진이면 전진
    {
     servo.write(90);
     digitalWrite(LEFT_A, HIGH);
     digitalWrite(LEFT_B, LOW);
     digitalWrite(RIGHT_A, HIGH);
     digitalWrite(RIGHT_B, LOW);
     delay(70);
    }
    else if(a[i] == '3')  //좌회전이면 좌후진
    {
      leftback();
    }
    else if(a[i] == '4')  //우회전이면 우후진
    {
      rightback();
    }
    
    }
    count = 0;  //배열번호 0으로 초기화
  }
  
}

long microsecondsToCentimeters(long microseconds) //걸린시간을 cm로 바꿔주는 함수
{
  return microseconds / 29 / 2; 
}
void forward(){ //전진
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
}
void backward(){//후진
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, HIGH);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
}
void halt(){//정지
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, LOW);
  servo.write(90);
}
void left(){  //좌회전 
  servo.write(45); 
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
}
void right(){ //우회전 
  servo.write(135);
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);

}
void leftback(){  //좌 후진
   servo.write(45); 
   digitalWrite(LEFT_A, LOW);
   digitalWrite(LEFT_B, HIGH);
   digitalWrite(RIGHT_A, LOW);
   digitalWrite(RIGHT_B, HIGH);
   delay(70);
   
}
void rightback(){ //우후진
  servo.write(135);
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, HIGH);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  delay(70);
}

void freedrive(long cm1, long cm2, long cm3){  //초음파센서 자율주행
  if(cm1 < 20 && cm2 >= 20 && cm3 >= 20){  //왼쪽초음파센서만 20cm 보다 가까우면 우회전
  digitalWrite(LEFT_A, HIGH); 
  digitalWrite(LEFT_B, LOW); 
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  delay(70);
  halt();
  }
  if(cm2 < 20 && cm1 >= 20 && cm3 >= 20){  //중간 초음파센서만 20cm 보다 가까우면 우회전
  digitalWrite(LEFT_A, HIGH); 
  digitalWrite(LEFT_B, LOW); 
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  delay(70);
  halt();
  }
  if(cm3 < 20 && cm1 >= 20 && cm2 >= 20){  //오른쪽 초음파센서만 20cm 보다 가까우면 좌회전
  digitalWrite(LEFT_A, LOW); 
  digitalWrite(LEFT_B, HIGH); 
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
  delay(70);
  halt();
  }
 
  if(cm1 < 20 && cm2 < 20 && cm3 >= 20){  //왼쪽과 앞이 20cm 보다 가까우면 우회전
  digitalWrite(LEFT_A, HIGH); 
  digitalWrite(LEFT_B, LOW); 
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  delay(70);
  halt();
  }
  if(cm1 >= 20 && cm2 < 20 && cm3 < 20){  //오른쪽과 앞이 20cm보다 가까우면 좌회전
  digitalWrite(LEFT_A, LOW); 
  digitalWrite(LEFT_B, HIGH); 
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
  delay(70);
  halt();
  }
  if(cm1 < 20 && cm2 < 20 && cm3 < 20){ //삼면이 20cm 보다 가까우면 정지
    halt();
  }
  else{ //아니면 전진
    forward();
  }
}

long duration(int a, int b){  // 초음파센서 시간측정 
  digitalWrite(a, HIGH);  // 센서에 Trig 신호 입력
  delayMicroseconds(10); // 10us 정도 유지
  digitalWrite(a, LOW);   // Trig 신호 off
  return pulseIn(b, HIGH); // Echo pin: HIGH->Low 간격을 측정
}
  



