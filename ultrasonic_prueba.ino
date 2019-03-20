const byte trigPinTx1 = 4;
const byte echoPinTx1 = 2;
const byte intPinRx1 = 18;

const byte trigPinTx2 = 5;
const byte echoPinTx2 = 3;
const byte intPinRx2 = 19;

volatile unsigned long tiempo1 = 0;
volatile unsigned long tiempo2 = 0;
volatile unsigned long startTrig = 0;

volatile bool start1 = false;
volatile bool start2 = false;
volatile bool continuar1 = false;
volatile bool continuar2 = false;
volatile bool start = true;

void isr1();
void isr2();
void echo1ISR();
void echo2ISR();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Iniciando");

  pinMode(trigPinTx1, OUTPUT);
  pinMode(echoPinTx1, INPUT);
  pinMode(intPinRx1, INPUT);
  pinMode(trigPinTx2, OUTPUT);
  pinMode(echoPinTx2, INPUT);
  pinMode(intPinRx2, INPUT);
  attachInterrupt(digitalPinToInterrupt(echoPinTx1), echo1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(intPinRx1), isr1, RISING);
  attachInterrupt(digitalPinToInterrupt(echoPinTx2), echo2ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(intPinRx2), isr2, RISING);
  noInterrupts();
}

void loop() {
  
  if(continuar1){
    Serial.print("Tiempo1: ");
    Serial.println(tiempo1);
    delay(1000);
    continuar1 = false;
    start1 = false;
    continuar2 = true;
    digitalWrite(trigPinTx2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinTx2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinTx2, LOW);
    interrupts();
  }else if(continuar2){
    Serial.print("Tiempo2: ");
    Serial.println(tiempo2);
    delay(1000);
    continuar1 = false;
    start1 = false;
    start = true;
  }else if(start){
    digitalWrite(trigPinTx1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinTx1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinTx1, LOW);
    start = false;
    interrupts();
  }else{
    Serial.print("Midiendo");  
  }

}

void isr1(){
  noInterrupts();
  if(start1){
  tiempo1 = micros() - startTrig;
  continuar1 = true;
  }
}

void echo1ISR(){
  startTrig = micros();
  start1 = true;
}

void isr2(){
  noInterrupts();
  if(start2){
  tiempo2 = micros() - startTrig;
  continuar2 = true;
  }
}

void echo2ISR(){
  startTrig = micros();
  start2 = true;
}
