/*
 * 
 *      Esta sketh constroi um servidor HTTP que recebe comoando da posição de dois servomotores 
 *      , controla um carrinho
 *      ,atualiza os parâmetros no microcontrolador e retorna para o cliente status atualzado dos
 *      servo motores e sensores(temperatura,distância, luminosidade). Tudo isso sem necessidade
 *      de roteador, pois o próprio microcontrolador é um ponto de acesso cuja rede WiFi é 
 *      "Carrinho" e a senha é "arduino", o endereço ip de acesso o Wemos é 192.168.4.1
 * 
 *      Autor: Crelison Alves   Data de criação:27/12/2017    Data de aualização: --/--/----
 * 
 *      Placa de desenvolvimento Wemos D1/R1 -> Microcontrolador : ESP8266
 * 
 */


// Bibliotecas incluidas 

    #include <SPI.h>                  // inclui a biblioteca Serial
    #include <ESP8266WiFi.h>          // inclui a biblioteca Ethernet que habilita conexao Cliente/Servidor 
    #include "FS.h"                   // inclui a biblioteca da memmória flash
    #include <Servo.h>                // inclui a biblioteca de Servo motores
    #include <NewPing.h>              // Cria um objeto Sonar (ultrason)
    

// Rotulos e variaveis globais

    // Pinos de controle do carrinho
       int DireitoPoloA =  15;                // GPIO15 D10    fio cinza   -> Ponte-H B-1B
       int DireitoPoloB = 16;                 // GPIO16 D2     fio amarelo -> Ponte-H B-1A
       int EsquerdoPoloA = 5;                 // GPIO5  D3/D15 fio verde   -> Ponte-H A-1B
       int EsquerdoPoloB = 4;                 // GPIO4  D4/D14 fio azul    -> Ponte-H A-1A

       int motor = 0;                         // Status do motor: 0 -> parado; 1 -> ré; 2 -> frente; 3 -> esquerda; 4 -> direita
     
    // MAC (Media Access Control) edereço fisico de um dispositivo na rede
        byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    
    // Endereço IPv4 endeço logico do Wemos na rede local:
        IPAddress staticIP(192, 168, 4, 2); // IP set to Static
        IPAddress gateway(192, 168, 4, 1);// gateway set to Static
        IPAddress mascaraRede(255, 255, 255, 0);// subnet set to Static//IPAddress ip(192,168,4,1);
        WiFiServer server(80);

    // Configurações do Ponto de Acesso (rede wifi própria)
        const char* ssid = "Carrinho";
        const char* password = "arduino";

    // Arquivos 
        File arquivo;                // usado para carregar arquivos do cartão de memoria(HTML, imagem[ico,jpg],CSS,javaScript,JSON)

   // Variáveis de controle e prodessamento
        String url ="";              // usado para procurar o comando GET, arquivo solicitado e paramentro enviados
        int estadoURL = 0;           // guarda o íncice do String url 0 - > localisando GET, 1-> guarda nome do arquivo, 2 -> não definido
        int anguloHorizontal = 0 ,   // angulos de posicionemanto do servo motor 
            anguloVertical = 0,
            distancia =0;            // distância entre o sonar e um obstáculo, o valor é zero se estiver acime de MAX_DISTANCE ou menor que 2cm 
        bool memoriaFlash = false;   // Informa se o acesso a memória flash está liberado
        unsigned long tempo = millis();         // guarda o tempo em milissegundos para ativar a cada 100milisegundos o sonar

   //Pinos de controle do ultrason
        int TRIGGER_PIN = 13;  // Arduino pin GPIO 13 (D7) tied to trigger pin on the ultrasonic sensor.
        int ECHO_PIN    = 12;  // Arduino pin GPIO12 (D6) tied to echo pin on the ultrasonic sensor.
        unsigned int MAX_DISTANCE = 500; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
        NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance. 
   
   // Inicializa o Servomotor
        Servo servoHorizontal;  // cria um objeto Servo para controlar o movimento horizontal fi em coordenadas esfricas
        Servo servoVertical;  // cria um objeto Servo para controlar o movimento vertical(inclinacao) seria o teta em coordenadas esfricas
        int pinoServoHorizontal = 2;  // Reserva o GPIO2 (D9) do arduino para controlar o giro do servomotor da base
        int pinoServoVertical = 0;  // Reserva o GPIO0 (D8) do arduino para controlar o giro do servomotor responsavel pela inclinaçao
        int val;    // varivel que informa a posiço do servo motor entre 10 e 180

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {

        
      //Inicializa a comunicação com o monitor serial :
          Serial.begin(115200);
          while (!Serial) {
            ; // wait for serial port to connect. Needed for native USB port only
          }
          Serial.println("\n\rMonitor Serial instaldo\n\r\n\r");

      // Inicializando acesso a memória Flash
          Serial.println("\n\rInicializando acesso a memória Flash...");
          memoriaFlash = SPIFFS.begin();
          if (!memoriaFlash) {
              Serial.println("\n\rFalha no acesso a momória flash\n\r");
                 
          }else {
              Serial.println("\n\rAcesso a memória Flash inicializado com sucesso!\n\r");  
          }   

      // Inicializa a conexão wifi do microcontrolador ESP8266 e aguarda até que seja estabalecida a comunicação
          WiFi.mode(WIFI_AP);// Working mode only as Acess Point
          WiFi.softAP(ssid, password, 2, 0);          // ssid -> WiFi  | password -> senha  | canal 2  |  0 -> rede não oculta
          WiFi.config(staticIP, gateway, mascaraRede);
          server.begin();
          Serial.print("Criando ponto de acesso...");
          Serial.println();
          Serial.print("Srvidor iniciado, IP address: ");
          Serial.println(WiFi.softAPIP());
          
      // Configura o Servomotor
          servoHorizontal.attach(pinoServoHorizontal);  // associa o pino PWM do arduino pelo giro horizontal 
          servoVertical.attach(pinoServoVertical);  // associa o pino PWM responsavel pela inclinacao

      // inicializa o valor do sonar          
          distancia = sonar.ping_cm();

      // inicializando parâmetros dos motores do carrinho             
          pinMode(DireitoPoloA,OUTPUT);
          pinMode(DireitoPoloB,OUTPUT);
          pinMode(EsquerdoPoloA,OUTPUT);
          pinMode(EsquerdoPoloB,OUTPUT);
          digitalWrite(DireitoPoloA,LOW);
          digitalWrite(DireitoPoloB,LOW);
          digitalWrite(EsquerdoPoloA,LOW);
          digitalWrite(EsquerdoPoloB,LOW);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  


void loop() {



  if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
    tempo = millis();
    distancia = sonar.ping_cm();
  }
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {                         // INICIO -> Novo cliente conectado    
    //Serial.println("Novo clitente conectado ao Servidor Arduino");
    // an http request ends with a blank line
    if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
      tempo = millis();
      distancia = sonar.ping_cm();
    }    
    while (client.connected()) {        // INICIO -> Tratamento da requisição do cliente conectado
      if (client.available()) {         // INICIO -> Leitura dos dados enviados pelo cliente metodo GET
        char c = client.read();
        //Serial.write(c);
        if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
          tempo = millis();
          distancia = sonar.ping_cm();
        }       
        switch(estadoURL){
          case 0:             // INICIO -> procurar o comando GET

                              url += c;
                              if(url.indexOf("GET ")>=0){     // INICIO -> verificar se encontrou o GET  
                                  url="";
                                  estadoURL = 1;    // passa a procurar o nome do arquivo solicitado
                                  Serial.println("\n\rencontrei um GET");
                              }                               // FIM    -> verificar se encontrou o GET
          
          break;              // FIM    -> procurar o comando GET  
          case 1:             // INICIO -> procurar o nome do arquivo
                              url += c;
                              if(c==' '){     // INICIO -> composição do nome do arquivo
                                  estadoURL = 2;    // passa para o estado de disponibilizar o arquivo solicitado
                                  Serial.println("\n\ro arquivo solicitado foi #"+url+"#\n\r");
                                  
                              }                               // FIM    -> composição do nome do arquivo
                              if(c=='?'){     // INICIO -> Recupera parâmetros
                                  estadoURL = 3;    // passa a procurar o nome do arquivo solicitado
                                  Serial.println("\n\ro arquivo solicitado foi #"+url+"#\n\r");
                              }                              
                              
                    
          break;              // FIM    -> procurar o nome do arquivo
          case 2:             // INICIO -> disponibilizar o arquivo solicitado para o cliente
                              
                              if(url=="/ "||url=="index.htm "||url=="index.htlm "){     // INICIO -> formecer arquivo idex.html  
                                  // Envia a pagina inicial
                                        //Serial.println("Enviando arquivo index.html:");
                                        arquivo = SPIFFS.open("/index.html", "r");
                                        
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {


                                                client.write(arquivo,200); // Envia a página em blocos de 200 bytes
                                                        if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
                                                          tempo = millis();
                                                          distancia = sonar.ping_cm();
                                                        }

                                          }
                                          estadoURL = 0;    // prepara para a proxima solicitação http 
                                          url="";
                                          arquivo.close();
                                          client.flush();       // Esvazia o buffer de dados enviados pelo cliente
                                          client.stop();
                                          //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                                        }                        // FIM -> enviando arquivo     
                                        
                              }                                                              // FIM -> formecer arquivo idex.html    

                              if(url.indexOf("/logo.ico")>=0){                              // INICIO -> formecer imagem favico.ico  
                                  // Envia a pagina inicial
                                        Serial.println("Enviando arquivo logo.ico:");
                                        
                                        client.println("HTTP/1.1 200 OK");
                                        client.println("Content-Type: image/x-icon");
                                        client.println("Connection: close");  // the connection will be closed after completion of the response
                                        client.println();
                                        arquivo = SPIFFS.open("/logo.icol", "r");
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {
                                               client.write(arquivo,200); // send ico web page to client
                                          }
                                          arquivo.close();
                                        }                        // FIM -> enviando arquivo     
                                        estadoURL = 0;    // prepara para a proxima solicitação http 
                                        url="";
                                        // close the connection:
                                        client.flush();       // Esvazia o buffer de dados enviados pelo cliente
                                        client.stop();
                                        //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                              }                              // FIM -> formecer imagem favico.ico 
                              if(url.indexOf("/atualizar.js")>=0){     // INICIO -> atualizar 
                                  // Envia a pagina de atualização

                                                if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
                                                  tempo = millis();
                                                  distancia = sonar.ping_cm();
                                                }
                                        Serial.println("Inicio -> atualizar dados\n\r");                                                         // INICIO  --> atualizar
                                        client.println("HTTP/1.1 200 OK");
                                        client.println("Content-Type: application/json");
                                        client.println("Connection: close");  // the connection will be closed after completion of the response
                                        client.println("");                                        
                                        client.println("{");
                                        client.print("\t\"anguloHorizontal\":");
                                        client.print(anguloHorizontal);
                                        client.println(",");
                                        client.print("\t\"anguloVertical\":");
                                        client.print(anguloVertical);
                                        client.println(",");
                                        client.print("\t\"distancia\":");
                                        client.print(distancia);
                                        client.println(",");
                                        client.println("\t\"iluminancia\":10,");                                         
                                        client.println("\t\"temperatura\":5");
                                        client.println("}");
                                        estadoURL = 0;    // prepara para a proxima solicitação http
                                        url="";
                                        client.flush();       // Esvazia o buffer de dados enviados pelo cliente
                                        client.stop();   
                                        Serial.println("Fin atualizar")   ;                                      
                                       
                              }                                                                 //FIm -> atualizar
                              

          
          break;              // FIM    -> disponibilizar o arquivo solicitado para o cliente
          case 3:             // INICIO -> Pegar parametros enviados pelo cliente
                                        if(client.parseInt()==0){
                                          int N_argumentos = client.parseInt();
                                          for(int i = 1; i <= N_argumentos; i++){   // INICIO -> laço recuperar variáveis
                                            switch(client.parseInt()){    // reupera a variável que será atualizada
                                              case 1:                     // atualiza a variável -> anguloHorizontal
                                                      anguloHorizontal = client.parseInt();
                                              break;
                                              case 2:                     // atualiza a variável -> anguloVertical
                                                      anguloVertical = client.parseInt();
                                              break;
                                              case 3:                     // atualiza a variável -> anguloVertical
                                                      motor = client.parseInt();
                                                      Serial.println(motor);
                                              break;
                                            }
                                          
                                          }                                 // FIM    -> laço recuperar variáveis
                                        
                                        }
                                                if(abs(millis()-tempo)>100){  // atualiza o valor do sonar
                                                    tempo = millis();
                                                    distancia = sonar.ping_cm();
                                                }
                                        client.println("HTTP/1.1 200 OK");
                                        client.println("Content-Type: application/json");
                                        client.println("Connection: close");  // the connection will be closed after completion of the response
                                        client.println("");                                        
                                        client.println("{");
                                        client.print("\t\"anguloHorizontal\":");
                                        client.print(anguloHorizontal);
                                        client.println(",");
                                        client.print("\t\"anguloVertical\":");
                                        client.print(anguloVertical);
                                        client.println(",");
                                        client.print("\t\"distancia\":");
                                        client.print(distancia);
                                        client.println(",");
                                        client.println("\t\"iluminancia\":10,");                                         
                                        client.println("\t\"temperatura\":5");
                                        client.println("}");
                                        url="";
                                        arquivo.close();
                                        client.stop(); 
                                        estadoURL = 0;    // prepara para a proxima solicitação http
                                        // Atualiza a posição do servomotor
                                          val = anguloHorizontal+90;
                                          val = constrain(val,10,180);  
                                          servoHorizontal.write(val);      // sets the servo position according to the scaled value
                                          delay(15);
                                          val = -anguloVertical+90;
                                          val = constrain(val,10,170);  
                                          servoVertical.write(val);      // sets the servo position according to the scaled value
                                          delay(15);
                                        // Atualiza o estado de movimentação do carrinho
                                            switch(motor){
                                              case 0:           // carrinho parado
                                                digitalWrite(DireitoPoloA,LOW);
                                                digitalWrite(DireitoPoloB,LOW);
                                                digitalWrite(EsquerdoPoloA,LOW);
                                                digitalWrite(EsquerdoPoloB,LOW);
                                              break;
                                              case 1:           // carrinho -> RECUANDO
                                                digitalWrite(DireitoPoloA,LOW);
                                                digitalWrite(DireitoPoloB,HIGH);
                                                digitalWrite(EsquerdoPoloA,LOW);
                                                digitalWrite(EsquerdoPoloB,HIGH);
                                              break;
                                              case 2:           // carrinho -> AVANÇANDO
                                                digitalWrite(DireitoPoloA,HIGH);
                                                digitalWrite(DireitoPoloB,LOW);
                                                digitalWrite(EsquerdoPoloA,HIGH);
                                                digitalWrite(EsquerdoPoloB,LOW);
                                              break;
                                              case 3:           // carrinho -> ESQUERDA
                                                digitalWrite(DireitoPoloA,HIGH);
                                                digitalWrite(DireitoPoloB,HIGH);
                                                digitalWrite(EsquerdoPoloA,HIGH);
                                                digitalWrite(EsquerdoPoloB,LOW);
                                              break;
                                              case 4:           // carrinho -> DIREITA
                                                digitalWrite(DireitoPoloA,HIGH);
                                                digitalWrite(DireitoPoloB,LOW);
                                                digitalWrite(EsquerdoPoloA,HIGH);
                                                digitalWrite(EsquerdoPoloB,HIGH);
                                              break;
                                                
                                            }
          break;              // FIM    -> Pegar parametros enviados pelo cliente
          case 4:             // INICIO -> Não definido
          break;              // FIM    -> Não definido          
        }

        
        
      }                                 // FIM    -> Leitura dos dados enviados pelo cliente metodo GET   
    }                                   // FIM    -> Tratamento da requisição do cliente conectado          
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");

  }                                     // FIM    -> Novo cliente conectado               
}                                       // FIM    -> LOOP          

