// INCLUSÃO DE BIBLIOTECAS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DEFINIÇÕES
#define DESCENDO  0
#define PARADO    1
#define SUBINDO   2
#define FOGO      4

// DECLARAÇÃO DE FUNÇÕES
void exibeDisplay(byte mensagem, byte andar);
void setaPraCima(byte coluna);
void setaPraBaixo(byte coluna);

//INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DECLARAÇÃO DE VARIÁVEIS
byte PontaCE[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B00011,
  B00011,
  B00011
};
byte PontaCD[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11111,
  B11000,
  B11000,
  B11000
};
byte PontaBE[8] = {
  B00011,
  B00011,
  B00011,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001
};
byte PontaBD[8] = {
  B11000,
  B11000,
  B11000,
  B11111,
  B11110,
  B11100,
  B11000,
  B10000
};

byte CorpoE[8] = {
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011
};
byte CorpoD[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

// IMPLEMENTO DE FUNÇÕES
void configuraDisplay() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(1, PontaCE);
  lcd.createChar(2, PontaCD);
  lcd.createChar(3, PontaBE);
  lcd.createChar(4, PontaBD);
  lcd.createChar(5, CorpoE);
  lcd.createChar(6, CorpoD);

  lcd.home();
  lcd.print("Brincando Com");
  lcd.setCursor(0, 1);
  lcd.print("Ideias");
  delay(2000);
  lcd.clear();
}

void exibeDisplay(byte mensagem, byte andar) {
  switch (mensagem) {
    case DESCENDO: {
        lcd.clear();
        lcd.home();
        lcd.print("Descendo");
        setaPraBaixo(14);
        lcd.setCursor(0, 1);
        lcd.print("Ate andar: ");
        if (andar == 0) lcd.print("T");
        else lcd.print(andar);
        break;
      }
    case PARADO: {
        lcd.clear();
        lcd.home();
        lcd.print("Elevador parado");
        lcd.setCursor(0, 1);
        lcd.print("Andar atual: ");
        if (andar == 0) lcd.print("T");
        else lcd.print(andar);
        break;
      }
    case SUBINDO: {
        lcd.clear();
        lcd.home();
        lcd.print("Subindo");
        setaPraCima(14);
        lcd.setCursor(0, 1);
        lcd.print("Ate andar: ");
        lcd.print(andar);
        break;
      }
    case FOGO: {
        lcd.clear();
        lcd.home();
        lcd.print("INCENDIO!!");
        setaPraBaixo(14);
        lcd.setCursor(0, 1);
        lcd.print("Descendo");
        break;
      }
  }
}

void setaPraCima(byte coluna) {
  lcd.setCursor(coluna, 0);
  lcd.write(1);
  lcd.setCursor(coluna + 1, 0);
  lcd.write(2);
  lcd.setCursor(coluna, 1);
  lcd.write(5);
  lcd.setCursor(coluna + 1, 1);
  lcd.write(6);
}

void setaPraBaixo(byte coluna) {
  lcd.setCursor(coluna, 1);
  lcd.write(3);
  lcd.setCursor(coluna + 1, 1);
  lcd.write(4);
  lcd.setCursor(coluna, 0);
  lcd.write(5);
  lcd.setCursor(coluna + 1, 0);
  lcd.write(6);
}
