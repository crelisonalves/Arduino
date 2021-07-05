/*
 *     Autor: Crelison Alves      Data de criação 04/12/2017    Data de atualização: --/--/----
 * 
 *     Este programa mostra um relatório da memória flash e exibe todos os arquivo do usuária no micro controlador ESP8266
 * 
 *     Placa de desenvolvimento: Wemos D1/R1 ESP8266
 * 
 */


// Bibliotecas

  #include "FS.h"


// Rótulos e variáveis globais

  long total, disponivel;

//////////////////////////////////////////////////////////////////////////////////////////////////
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPIFFS.begin();
  delay(1);
  
  // Mostra o diagnóstico da memória flash
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    delay(1);
    Serial.println("\n\r\n\rDados da Memória Flash (bytes)\n\r");    
    Serial.print("Total:\t");
    total = fs_info.totalBytes;
    Serial.println(total);
    Serial.print("Usada:\t"); 
    delay(1);
    disponivel = fs_info.usedBytes;  
    Serial.println(disponivel);
    Serial.print("N máximo permitido de arquivos\n\rabertos simultâmente:\t");
    Serial.println(fs_info.maxOpenFiles);
    Serial.print("Tamanho máximo do nome de arquivo:\t");  
    delay(1);
    Serial.println(fs_info.maxPathLength);   
    char c = 2;
    Serial.println("Mapa da Memória");
    Serial.println("");
    Serial.print(c); 
    Serial.print(" -> memória livre     |     # -> memória usada ");  
    Serial.print((float)disponivel*100/total);
    Serial.println("%");
    
   
    for(long i=1, usado = disponivel*99*10/total;i< 99*10+1 ;i++){


       if(i<usado){
        Serial.print("#");
       }else {
        Serial.print(c);
       }
       if(i%99==0&&i>0) {
          
          Serial.print("\n\r");
       }
       
    }

  
  // Exibe todos os arquivos do usuário na memória flash
    
    delay(1);
    Dir dir = SPIFFS.openDir("/");
    Serial.println("\n\rArquivos do usuário armazenados na memória:");    
    Serial.println("\n\rNome\t\ttamanho(bytes)");
    while (dir.next()) {
      Serial.print(dir.fileName()+"\t");
      delay(1);
      File f = dir.openFile("r");
     Serial.println(f.size());
    }

    
    SPIFFS.end();
    Serial.println("\n\r Relatório concluido\n\r");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
  // put your main code here, to run repeatedly:

}
