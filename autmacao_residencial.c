#include <IRremote.h>
//banco de vriaveis 
#define VET_TAM 4
#define VET_ARG 2
#define DOOR_PIN 3
#define COLLER_PIN 4
#define LIGHT_PIN1 5
#define LIGHT_PIN2 6
#define FIRST_PIN 3
#define QTD_PINS 4
#define IR_PIN 13
byte comands[VET_TAM][VET_ARG];
IRrecv irrecv(IR_PIN);
decode_results decodedSignal;
int teclapressionada;

//funcao de troca de estados
void change (byte comands[VET_TAM][VET_ARG], byte x){
    comands[x][1]= !comands [x][1];
    digitalWrite (comands [x][0], comands[x][1]);
    //exclusividae de troca da porta (abertura e fechamento)
    if (comands[0][1]==1){
    comands[0][1]= !comands [0][1];
    delay(300);        
    digitalWrite (comands [0][0], comands[0][1]);      
    }
}
void setup () {
    //banco de variaveis
    int x, y=FIRST_PIN;
    //iniciando a leitura serial para modulo bluetooth
    Serial.begin (38400);
    //iniciando portas digitais
    for (x=0; x<VET_TAM; x++){
        comands[x][1]=0;
        comands[x][0] = y;
        pinMode (comands[x][0], OUTPUT);
        y++;
        }
    //iniciando leitor infravermelho
   irrecv.enableIRIn();
   
}


void loop(){
     //banco de variaveis
     char aux;
    int i;
    teclapressionada =-1;
    //leitura do modulo bluetooth
    if (Serial.available () >0){
       aux = (char) Serial.read ();
       }
     // o bluetooth envia chars, é diminuido 65 para a letra A representar o 0
    i= aux - 65;
    //interpretando o que foi lido no bluetooth
    if ((i<= QTD_PINS) && (i>=0) && (comands [i][1] == 0)){
        change (comands, i);
      } 
    if (((i-32)<= QTD_PINS) && ((i-32)>=0) && (comands [i-32][1] == 1)){
        change (comands, i-32);
      }
    //lendo o infravermelho   
    if (irrecv.decode(&decodedSignal)) {
     teclapressionada = (decodedSignal.value)-1;
        //Serial.println (decodedSignal.value);
     //interpretando comandos de turn on (0<=X<10)   
    if ((teclapressionada<10) && (comands[teclapressionada][1] == 0)){
        if (teclapressionada == 0 ){
         change (comands, 0);}
       else if (teclapressionada == 1){
        change (comands, 1);}
       else if (teclapressionada == 2){
        change (comands, 2);}
       else if (teclapressionada == 3){
        change (comands, 3);}
      }
     //interpretando comandos de turn OFF (X>=2048)   
     if ((teclapressionada>2047) && (comands[teclapressionada][1] == 1)){
        teclapressionada -= 2048;
        if (teclapressionada == 0 ){
         change (comands, 0);}
       else if (teclapressionada == 1){
        change (comands, 1);}
       else if (teclapressionada == 2){
        change (comands, 2);}
       else if (teclapressionada == 3){
        change (comands, 3);}
      }
     //limpando buffer do infra vermelho   
     irrecv.resume();
      }
    delay(300);
  }