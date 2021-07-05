/*

Autor: Crelison Alves
Data de criação: 25/06/2017
Data de atualização: - / - / - 
Título: Led tricolor com ânodo de modo coum coom cores e intensidade luminosa variáves
Plataforma: Arduino Uno

Resumo:

Este programa usa o código GRAY modificado como base para mudança de estados, onde cada bit representa a cor que será afetada, o led acende e apaga usando as portas
analógicas associadas aos pinos 3, 6 e 9. O valor lócico 1 indica o o led será ligado, caso contrário será apagado, conforme tabeta do códgo GRAY modificado.
Como o led é do tipo ânodo de modo comum e a porta usada é a analógica o valor 255 ésudo para manter o led apagado e o valor 0 para acender o led, e os valores intermediários 
para causar a sensação que o mesmo está acendendo ou apagando.



                                                        ----------        2 ----------|>|------------------1
                                                        |        |               |----|>|------------------3
                                                        |        |               |----|>|------------------4
                                                        |        |
                                                        |        |      Led tricolor com ânodo de modo comum( o pino com o terminal mais extenso é o ânodo) 
                                                        ----------
                                                          | | | |       1 - Terminal da cor Vermelho (Pino 9       : Arduino)
                                                          | | | |       2 - Cátodo                   (Terminal Vcc : Arduino)
                                                          | | | |       3 - Terminal da cor Verde    (Pino 6       : Arduino)
                                                            |           4 - Terminal da cor Azul     (Pino 3       : Arduino)
                                                          1 2 3 4

      transição         Código GRAY modificado  : Cores dos leds ( 0 -> apagado  ; 1 -> aceso )

                        Cores   Vermelho  Verde  Azul
                        Bit        3        2     1
      estadoDoBrilho
                                   0        0     0
                1                  0        0     1
                                   0        0     0
                2                  0        1     1
                                   0        0     0
                3                  0        1     0
                                   0        0     0
                4                  1        1     0
                                   0        0     0
                5                  1        1     1
                                   0        0     0
                6                  1        0     1
                                   0        0     0
                7                  1        0     0
                                   0        0     0

  Iremos usar a função analogWrite(porta,brilho_Do_Led) para aceder os leds .

 */

// Declaração das variáveis

    //portas dos leds
    int ledVermelho = 9;              //  aponta para a porta do pino de nº 9 : LED vermelho
    int ledVerde    = 6;              //  aponta para a porta do pino de nº 6 : LED Verde
    int ledAzul     = 3;              //  aponta para a porta do pino de nº 3 : LED Azul
    const int ledPin =  LED_BUILTIN;  // led interno do Arduino UNO associado ao pino13

    //Intensidade luminoso ( Valor : 255 -> Led apagado ; 0 -> Led aceso)
    int brilhoLedVermelho = 255;    // Led Vermelho
    int brilhoLedVerde    = 255;    // Led Verde
    int brilhoLedAzul     = 255;    // Led Azul

    int estadoDoBrilho = 1;         // controla qual grupo de cores será selecionado, veja a tabela GRAY modificado
    int contador = 0;               // conta de 0 até 255 ( onde os leds são acesos e de 256 até 510 onde os leds são apapagados gradativamente)
    int Tempo  = 10;                // tempo de espera de cada transição em milisegundo


// O setup, que é executado quando o microcontrolador é ligado ou pressionado o botão de reset:

    void setup() {
    // Informa ao microcontrolador as portas que serão usadas e em seguida são inicializados
    pinMode(ledVermelho, OUTPUT);
    pinMode(ledVerde, OUTPUT);
    pinMode(ledAzul, OUTPUT);
    pinMode(ledPin, OUTPUT);
    analogWrite(ledVermelho, brilhoLedVermelho);
    analogWrite(ledVerde, brilhoLedVerde);
    analogWrite(ledAzul, brilhoLedAzul);
    digitalWrite(ledPin, estadoDoBrilho%2);

    // Espera o valor de Tempo milisegundos para dar continuidade no proceso de transição de cores
    delay(Tempo);   
}

// the loop routine runs over and over again forever:
void loop() {
    
  switch(estadoDoBrilho){  
    
   case 1:
       
      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedAzul--;      // Aumenta a intensidade luminosa do led azul
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedAzul++;      // Aumenta a intensidade luminosa do led azul
         
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(brilhoLedAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
      }
        
      break;
       
 
      case 2:

      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedAzul--;      // Aumenta a intensidade luminosa do led azul
         brilhoLedVerde--;     // Aumenta a intensidade luminosa do led verde
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedAzul++;      // Diminue a intensidade luminosa do led azul
         brilhoLedVerde++;     // Diminue a intensidade luminosa do led verde         
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
      }

      break;

      case 3:

      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedVerde--;     // Aumenta a intensidade luminosa do led verde
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedVerde++;     // Diminue a intensidade luminosa do led verde 
       
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600); 
      }
     
      break;


      case 4:
    
      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedVerde--;     // Aumenta a intensidade luminosa do led verde
         brilhoLedVermelho--;  // Aumenta a intensidade luminosa do led vermelho
         
      } else if(contador <= 510){

         contador++;           // Incrementa o contador
         brilhoLedVerde++;     // Diminue a intensidade luminosa do led verde 
         brilhoLedVermelho++;  // Diminue a intensidade luminosa do led vermelho       
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
      }

      break;


      case 5:

      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedAzul--;      // Aumenta a intensidade luminosa do led azul
         brilhoLedVerde--;     // Aumenta a intensidade luminosa do led verde
         brilhoLedVermelho--;  // Aumenta a intensidade luminosa do led vermelho
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedAzul++;      // Diminue a intensidade luminosa do led azul
         brilhoLedVerde++;     // Diminue a intensidade luminosa do led verde 
         brilhoLedVermelho++;  // Diminue a intensidade luminosa do led vermelho       
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
      }

      break;

      case 6:

      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedAzul--;      // Aumenta a intensidade luminosa do led azul
         brilhoLedVermelho--;  // Aumenta a intensidade luminosa do led vermelho
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedAzul++;      // Diminue a intensidade luminosa do led azul
         brilhoLedVermelho++;  // Diminue a intensidade luminosa do led vermelho       
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho++;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
         
      }
      
      break;

      case 7:

      // reverse the direction of the fading at the ends of the fade:
      if (contador < 255 ) {  //Aumenta a intensidade de brilho dos leds afetados
        
         contador++;           // Incrementa o contador
         brilhoLedVermelho--;  // Aumenta a intensidade luminosa do led vermelho
         
      } else if(contador < 510){

         contador++;           // Incrementa o contador
         brilhoLedVermelho++;  // Diminue a intensidade luminosa do led vermelho       
      } else {

         // Prepara as condições necessárias para dar início à próxima transição de cor
         contador = 0;
         int brilhoLedVermelho = 255;    // Led Vermelho
         int brilhoLedVerde    = 255;    // Led Verde
         int brilhoLedAzul     = 255;    // Led Azul
         estadoDoBrilho = 1;
         analogWrite(ledVermelho, brilhoLedVermelho);
         analogWrite(ledVerde, brilhoLedVerde);
         analogWrite(ledAzul, brilhoLedAzul);
         digitalWrite(ledPin, estadoDoBrilho%2);
         delay(600);
         
      }
       
       break;
  
  }

  
    analogWrite(ledVermelho, brilhoLedVermelho);
    analogWrite(ledVerde, brilhoLedVerde);
    analogWrite(ledAzul, brilhoLedAzul);
    digitalWrite(ledPin, estadoDoBrilho%2);

    // Espera o valor de Tempo milisegundos para dar continuidade no proceso de transição de cores
    delay(Tempo);
}
