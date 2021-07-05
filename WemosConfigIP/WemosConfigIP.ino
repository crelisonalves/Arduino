#define VELOCIDADE 115200

String REDE="BBC_SIBELLE";
String SENHA="13579864";

String IPLIVRE="192.168.1.100";
String MASCARA="255.255.255.0";
String ROTEADOR="192.168.1.1";

#include <SoftwareSerial.h>
SoftwareSerial monitorSerial(9, 8); // RX, TX

void cmdESP(String cmd, String msg="", int limite=7000) {
  // envia um comando para o ESP, aguarda, exibe e descarta a resposta
  if (msg!="") monitorSerial.println(msg);
  Serial.println(cmd);
  unsigned long chegada=millis();
  boolean continuar=true; 
  String S="";
  unsigned long ultimochar=0;
  while (continuar) { 
    if (Serial.available()) {
      char c = Serial.read();
      ultimochar=millis();
      S=S+c;
      if (c==10) {  // LF, fim da linha recebida
        byte p=S.indexOf(13);
        String S1=S.substring(0,p);
        if (S1=="OK") continuar=false;
        if (S1=="ready") continuar=false;
        if (S1=="no change") continuar=false;
        if (S1=="ERROR") continuar=false;
        monitorSerial.print(S);
        S="";
      }  
    }  
    if (millis()-chegada > limite) continuar=false;
  }
  if (S!="") monitorSerial.print(S);      
}


void setup() {
  monitorSerial.begin(VELOCIDADE);
  monitorSerial.println("*** ESP8266 Static IP / IP fixo - BR-Arduino.org");
  delay(2000);
  Serial.begin(VELOCIDADE);
  Serial.setTimeout(5000);

  String cmdConectar="AT+CWJAP=\""+REDE+"\",\""+SENHA+"\"";
  String cmdMudarIP="AT+CIPSTA_DEF=\"" + IPLIVRE + "\",\"" + ROTEADOR +"\",\"" + MASCARA + "\"";
  
  cmdESP("AT+CWMODE=3", "Modo de operacao misto, AP + STATION");
  cmdESP(cmdConectar,"Conectando a uma rede WiFi",30000);
  cmdESP("AT+CIFSR","Consultando os enderecos IP definidos inicialmente",10000);
  cmdESP(cmdMudarIP,"Definindo nosso proprio IP fixo");
  cmdESP("AT+CIFSR","Consultando novamente os enderecos IP, apos definicao acima.",10000);
  
  //cmdESP("AT+CWDHCP_DEF=1,1","Reativando o IP dinamico.");
}

void loop() {
}
