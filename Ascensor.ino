/*ASCENSOR SOCIEDAD GASTRONOMICA

   Alberto Yebra García-Abad

   Versión 1.0 -
   Descripción: Sólo contempla dos pulsadores subir y bajar, y 3 relés

   Versión 1.1 - Se contemplan los finales de carrera y se modifica la máquina de estados para que controle la activación del motor hasta que corte el fin de carrera
   Salida se modifica el comportamiento respecto a la versión 1.0
   - Las salidas son R1 subir (activa la entrada de 220 hacia las 3 fases con el condensador, no hay cambio de giro) Acción Subir
   - La salida R2, activa ahora la alectrovalvula de 220, con ello se consigue bajar

  VERSION 1.3 - Se introduce la lógica de control de la puerta abierta.
              - Se introduce un counter en cada sentido de seguridad para que pare y pase a estdo 0 transcurrido un tiempo determinado
   ENTRADAS:
   I.1. PULSADOR SUBIR
   I.2. PULSADOR BAJAR
   I.3. SETA DE EMERGENICA
   I.4. FIN DE CARRERA PLANTA SUPERIOR
   I.5. FIN DE CARRERA PLANTA INFERIOR
   I.6. PUERTAS ABIERTAS.

   SALIDA
   R1 Subir
   R2 Bajar
   SUBIR
   N    F
   |    |---------------------- M1
   |    |
   |    |---| |---------------- M2
   |    |
   | -- | --------------------- M3

   BAJAR
   N    F
   |    |---| |---------------- M1
   |    |
   |    |---------------------- M2
   |    |
   | -- | --------------------- M3



   Telemando de 2 Relés



   ESTADOS DEL ASCENSOR


   SUBIR: Una vez pulsado el botón subir, y cumplir las condiciones de estado de reposo (
   I.1. PULSADOR SUBIR                    (1)
   I.2. PULSADOR BAJAR                    (0)
   I.3. SETA DE EMERGENICA                (0)
   I.4. FIN DE CARRERA PLANTA SUPERIOR    (0)
   I.5. FIN DE CARRERA PLANTA INFERIOR    (1 O)
   I.6. PUERTAS ABIERTAS.                 (0)
   SALIDAS
   R1 ONF
   R2 OFF

   BAJAR
   I.1. PULSADOR SUBIR                    (1)
   I.2. PULSADOR BAJAR                    (0)
   I.3. SETA DE EMERGENICA                (0)
   I.4. FIN DE CARRERA PLANTA SUPERIOR    (0)
   I.5. FIN DE CARRERA PLANTA INFERIOR    (1 O)
   I.6. PUERTAS ABIERTAS.                 (0)
   SALIDAS
   R1 OFF
   R2 ON


*/
//Definición de entradas
const int PulsadorSubir =  3;    //Implementado
const int PulsadorBajar =  4;    //Implementado
const int PulsadorSeta =  5;    //No utilizado

const int FinCarreraArriba =  1; //Implementado
const int FinCarreraAbajo =  0;   //Implementado
const int FinCarreraPuertas =  2;


const int Relay1 =  8; //R1       //Implementado
const int Relay2 =  9; //R2       //Implementado
//const int Relay3 =  10; //R3

unsigned long currentMillis = 0;
/*
  enum eESTADOS {
  REPOSO,     //0
  SUBIR,      //1
  BAJAR,      //3
  } estado;
*/
int estado = 0;
long previousMillis = 0;
long Activation = 5000;
long TiempodeSubida = 28000;
long TiempodeBajada = 35000;


int  ValuePulsadorSubir = 0;
int  ValuePulsadorBajar = 0;

int  ValueFCArriba = 0;
int  ValueFCAbajo = 0;
int  ValueFCPuerta = 0;
void AccionarEstado(int Est) {
  if (Est == 1) {
    subir();
  } else if (Est == 2) {
    bajar();
  } else {
    parar();

  }
}

int invertir(int Variable) {
  if (Variable == 1) {
    Variable = 0;
  }
  if (Variable == 0) {
    Variable = 1;
  }
  return Variable;
}

void subir() {
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, HIGH);

}
void bajar() {
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, LOW);

}
void parar() {
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
}

void setup() {
  // put your setup code here, to run once:
  estado = 0;

  //SETTING INICIAL

  pinMode(PulsadorSubir, INPUT);
  pinMode(PulsadorBajar, INPUT);
  pinMode(PulsadorSeta, INPUT);

  pinMode(FinCarreraArriba, INPUT);
  pinMode(FinCarreraAbajo, INPUT);
  pinMode(FinCarreraPuertas, INPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);

  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);

  Serial.begin(9600);

   //Inicio de la temporización
   
   
}

void loop() {
  // put your main code here, to run repeatedly:

  int ValuePulsadorSubir = digitalRead(PulsadorSubir);
  int ValuePulsadorBajar = digitalRead(PulsadorBajar);
  int ValueFCArriba = digitalRead(FinCarreraArriba);
  int ValueFCAbajo = digitalRead(FinCarreraAbajo);
  int ValueFCPuerta = digitalRead(FinCarreraPuertas);

  

  // INVERSOR EN FUNCIÓN COMO VENGAN LOS FINES DE CARRERA 
  //FC de abajo viene invertido
  ValueFCAbajo = !ValueFCAbajo;
  ValueFCArriba = !ValueFCArriba;

//  CONFIGURACIÓN DEL FIN DE CARRERA DE LA PUERTA

  ValueFCPuerta == 0;


  //VISUALIZACIÓN DE DATOS
  Serial.print("Estado: "); Serial.print(estado); Serial.print(" Pulsador Subir:"); Serial.print(ValuePulsadorSubir); Serial.print(" Pulsador Bajar:"); Serial.print(ValuePulsadorBajar);Serial.print(" FC Abajo:"); Serial.print(ValueFCAbajo);  Serial.print("  FC ARRIBA:"); Serial.println(ValueFCArriba);
   
  currentMillis = millis();
  //Serial.print("TIEMPO: "); Serial.print(currentMillis); Serial.print("DIFERENCIA: "); Serial.println(currentMillis-previousMillis);
 
  
  
  switch (estado) {

    case 1: { //SUBIENDO
        if (ValueFCArriba == 1 ) { //Sin pulsación o Puerta abierta
          estado = 0;
          
        }
        //Contamos
        if ( (currentMillis-previousMillis) >TiempodeSubida){
          estado=0;
          previousMillis = currentMillis; 
        }
        break;
      }

    case 2: { //BAJANDO
        if (ValueFCAbajo == 1 ) { //Sin pulsación o Puerta abierta
          estado = 0;
        }
        if ( (currentMillis-previousMillis) > TiempodeBajada){
          estado=0;
          previousMillis = currentMillis; 
        }

        break;
      }
    case 0: {
        if (ValuePulsadorSubir == 1 && ValuePulsadorBajar == 0 && !ValueFCArriba == 1 ) { //Pulsador subir
          //Chequeamos que condiciones de reposo son correctas y que no está abajo
          //
          previousMillis = currentMillis ;
          estado = 1;
          //Serial.println("subir");

        } else if (ValuePulsadorSubir == 0 && ValuePulsadorBajar == 1 && !ValueFCAbajo == 1 ) { //Pulsador BAJAR
          previousMillis = currentMillis ;
          //
          //
          estado = 2;
          //Serial.println("bajar");

        } else {
          estado = 0;
          previousMillis = currentMillis ;
        }

        break;
      }


  }

  AccionarEstado(estado);
}
