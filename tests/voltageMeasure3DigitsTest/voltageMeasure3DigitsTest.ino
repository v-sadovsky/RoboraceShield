#include <SPI.h>     // подключаем библиотеку для работы с SPI

#define VOLTAGE_PIN A0 
#define REF_VOLTAGE 5.0
#define MAX_ADC     1024.0

#define R1 4.7
#define R2 1.6

#define LOAD_PIN 8   // ножка, подключенная к выводу LOAD драйвера

void setup() {
   // Настраиваем ножку на работу в режиме выхода
  pinMode(LOAD_PIN, OUTPUT);

  // Устанавливаем порядок передачи бит в MSB для SPI  
  SPI.setBitOrder(MSBFIRST);
  
  // Стартуем интерфейс SPI
  SPI.begin();

  // Пауза для микросхемы MAX7219, 
  // чтобы драйвер после включения питания начал реагировать на команды
  delay(10);

  // Переходим в нормальный режим работы из состояния shutdown
  sendPacket(0x0C, 0x01);

// Настраиваем драйвер
  sendPacket(0x0B, 0x02); // используем два разряда 
  sendPacket(0x09, 0x07); // внутреннее декодирование для каждого разряда
}

float adcValue = 0;
float inputVoltage;
float MeasuredVoltage;
int voltage;
byte digit1, digit2, digit3, tmp;
bool dot1 = false;

void loop() {
  adcValue = analogRead(VOLTAGE_PIN) + 0.5;  //  ADC Hardware rounds down

  // Convert measured adc value to the voltage
  MeasuredVoltage = adcValue * REF_VOLTAGE / MAX_ADC;
  
  // Calculate input voltage that correspond measured voltage
  inputVoltage = (R1 + R2) * MeasuredVoltage / R2;

  if (inputVoltage < 10) {
      dot1 = true;
      voltage = (int)(inputVoltage * 100);
  } else {
      voltage = (int)(inputVoltage * 10);
  }

  // Get digits of displayed voltage
  digit1 = voltage / 100;
  tmp =  voltage % 100;
  digit2 = tmp / 10;
  digit3 = tmp % 10;

  if (dot1) {
      digit1 |= 0b10000000;
  } else {
      digit2 |= 0b10000000;
  }

  // Display result
  sendPacket(0x01, digit1);
  sendPacket(0x02, digit2);
  sendPacket(0x03, digit3);

  delay(250);
}

void sendPacket(uint8_t address, uint8_t value) {
  // Ensure LOAD/CS is LOW
  digitalWrite(LOAD_PIN, LOW);

  // Send the register address
  SPI.transfer(address);

  // Send the value
  SPI.transfer(value);

  // Tell chip to load in data
  digitalWrite(LOAD_PIN, HIGH);
}
