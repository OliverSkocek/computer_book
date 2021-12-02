#define WRITE_ENABLE 10
#define OUTPUT_ENABLE 11
#define RESET 12
#define NEXT 13

#define DATA 2

#define HALT 0b00000000
#define IF 0b00100000
#define NAND 0b01000000
#define LSHIFT 0b01100000
#define POP 0b10000000
#define PUSH 0b10100000
#define LOAD 0b11000000
#define RESET_IF 0b11100000


byte data[] = {
  // precondition
  LOAD | 0b01000,  // x 
  POP | 0b0,
  POP | 0b00100,
  LOAD | 0b00010,  // y
  POP | 0b00010,
  POP | 0b00110,
  // Code
  NAND | 0b00100,
  POP | 0b01000,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b10000,
  PUSH | 0b00100,
  POP | 0b01000,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b01010,
  PUSH | 0b00110,
  POP | 0b01100,
  POP | 0b01110,
  NAND | 0b01100,
  POP | 0b01110,
  NAND | 0b01000,
  POP | 0b01000,
  NAND | 0b01100,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b00100,
  LSHIFT | 0b10000,
  POP | 0b00110,
  IF,
    LOAD | 0b00010,
    POP | 0b10010,
//END,
  PUSH | 0b10001,
  POP | 0b01100,
  POP | 0b01110,
  NAND | 0b01100,
  POP | 0b01100,
  POP | 0b01110,
  NAND | 0b01100,
  RESET_IF,
  PUSH | 0b10010,
  POP | 0b00110,
  HALT,
};

void writeEEPROM(byte data){
  for (int pin=DATA + 7; pin>=DATA; pin--){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, data &1);
    data = data >> 1;
  }
  digitalWrite(WRITE_ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_ENABLE, HIGH);
  delay(10);
}

void nextAddress(){
  digitalWrite(NEXT, HIGH);
  delayMicroseconds(100);
  digitalWrite(NEXT, LOW);
  delay(10);
}

void resetAddress(){
  digitalWrite(RESET, HIGH);
  delayMicroseconds(100);
  digitalWrite(RESET, LOW);
  delay(10);
}

byte readEEPROM() {
  byte _data = 0;
  for (int pin=DATA + 7; pin>=DATA; pin--){
    pinMode(pin, INPUT);
    _data = (_data << 1) + digitalRead(pin);
  }
  return _data;
}

void printContents() {
  digitalWrite(OUTPUT_ENABLE, LOW);
  delay(100);
  byte opcode;
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address++) {
    opcode = readEEPROM();
    nextAddress();
    char buf[20];
    sprintf(buf, "%03x:  %1x %1x %1x %1x %1x %1x %1x %1x", address,
    (opcode >> 0) & 1, (opcode >> 1) & 1, (opcode >> 2) & 1, (opcode >> 3) & 1, 
    (opcode >> 4) & 1, (opcode >> 5) & 1, (opcode >> 6) & 1, (opcode >> 7) & 1);
    //sprintf(buf, "%03x:  %04x", address,opcode);

    Serial.println(buf);
  }
  resetAddress();
  digitalWrite(OUTPUT_ENABLE, HIGH);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(WRITE_ENABLE, OUTPUT);
  pinMode(OUTPUT_ENABLE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(NEXT, OUTPUT);

  digitalWrite(OUTPUT_ENABLE, HIGH);
  digitalWrite(WRITE_ENABLE, HIGH);
  resetAddress();
  Serial.begin(9600);
  Serial.print("Programming EEPROM");

  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address++) {
    writeEEPROM(data[address]);
    nextAddress();
    if (address % 16 == 0) {
      Serial.print(".");
    }
  }
  resetAddress();
  Serial.println(" done");

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();

  resetAddress();
  for (int pin=2; pin<=13; pin++){
    pinMode(pin, INPUT);
  }
  pinMode(OUTPUT_ENABLE, OUTPUT);
  pinMode(WRITE_ENABLE, OUTPUT);
  digitalWrite(OUTPUT_ENABLE, LOW);
  digitalWrite(WRITE_ENABLE, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
