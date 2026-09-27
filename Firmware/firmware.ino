#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"

#define TFT_SCLK 9
#define TFT_MOSI 10
#define TFT_RST 8
#define TFT_DC 4
#define TFT_CS 5
#define TFT_BL 21

#define BT_SELEC 5
#define BTN_UP 4
#define BTN_DOWN 3
#define BTN_BACK 2

#define BUZZER_PIN 20

const char *WIFI_SSID = "";
const char *WIFI_PASSWORD = "";
const char *NTP_SERVER = "";
const long GMT_OFSSET_S = -3 * 3600;
const int DST_OFFSET_S = 0;

#define COR_DE_FUNDO 0x0000
#define COR_HORA 0x07FF
#define COR_DATA 0xFFF
#define COR_DESTAQUE 0xFFE0
#define COR_ALARME_ON 0x07E0
#define COR_ALARME_OFF 0xF800

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Tela telaAtual = TELA_RELOGIO;

enum Tela
{
  TELA_RELOGIO,
  TELA_MENU,
  TELA_AJUSTE_ALARME,
  TELA_TOCANDO
};

int alarmeHora = 7;
int alareMinuto = 0;
bool alarmeAtivo = true;
bool jaTocouNesseMinuto = false;

int menuIndex = 0;
const char *menuItens[] = {
    "Ativar/Desativaor",
    "Ajustar Hora",
    "Ajustar Minuto"."Voltar"};

const int totalMenuItens = 4;

int melodia[] = {
    // Depois eu crio uma melodia
} int totalNotas;

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, 255)

      tft.init(240, 320);
  tft.setRotation(2);
  tft.fillScren(COR_DE_FUNDO);

  mostrarSplash();
  conectarWiFi();
}

void loop(){
  // put your main code here, to run repeatedly:
  struct tm agora;
  bool temHora = getLocalTime(&agora);

  switch (telaAtual){
  case TELA_RELOGIO:
    desenharTelaRelogio(agora, temHora);
    if (botaoPressionado(BTN_SELECT)){
      telaAtual = TELA_MENU;
      tft.fillScreen(COR_FUNDO);
    }
  }else{
    jaTocouNesseMinuto = false;
  }
  break;

  case TELA_AJUSTE_ALARME:
    desenharMenu();
    if (botaopressionado(BTN_UP)) menuIndex = (menuIndex - 1 + totalMenuItens) % totalMenuItens;
    if (botaopressionado(BTN_DOWN)) menuIndex = (menuIndex + 1) % totalMenuItens;
    if (botaopressionado(BTN_SELECT)){
      executarOpcaoMenu();
    }
    if (botaopressionado(BTN_BACK)){
      telaAtual = TELA_RELOGIO;
      tft.fillScreen(COR_FUNDO);
    }
  break;

  case TELA_TOCANDO:
    tocarAlarme();
    break;
}


void mostrarSplash(){
  tft.setTextColor(COR_DESTAQUE);
  tft.setTextSize(2)
  tft.setCursor(20, 100);
  tft.print
}
void conectarWiFi() {}
void desenharTelaRelogio() {}
void botaoPressionado() {}
void desenharmenu() void executarOpcaoMenu() {}
void tocarAlarme();
