#include <AccelStepper.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define xstep 2  //x축
#define xdir 3
#define xena 30

#define ystep 4  //y축
#define ydir 5
#define yena 31

#define zstep 6  //z축
#define zdir 7
#define zena 32

LiquidCrystal_I2C lcd(0x27, 20, 4);


String one = "O";    // initial value
String two = "O";    // initial value
String three = "O";  // initial value


AccelStepper stepperx = AccelStepper(1, xstep, xdir);
AccelStepper steppery = AccelStepper(1, ystep, ydir);
AccelStepper stepperz = AccelStepper(1, zstep, zdir);

Servo servo;

int x1, x2, y1, y2, z1, z2, i1, i2, i3, a, angle;


const int buzzerPin = 53;
const int lswx1 = 10;  //x축 오른쪽
const int lswx2 = 11;  //x축 왼쪽
const int lswy1 = 12;  //y축 오른쪽
const int lswy2 = 13;  //y축 왼쪽
const int lswz1 = 8;   //z축 오른쪽
const int lswz2 = 9;   //z축 왼쪽

int smp = 50;  // 서보 모터

const int xsd = 1000;
const int ysd = 1000;
const int zsd = 500;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);

  lcd.init();  // initialize the lcd
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(xstep, OUTPUT);
  pinMode(xdir, OUTPUT);
  pinMode(ystep, OUTPUT);
  pinMode(ydir, OUTPUT);
  pinMode(zstep, OUTPUT);
  pinMode(zdir, OUTPUT);
  

  servo.attach(smp);

  pinMode(lswx1, INPUT);
  pinMode(lswx2, INPUT);
  pinMode(lswy1, INPUT);
  pinMode(lswy2, INPUT);
  pinMode(lswz1, INPUT);
  pinMode(lswz2, INPUT);


  stepperx.setMaxSpeed(1000);
  stepperx.setAcceleration(50);
  stepperx.setCurrentPosition(0);

  steppery.setMaxSpeed(1000);
  steppery.setAcceleration(50);
  steppery.setCurrentPosition(0);

  stepperz.setMaxSpeed(1000);
  stepperz.setAcceleration(150);
  stepperz.setCurrentPosition(0);

  sstop();

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Cargo Carry System");
  lcd.setCursor(0, 1);
  lcd.print("First Cargo:");
  lcd.setCursor(16, 1);
  lcd.print(one);  // Change a to b
  lcd.setCursor(0, 2);
  lcd.print("Second Cargo:");
  lcd.setCursor(16, 2);
  lcd.print(two);  // Change a to c
  lcd.setCursor(5, 3);
  lcd.print("GREG TECH!");

  homing();
}

void loop() {
  running();
}

void sstop() {
  digitalWrite(xena, HIGH);
  digitalWrite(yena, HIGH);
  digitalWrite(zena, HIGH);
}

void sstart() {
  digitalWrite(xena, LOW);
  digitalWrite(yena, LOW);
  digitalWrite(zena, LOW);
}

void running() {

  if (Serial1.available()) {

    a = Serial1.read();

    Serial.write(a);
    Serial.println("");

    scp();

    switch (a) {
      case '1':
        a1();
        one = "O";  // update value of one
        tone(buzzerPin, 1000);
        delay(1000);
        noTone(buzzerPin);

        break;


      case '2':
        a2();
        one = "X";
        tone(buzzerPin, 1000);
        delay(1000);
        noTone(buzzerPin);

        break;

      case '3':
        a3();
        two = "O";
        tone(buzzerPin, 1000);
        delay(1000);
        noTone(buzzerPin);

        break;

      case '4':
        a4();
        two = "X";
        tone(buzzerPin, 1000);
        delay(1000);
        noTone(buzzerPin);

        break;

      case '7':
        Serial3.println("출발");

        break;

      case '8':
        Serial3.println("정지");

        break;

      case '5':
        homing();

        break;

      case '0':
        gon();
        goff();

        break;

      default:
        break;
    }

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Cargo Carry System");
    lcd.setCursor(0, 1);
    lcd.print("First Cargo:");
    lcd.setCursor(16, 1);
    lcd.print(one);  // Change a to b
    lcd.setCursor(0, 2);
    lcd.print("Second Cargo:");
    lcd.setCursor(16, 2);
    lcd.print(two);  // Change a to c
    lcd.setCursor(5, 3);
    lcd.print("GREG TECH!");
  }
  a0();
}

void x() {
  digitalWrite(xena, LOW);

  stepperz.stop();

  digitalWrite(xdir, x2);  //회전방향 출력

  for (i1 = 0; i1 < x1; i1++) {  //정해진 스텝수만큼 펄스입력
    digitalWrite(xstep, HIGH);
    delayMicroseconds(xsd);  //딜레이값
    digitalWrite(xstep, LOW);
    delayMicroseconds(xsd);

    if (digitalRead(lswx1) == LOW || digitalRead(lswx2) == LOW) {
      digitalWrite(xena, HIGH);
      Serial.println("x limit");
      break;
    }
  }

  digitalWrite(xena, HIGH);
}

void y() {
  digitalWrite(yena, LOW);

  digitalWrite(ydir, y2);  //회전방향 출력

  for (i2 = 0; i2 < y1; i2++) {  //정해진 스텝수만큼 펄스입력
    digitalWrite(ystep, HIGH);
    delayMicroseconds(ysd);  //딜레이값
    digitalWrite(ystep, LOW);
    delayMicroseconds(ysd);

    if (digitalRead(lswy1) == LOW || digitalRead(lswy2) == LOW) {
      digitalWrite(yena, HIGH);
      Serial.println("y limit");
      break;
    }
  }

  digitalWrite(yena, HIGH);
}

void z() {
  digitalWrite(zena, LOW);

  digitalWrite(zdir, z2);  //회전방향 출력


  for (i3 = 0; i3 < z1; i3++) {  //정해진 스텝수만큼 펄스입력
    digitalWrite(zstep, HIGH);
    delayMicroseconds(zsd);  //딜레이값
    digitalWrite(zstep, LOW);
    delayMicroseconds(zsd);

    if (digitalRead(lswz1) == LOW || digitalRead(lswz2) == LOW) {
      digitalWrite(zena, HIGH);
      Serial.print("z limit");
      break;
    }
  }

  digitalWrite(zena, HIGH);
}

void a1() {
  x1 = 260;  //step 1
  x2 = 0;

  z1 = 3600;
  z2 = 0;

  x();
  z();

  delay(1000);

  gon();

  delay(1000);

  z2 = 1;

  z();

  x1 = 170;  //step 2
  x2 = 0;

  y1 = 610;
  y2 = 0;

  z1 = 3100;
  z2 = 0;

  move();

  delay(1000);

  goff();

  delay(1000);

  z2 = 1;

  z();

  x1 = 430;  //step 3
  x2 = 1;

  y1 = 610;
  y2 = 1;

  x();
  y();
}

void a2() {

  x1 = 430;  //step 1
  x2 = 0;

  y1 = 610;
  y2 = 0;

  z1 = 3100;
  z2 = 0;

  move();

  delay(1000);

  gon();

  delay(1000);

  z2 = 1;

  z();

  x1 = 170;  //step 2
  x2 = 1;

  y1 = 610;
  y2 = 1;

  z1 = 3600;
  z2 = 0;

  move();

  delay(1000);

  goff();

  delay(1000);

  z2 = 1;

  z();

  x1 = 260;  //step 3
  x2 = 1;

  x();
}

void a3() {

  x1 = 260;  //step 1
  x2 = 0;

  z1 = 3600;
  z2 = 0;

  x();
  z();

  delay(1000);

  gon();

  delay(1000);

  z2 = 1;

  z();

  x1 = 80;  //step 2
  x2 = 1;

  y1 = 730;
  y2 = 0;

  z1 = 2000;
  z2 = 0;

  move();

  y1 = 100;  //step 2-1
  y2 = 0;

  z1 = 1100;
  z2 = 0;

  y();
  z();

  delay(1000);

  goff();

  delay(1000);

  z2 = 1;

  z();

  y1 = 100;  //step 3
  y2 = 1;

  z1 = 2000;
  z2 = 1;

  y();
  z();

  x1 = 180;  //step 3-1
  x2 = 1;

  y1 = 730;
  y2 = 1;

  x();
  y();
}

void a4() {

  x1 = 160;  //step 1
  x2 = 0;

  y1 = 720;
  y2 = 0;

  z1 = 2000;
  z2 = 0;

  move();

  y1 = 100;  //step 1-1
  y2 = 0;

  z1 = 1100;
  z2 = 0;

  y();
  z();

  delay(1000);

  gon();

  delay(1000);

  z2 = 1;

  z();

  y1 = 100;  //step 2
  y2 = 1;

  z1 = 2000;
  z2 = 1;

  y();
  z();

  x1 = 100;  //step 2-1
  x2 = 0;

  y1 = 720;
  y2 = 1;

  z1 = 3600;
  z2 = 0;

  move();

  delay(1000);

  goff();

  delay(1000);

  z2 = 1;

  z();

  x1 = 260;  //step 3
  x2 = 1;

  x();
}

void a0() {
  x1 = 0;
  y1 = 0;
  z1 = 0;
}

void move() {
  x();
  y();
  z();
}

void ghome() {

  Serial.println("Gripper home");

  for (angle = 0; angle <= 170; angle += 1) {
    servo.write(angle);
    delay(15);
  }
}

void gon() {

  Serial.println("Gripper ON");

  for (angle = 170; angle >= 10; angle -= 1) {
    servo.write(angle);
    delay(15);
  }
}
void goff() {
  Serial.println("Gripper OFF");

  for (angle = 0; angle <= 170; angle += 1) {
    servo.write(angle);
    delay(15);
  }
}
void homing() {

  Serial.println("Homing");

  zhoming();
  xhoming();
  yhoming();

  ghome();

  sstop();

  Serial.println("End");

  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
}

void xhoming() {
  digitalWrite(xena, LOW);

  stepperx.setSpeed(200);

  while (true) {
    stepperx.runSpeed();
    if (digitalRead(lswx2) == LOW) {

      stepperx.move(-100);

      while (stepperx.isRunning()) {
        stepperx.run();
      }

      digitalWrite(xena, HIGH);
      Serial.println("x homing");
      break;
    }
  }

  stepperx.setCurrentPosition(0);
}

void yhoming() {
  digitalWrite(yena, LOW);

  steppery.setSpeed(200);

  while (true) {
    steppery.runSpeed();
    if (digitalRead(lswy2) == LOW) {
      steppery.move(-100);

      while (steppery.isRunning()) {
        steppery.run();
      }

      digitalWrite(yena, HIGH);
      Serial.println("y homing");
      break;
    }
  }

  steppery.setCurrentPosition(0);
}

void zhoming() {
  digitalWrite(zena, LOW);

  stepperz.setSpeed(800);

  while (true) {
    stepperz.runSpeed();

    if (digitalRead(lswz1) == LOW) {
      stepperz.move(-300);

      while (stepperz.isRunning()) {
        stepperz.run();
      }

      digitalWrite(zena, HIGH);
      Serial.println("z homing");
      break;
    }
  }

  stepperz.setCurrentPosition(0);
}

void scp() {
  stepperx.setCurrentPosition(0);
  steppery.setCurrentPosition(0);
  stepperz.setCurrentPosition(0);
}