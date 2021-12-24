#define WRITE_ENABLE 10
#define OUTPUT_ENABLE 11
#define RESET_PIN 12
#define NEXT 13

#define DATA 2

#define HALT 0b00000000
#define IF 0b00100000
#define NAND 0b01000000
#define LSHIFT 0b01100000
#define POP 0b10000000
#define PUSH 0b10100000
#define LOAD 0b11000000
#define RESET 0b11100000

/*
byte data[] = {
  LOAD | 0b11111,
  POP | 0b00000,
  PUSH | 0b00000,
  LOAD | 0b11111,
  POP | 0b00010,
  PUSH | 0b00010,
  LOAD | 0b11111,
  POP | 0b00100,
  PUSH | 0b00100,
  LOAD | 0b11111,
  POP | 0b00110,
  PUSH | 0b00110,
  LOAD | 0b11111,
  POP | 0b01000,
  PUSH | 0b01000,
  LOAD | 0b11111,
  POP | 0b01010,
  PUSH | 0b01010,
  LOAD | 0b11111,
  POP | 0b01100,
  PUSH | 0b01100,
  LOAD | 0b11111,
  POP | 0b01110,
  PUSH | 0b01110,
  LOAD | 0b11111,
  POP | 0b10000,
  PUSH | 0b10000,
  LOAD | 0b11111,
  POP | 0b10010,
  PUSH | 0b10010,
  LOAD | 0b11111,
  POP | 0b10100,
  PUSH | 0b10100,
  LOAD | 0b11111,
  POP | 0b10110,
  PUSH | 0b10110,
  LOAD | 0b11111,
  POP | 0b11000,
  PUSH | 0b11000,
  LOAD | 0b11111,
  POP | 0b11010,
  PUSH | 0b11010,
  LOAD | 0b11111,
  POP | 0b11100,
  PUSH | 0b11100,
  LOAD | 0b11111,
  POP | 0b11110,
  PUSH | 0b11110,

  RESET,
};*/


/*
byte data[] = {
  // precondition
  LOAD | 0b01010,  // x 
  POP | 0b00100,
  PUSH | 0b00100,
  POP | 0b00000,
  LOAD | 0b00110,  // y
  POP | 0b00010,
  LOAD | 0b10100,  // y
  POP | 0b11100,
  LOAD | 0b00100,  // y
  POP | 0b11110,
  NAND | 0b0,
  LSHIFT | 0b0,
  IF,
    RESET,
  NAND | 0b11111,
  LSHIFT | 0b11100,
  HALT,
};*/

/*
byte data[] = {
  // precondition
  LOAD | 0b01010,  // x 
  POP | 0b0,
  LOAD | 0b00010,  // y
  POP | 0b11110,
  LOAD | 0b10100,  // y
  POP | 0b11100,
  PUSH | 0b0,
  IF,
    RESET,
  PUSH | 0b11111,
  PUSH | 0b11100,
  HALT,
};*/

/*byte data[] = {
  LOAD | 0b00010,
  IF,
    RESET,
  POP | 0b01011,
  LOAD | 0b00010,
  HALT,
};*/
/*byte data[] = {
  LOAD | 0b00010,
  IF,
    LOAD | 0b00010,
//END,
  PUSH | 0b10001,
  IF,
    LOAD | 0b00010,
//END,
  POP | 0b01101,

  IF,
    RESET,
//END,
  PUSH | 0b10011,
  IF,
    RESET,
//END,
  POP | 0b00111,
  HALT,
};*/

byte data[] = {
  // precondition
  LOAD | 0b10000,
  POP | 0b11010,
  IF,
    LOAD | 0b00000,
    POP | 0b11010,
  LSHIFT | 0b11011,
  IF,
    LOAD | 0b01010,  // x 
    POP | 0b00000,
    LOAD | 0b01110,  // y
    POP | 0b00010,
  // Code
  NAND | 0b00001,
  POP | 0b00100,
  POP | 0b00110,
  NAND | 0b00100,
  POP | 0b00110,    // x and y
  
  PUSH | 0b00000,
  POP | 0b01000,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b01100,    // nicht x

  PUSH | 0b00010,
  POP | 0b01110,
  POP | 0b01000,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b01000,    // nicht y

  PUSH | 0b00000,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b01000,
  NAND | 0b01100,
  POP | 0b01010,
  NAND | 0b01000,
  POP | 0b00000,    // x xor y

  LSHIFT | 0b00110,   // lshift x und y
  POP | 0b00010,
  IF,
    LOAD | 0b00010,
    POP | 0b11100,    // Überlauf

  POP | 0b00101, // if lshift x und y ungleich null springe
  POP | 0b00110,
  NAND | 0b00100,
  POP | 0b00100, 
  POP | 0b00110,
  NAND | 0b00100,
  LOAD | 0b10000,
  POP | 0b11110,
  IF,
    LOAD | 0b00000,
    POP | 0b11110,    
  LSHIFT | 0b11111,
  
  IF,
    RESET,
  PUSH | 0b00001,
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
  digitalWrite(RESET_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(RESET_PIN, LOW);
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
  pinMode(RESET_PIN, OUTPUT);
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
