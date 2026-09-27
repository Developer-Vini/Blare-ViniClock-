//I didn't write the code in English because I was focused on the code itself; translating everything would have taken too long.'

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include "NotasMusicais.h"

#define TFT_SCLK 9
#define TFT_MOSI 10
#define TFT_RST 8
#define TFT_DC 4
#define TFT_CS 5
#define TFT_BL 21

#define BTN_SELECT 5
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

int w = 240
int h = 320

int menuIndex = 0;
const char *menuItens[] = {
    "Ativar/Desativaor",
    "Ajustar Hora",
    "Ajustar Minuto"."Voltar"};

const int totalMenuItens = 4;

int melodia[] = {
    NOTE_E5,
    NOTE_D5, 
    NOTE_FS4, 
    NOTE_GS4, 
    NOTE_GS4, 
    NOTE_CS5, 
    NOTE_B4, 
    NOTE_D4, 
    NOTE_E4, 
    NOTE_B4, 
    NOTE_A4, 
    NOTE_CS4, 
    NOTE_E4, 
    NOTE_A4, 
    NO_SOUND
} 
int totalNotas = 15;

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, 255)

  tft.init(w, h);
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
      tft.fillScreen(COR_DE_FUNDO);
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
      tft.fillScreen(COR_DE_FUNDO);
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
  tft.print("Blare Vini");
  delay(1200);
  tft.fillScreen(COR_DE_FUNDO)
}
void conectarWiFi(){
  tft.setCursor(10, 10);
  tft.setTextColor(COR_DATA);
  tft.setTextSize(1);
  tft.print("Conectando no WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int tentativas = 0;
  //tft.setTextSize()
  while(WiFi.status != WL_CONNECTED && tentativas = 10){
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if(WiFi.status() == WL_CONNECTED){
    confiTime(GMT_OFSSET_S, DST_OFFSET_S, NTP_SERVER);
    tft.fillScreen(COR_DE_FUNDO);
  }else{
    tft.fillScreen(COR_DE_FUNDO);
    tft.setCursor(10, 10);
    tft.setTextColor(COR_ALARME_OFF);
    tft.print("Sem WiFi - hora manual");
    delay(1500);
    tft.fillScreen(COR_DE_FUNDO);
  }
}
void desenharTelaRelogio(){
    tft.fillRect(0, 0, w, h, COR_DE_FUNDO);

    tft.setTextSize(5);
    tft.setTextColor(COR_HORA);
    tft.setCursor(30, 80);
    if(temHora) {
      if(agora.tm_hour < 10) tft.print("0");
      tft.print(agora.tm_hour);
      tft.print(":");
      if(agora.tm_min < 10) tft.print("0");
      tft.print(agora.tm_min);
    }else{
      tft.print("--:--");
    }

    tft.setTextSize(1);
    tft.setTextColor(COR_DATA);
    tft.setCursor(60, 140);
    if(temHora){
      tft.printf("%02d/%02d/%04d", agora.tm_mday, agora.tm_mon + 1, agora.tmm_year + 1900);
    }

    tft.setCursor(50, 200);
    tft.setTextColor(alarmeAtivo ? COR_ALARME_ON : COR_ALARME_OFF);
    tft.printf("Alarme %02d/%02d %s", alarmeHora, alareMinuto, alarmeAtivo ? "ON" : "OFF");
}
void desenharmenu(){
  tft.fillRect(0,0, w,h, COR_DE_FUNDO);
  tft.setTextSize(2);
  tft.setCursor(50, 10);
  tft.setTextColor(COR_DESTAQUE);
  tft.print("MENU");

  for(int i=0;i<totalMenuItens;i++){
    tft.setCursor(20, 50 + i * 30);
    tft.setTextColor(i == menuIndex ?COR_DESTAQUE : COR_DATA);
    tft.print(i == menuIndex ? ">":"");
    tft.print(menuItens[i]);
  }
} 
void executarOpcaoMenu() {
  switch(menuIndex){
    case 0:
      alarmeAtivo = !alarmeAtivo;
    break;
    case 1:
      telaAtual = TELA_AJUSTE_ALARME;
      tft.fillScreen(COR_DE_FUNDO);
    break;
    case 2:
      telaAtual = TELA_AJUSTE_ALARME;
      tft.fillScreen(COR_DE_FUNDO);
    break;
    case 3:
      telaAtual = TELA_RELOGIO;
      tft.fillScreen(COR_DE_FUNDO);
      break;
    }
}
void desenharAjusteAlarme() {
  tft.fillRect(0, 0, 240, 240, COR_DE_FUNDO);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.setTextColor(COR_DESTAQUE);
  tft.print("Ajustar Alarme");

  tft.setTextSize(4);
  tft.setCursor(50, 100);
  tft.setTextColor(COR_HORA);
  tft.printf("%02d:%02d", alarmeHora, alarmeMinuto);

  tft.setTextSize(1);
  tft.setCursor(20, 200);
  tft.setTextColor(COR_DATA);
  tft.print("UP/DOWN: minuto | SELECT: voltar");
}

void tocarAlarme() {
  tft.fillScreen(COR_ALARME_OFF);
  tft.setTextSize(3);
  tft.setTextColor(0xFFFF);
  tft.setCursor(30, 90);
  tft.print("ACORDA!");

  for (int i = 0; i < totalNotas; i++) {
    tone(BUZZER_PIN, melodia[i], 250);
    unsigned long inicio = millis();
    while (millis() - inicio < 300) {
      if (digitalRead(BTN_BACK) == LOW || digitalRead(BTN_SELECT) == LOW) {
        noTone(BUZZER_PIN);
        jaTocouNesseMinuto = true;
        telaAtual = TELA_RELOGIO;
        tft.fillScreen(COR_DE_FUNDO);
        return;
      }
    }
  }
  noTone(BUZZER_PIN);
}
bool botaoPressionado(int pino) {
  if (digitalRead(pino) == LOW && (millis() - ultimoDebounce) > debounceDelay) {
    ultimoDebounce = millis();
    return true;
  }
  return false;
}