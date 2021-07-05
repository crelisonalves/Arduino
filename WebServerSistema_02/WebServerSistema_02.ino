/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 4,10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */
// Bibliotecas incluidas 

    #include <SPI.h>                  // inclui a biblioteca Serial
    #include <Ethernet.h>             // inclui a biblioteca Ethernet que habilita conexao Cliente/Servidor 
    #include <SD.h>                   // inclui a biblioteca SD que habilita o cartao micro SD
    //#include <Servo.h>                // 

// Rotulos e variaveis globais

    // MAC (Media Access Control) edereço fisico de um dispositivo na rede
        byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    
    // Endereço IPv4 endeço logico do arduino na rede local:
        IPAddress ip(192,168,1,100);

    // Arquivos 
        File arquivo;                 // usado para carregar arquivos do cartão de memoria(HTML, imagem[ico,jpg],CSS,javaScript,JSON)

   // Variáveis de controle e prodessamento
        String url =" ";       // usado para procurar o comando GET, arquivo solicitado e paramentro enviados
        int estadoURL = 0;     // guarda o íncice do String url 0 - > localisando GET, 1-> guarda nome do arquivo, 2 -> não definido
        int anguloHorizontal =0 ,anguloVertical = 0;    // angulos de posicionemanto do servo motor 

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
 // Serial.begin(9600);
 // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
 // }


        // Inicializando o cartao de memoria micro SD
        //Serial.println("Inicializando cartao micro SD...");
        // Na Shild Ethernet o pino de dados reservado para o micro SD  -> CS pino 4. 
        //pinMode(10, OUTPUT);
        //SD.begin(4);
        if (!SD.begin(4)) {
             // Serial.println("Falha no cartao SD");
             
        }else {
             // Serial.println("Cartao micro SD inicializado");  
        }   


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("O endereço do servidor Arduino é: ");
 // Serial.println(Ethernet.localIP());
}


void loop() {                           // INICIO -> LOOP              
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {                         // INICIO -> Novo cliente conectado    
    //Serial.println("Novo clitente conectado ao Servidor Arduino");
    // an http request ends with a blank line
     
    while (client.connected()) {        // INICIO -> Tratamento da requisição do cliente conectado
      if (client.available()) {         // INICIO -> Leitura dos dados enviados pelo cliente metodo GET
        char c = client.read();
        //Serial.write(c);
        
        switch(estadoURL){
          case 0:             // INICIO -> procurar o comando GET

                              url += c;
                              if(url.indexOf("GET ")>=0){     // INICIO -> verificar se encontrou o GET  
                                  url=" ";
                                  estadoURL = 1;    // passa a procurar o nome do arquivo solicitado
                                  //Serial.println("\n\rencontrei um GET");
                              }                               // FIM    -> verificar se encontrou o GET
          
          break;              // FIM    -> procurar o comando GET  
          case 1:             // INICIO -> procurar o nome do arquivo
                              url += c;
                              if(c==' '){     // INICIO -> composição do nome do arquivo
                                  estadoURL = 2;    // passa para o estado de disponibilizar o arquivo solicitado
                                 // Serial.println("\n\ro arquivo solicitado foi #"+url+"#\n\r");
                                  
                              }                               // FIM    -> composição do nome do arquivo
                              if(c=='?'){     // INICIO -> Recupera parâmetros
                                  estadoURL = 3;    // passa a procurar o nome do arquivo solicitado
                                 // Serial.println("\n\ro arquivo solicitado foi #"+url+"#\n\r");
                              }                              
                              
                    
          break;              // FIM    -> procurar o nome do arquivo
          case 2:             // INICIO -> disponibilizar o arquivo solicitado para o cliente
                              
                              if(url==" / "||url==" index.htm "||url==" index.htlm "){     // INICIO -> formecer arquivo html, css e java script  
                                  // Envia a pagina inicial
                                        //Serial.println("Enviando arquivo index.htm:");
                                        arquivo = SD.open("/arduino/sistema/index4.htm");
                                        if (arquivo) {           // INICIO -> enviando arquivo HTML
                                          while(arquivo.available()) {
                                               client.write(arquivo.read()); // send web page to client
                                          }
                                          estadoURL = 0;    // prepara para a proxima solicitação http 
                                          url=" ";
                                          arquivo.close();
                                          client.stop();
                                          //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                                        }                        // FIM -> enviando arquivo HTML    

                              }                                                              // FIM -> formecer arquivo html, css e java script    

                              if(url.indexOf(".ico")>=0){                              // INICIO -> formecer imagem favico.ico  
                                  // Envia a pagina inicial
                                        //Serial.println("Enviando arquivo favico.ico:");
                                        
                                        client.println("HTTP/1.1 200 OK");
                                        client.println("Content-Type: image/x-icon");
                                        client.println("Connection: close");  // the connection will be closed after completion of the response
                                        client.println();
                                        arquivo = SD.open("/arduino/icones/robot0.ico");
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {
                                               client.write(arquivo.read()); // send ico web page to client
                                          }
                                          arquivo.close();
                                        }                        // FIM -> enviando arquivo     
                                        estadoURL = 0;    // prepara para a proxima solicitação http 
                                        url=" ";
                                        // close the connection:
                                        client.stop();
                                        //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                              }                              // FIM -> formecer imagem favico.ico 
                              if(url.indexOf("json.txt")>=0){     // INICIO -> JSON 
                                  // Envia a pagina inicial
                                        
                                        arquivo = SD.open("/arduino/sistema/json.txt");
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {
                                               client.write(arquivo.read()); // send web page to client
                                               //Serial.println("Enviando arquivo dados.json:");
                                          }
                                          url=" ";
                                          arquivo.close();
                                          client.stop();
                                          //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                                        }                        // FIM -> enviando arquivo     
                                       
                              }                                                                 //FIm -> JSON
                              if(url.indexOf("ajax.js")>=0){                                    // INICIO -> AJAX 
                                  // Envia a pagina inicial
                                        
                                        arquivo = SD.open("/arduino/sistema/ajax.js");
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {
                                               client.write(arquivo.read()); // send web page to client
                                               //Serial.println("Enviando arquivo dados.json:");
                                          }
                                          url=" ";
                                          arquivo.close();
                                          client.stop();
                                          //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                                        }                        // FIM -> enviando arquivo     
                                       
                              }                                                                 //FIm -> AJAX                              
                              if(url.indexOf("designer.css")>=0){                                    // INICIO -> AJAX 
                                  // Envia a pagina inicial
                                        
                                        arquivo = SD.open("/arduino/sistema/designer.css");
                                        if (arquivo) {           // INICIO -> enviando arquivo
                                          while(arquivo.available()) {
                                               client.write(arquivo.read()); // send web page to client
                                               //Serial.println("Enviando arquivo dados.json:");
                                          }
                                          url=" ";
                                          arquivo.close();
                                          client.stop();
                                          //Serial.println("Cliente desconectado pelo Arduino\n\r\n\r");
                                        }                        // FIM -> enviando arquivo     
                                       
                              }                                                                 //FIm -> AJAX  
          
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
                                            }
                                          
                                          }                                 // FIM    -> laço recuperar variáveis
                                        
                                        }
                                        client.println("HTTP/1.1 200 OK");
                                        client.println("Content-Type: application/json");
                                        client.println("Connection: close");  // the connection will be closed after completion of the response
                                        client.println();                                        
                                        client.println("{");
                                        client.print("\t\"anguloHorizontal\":");
                                        client.print(anguloHorizontal);
                                        client.println(",");
                                        client.print("\t\"anguloVertical\":");
                                        client.print(anguloVertical);
                                        client.println(",");
                                        client.println("\t\"distancia\":15,");
                                        client.println("\t\"iluminancia\":10,");                                         
                                        client.println("\t\"temperatura\":5");
                                        client.println("}");
                                        url=" ";
                                        arquivo.close();
                                        client.stop(); 
                                        estadoURL = 0;    // prepara para a proxima solicitação http 
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

