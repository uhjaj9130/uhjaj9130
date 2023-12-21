//본선 코드
#define ENA 5 //핀 지정
#define IN1 22
#define IN2 6
#define IN3 7
#define IN4 23
#define ENB 8
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12

// 초음파 센서1
#define Trig A0
#define Echo A1

// 초음파 센서2
#define Trig A2
#define Echo A3

const int distSenPin1 = A8; //적외선 센서 핀 지정
const int distSenPin2 = A9;

int sensorL = 24; // 라인트레이싱 핀 지정
int sensorC = 2;
int sensorR = 3;
#define left 24
#define center 2
#define right 3

int a; //초음파 센서값을 받을 변수지정
int b;
double d; //적외선 아날로그 센서값을 받을 변수지정
double f;
int LE; // 라인트레이싱 센서값을 받을  번수지정
int CE;
int RI;

void setup() {
  analogReference(DEFAULT); //아날로그 기본전압인 5v사용
  Serial.begin(9600); //시리얼 통신
  pinMode(24, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(ENA, INPUT);
  pinMode(22, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(ENB, INPUT);
  pinMode(7, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A3, INPUT);
  pinMode(A2, OUTPUT);
}
double getUltrasonicSensor1(void) { //초음파 센서값을 받아서 반환하는 함수 왼쪽
  unsigned long duration1;
  digitalWrite(A0, HIGH);
  delayMicroseconds(10);
  digitalWrite(A0, LOW);
  duration1 = pulseIn(A1, HIGH, 30000UL);
  return duration1 * 0.17;
}
double getUltrasonicSensor2(void) { //초음파 센서값을 받아서 반환하는 함수 오른쪽
  unsigned long duration2;
  digitalWrite(A2, HIGH);
  delayMicroseconds(10);
  digitalWrite(A2, LOW);
  duration2 = pulseIn(A3, HIGH, 30000UL);
  return duration2 * 0.17;
}
void drive() { //원형경기장까지 진입할 라인트레이싱 함수
  LE = digitalRead(sensorL);
  CE = digitalRead(sensorC);
  RI = digitalRead(sensorR);
  if (LE == HIGH && CE == LOW && RI == HIGH) { //검흰검, 검검검, 흰검흰, 흰흰흰 감지시 직진
    moveForward();
  } else if (LE == LOW && CE == LOW && RI == LOW) {
    moveForward();
  } else if (LE == HIGH && CE == HIGH && RI == HIGH) {
    moveForward();
  } else if (LE == LOW && CE == LOW && RI == HIGH) { //검검흰 왼쪽으로 도는함수
    PturnLeft();
  } else if (LE == HIGH && CE == LOW && RI == LOW) { //흰검검 오른쪽으로 도는함수
    PturnRight();
  } else if (LE == LOW && CE == HIGH && RI == HIGH) { //검흰흰 왼쪽으로 도는 함수
    turnLeft();
  } else if (LE == HIGH && CE == HIGH && RI == LOW) { //흰흰검 오른쪽으로 돌기
    turnRight();
  } else if (LE == LOW && CE == HIGH && RI == LOW) {
    moveForward();
  }
}

//전진
void moveForward() {
  analogWrite(ENA, 255);
  digitalWrite(22, HIGH);
  digitalWrite(6, LOW);
  analogWrite(ENB, 255);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

// 좌회전
void turnLeft() {
  analogWrite(ENA, 255);
  digitalWrite(22, HIGH);
  digitalWrite(6, LOW);
  analogWrite(ENB, 110);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

//좌회전
void PturnLeft() {
  analogWrite(ENA, 255);
  digitalWrite(22, HIGH);
  digitalWrite(6, LOW);
  analogWrite(ENB, 150);
  digitalWrite(7, LOW);
  digitalWrite(23, HIGH);
}

//좌회전
void PPturnLeft() {
  analogWrite(ENA, 255);
  digitalWrite(22, HIGH);
  digitalWrite(6, LOW);
  analogWrite(ENB, 150);
  digitalWrite(7, HIGH);
  digitalWrite(23, HIGH);
}

// 우회전
void turnRight() {
  analogWrite(ENA, 110);
  digitalWrite(22, HIGH);
  digitalWrite(6, LOW);
  analogWrite(ENB, 255);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

//우회전
void PturnRight() {
  analogWrite(ENA, 150);
  digitalWrite(22, LOW);
  digitalWrite(6, HIGH);
  analogWrite(ENB, 255);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

//우회전
void PPturnRight() {
  analogWrite(ENA, 0);
  digitalWrite(22, HIGH);
  digitalWrite(6, HIGH);
  analogWrite(ENB, 230);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

// 후진
void back() {
  analogWrite(ENA, 255);
  digitalWrite(22, LOW);
  digitalWrite(6, HIGH);
  analogWrite(ENB, 255);
  digitalWrite(7, LOW);
  digitalWrite(23, HIGH);
}

//한바퀴돌기
void turn() {
  analogWrite(ENA, 1기0);
  digitalWrite(22, LOW);
  digitalWrite(6, HIGH);
  analogWrite(ENB, 150);
  digitalWrite(7, HIGH);
  digitalWrite(23, LOW);
}

// 다시 원형경기장 검은색 부분으로 돌아가는함수
void backcircle() {
  back();
  delay(3000);
  turn();
  delay(2300);
}

// 타미야 돌리는함수
void tamiya() {
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
}

//타미야 돌리는 함수
void REtamiya() {
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
}
// 라인트레이싱을 끝내고 원형경기장안으로 진입하는 함수
void incircle() {
  moveForward();
  delay(2300);
}

// 원형경긱장 진입후 상대로봇과 씨름하는 함수
void battle() {
  a = getUltrasonicSensor1(); //a값을 초음파센서 왼쪽으로 받아 지정
  b = getUltrasonicSensor2(); //b값을 초음파센서 오른쪽으로 받아 지정
  if ((a <= 300 || d <= 60) && f > 60) { // 왼쪽 초음파센서가 30cm이내 거리의 상대로봇을 감지하거나 왼쪽 적외선센서만 

// 상대로봇을 감지하면 왼쪽으로 회전
    analogWrite(ENA, 155);
    digitalWrite(22, HIGH);
    digitalWrite(6, LOW);
    analogWrite(ENB, 255);
    digitalWrite(7, LOW);
    digitalWrite(23, HIGH);
  } else if ((b <= 300 || f <= 60) && d > 60) { // 오른쪽 초음파센서가 30cm이내 거리의 상대로봇을 감지하거나 오른쪽 적외// 선센서만 상대로봇을 감지하면 오른쪽으로 회전
    analogWrite(ENA, 255);
    digitalWrite(22, LOW);
    digitalWrite(6, HIGH);
    analogWrite(ENB, 155);
    digitalWrite(7, HIGH);
    digitalWrite(23, LOW);
  } else if (a > 300 && b > 300) {
    if (LE == LOW && CE == LOW && RI == LOW) { // 오른쪽과 왼쪽 초음파센서 둘이 상대로봇을 감지하지못하면 직진
      moveForward();
    } else if (LE == HIGH || CE == HIGH || RI == HIGH) { //흰색 감지시 뒤로 돌기
      backcircle();
    }
  }
}

void loop() {
  unsigned long pv = 0;
  unsigned long ct = millis();
  int i;
  double voltage1, distance1, voltage2, distance2;              // 적외선 센서로 거리를 측정하고 공식을 통해 정확한 // 거리를 측정해서 d,f에 거리값 저장
  voltage1 = 5.0 / 1023.0 * double(analogRead(distSenPin1));
  distance1 = 27.588 * pow(voltage1, -1.198);
  voltage2 = 5.0 / 1023.0 * double(analogRead(distSenPin2));
  distance2 = 27.588 * pow(voltage2, -1.198);
  d = distance1;
  f = distance2;
  drive();
  if (ct - pv >= 18500 && ct - pv <= 33000) { //18.5초와 33초사이에 검검검을 인지하면 원형경기장 진입 함수를 실행하고 // 진입후 씨름함수 무한실행
    if (LE == LOW && CE == LOW && RI == LOW) {
      incircle();
      while (1) {
        battle();
      }
    }
  }
}