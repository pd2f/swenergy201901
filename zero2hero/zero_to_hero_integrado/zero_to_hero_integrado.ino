//Programa: Sensor de som KY-038
//Autor: Arduino e Cia

#define PinoA0 11
#define PinoA1 12
#define PinoA2 2
#define PinoA3 6
#define pino_led1 9
#define pino_led2 10
#define pino_seletor 13
//Definicao pinos leds
int pinoled_ver = 3;
int pinoled_ama = 4;
int pinoled_verm = 5;

//Definicao pinos sensor
int pino_analogico = A5;
int pino_digital = 7;
int valor_A0 = 0;
int valor_D = 0;
int contador = 0;

int leituras[20];
int grupos_status[10];
int grupo_selecionado = 0;

//Criterio de fital
int chegada = 0;
void setup()
{
  Serial.begin(9600);
  Serial.println("Inicio");
  //Define pinos led como saida
  pinMode(pinoled_ver, OUTPUT);
  pinMode(pinoled_ama, OUTPUT);
  pinMode(pinoled_verm, OUTPUT);
  //Define pinos sensor como entrada
  pinMode(pino_analogico, INPUT);
  pinMode(pino_digital, INPUT);
  //Set pino fita
  pinMode(pino_led1, OUTPUT);
  //Set fita 1 desligada
  digitalWrite(pino_led1, LOW);
  digitalWrite(pino_led2, LOW);
  //  Pinos do display
  pinMode(PinoA0, OUTPUT);
  pinMode(PinoA1, OUTPUT);
  pinMode(PinoA2, OUTPUT);
  pinMode(PinoA3, OUTPUT);
  //  Pino Seletor
  pinMode(pino_seletor, INPUT);
  chegada = 0;
}

void loop()
{
  valor_A0 = analogRead(pino_analogico);
  valor_D = digitalRead(pino_digital);
  if (digitalRead(pino_seletor) == HIGH) {
    delay(2000);
    Serial.println("pino seletor acionado");
    selecionaGrupo();
    chegada=0;
  }
      monitor_serial();
  //Condição de promocao de fase.
  chegada += equalizer(valor_D);
  if (chegada == 20) {
    addGrupoStatus(grupo_selecionado);
    setFita(grupo_selecionado);
    Serial.print("1:");
    Serial.println(chegada);
    chegada = 0;
    Serial.print("Limpa:");
    Serial.print(chegada);
    delay(1000);
  }
}

void selecionaGrupo() {
  if (grupo_selecionado > 9) {
    grupo_selecionado = 0;
  }
  else {
    Serial.print("Grupo Selecionado:");
    Serial.println(grupo_selecionado + 1);
    setGrupoDisplay(++grupo_selecionado);
    setFita(grupo_selecionado);
  }
}

void addGrupoStatus(int grupo) {
  if (grupo != 0) {
    grupos_status[grupo] += 1;
  }
  Serial.print("Grupo Status:");
  Serial.println(grupos_status[grupo]);
}

void setFita(int grupo) {
  digitalWrite(pino_led1, LOW);
  digitalWrite(pino_led2, LOW);
  if (grupo != 0) {
    if (grupos_status[grupo] == 1) {
      digitalWrite(pino_led1, HIGH);
      delay(10000);
    }
    if (grupos_status[grupo] == 2) {
      digitalWrite(pino_led1, HIGH);
      digitalWrite(pino_led2, HIGH);
      delay(10000);
    }
  }
}

void monitor_serial() {
  if (valor_D == 1) {
    Serial.print("Saida A0: ");
    Serial.print(valor_A0);
    Serial.print(" Saida D0: ");
    Serial.println(valor_D);
    Serial.print("Chegada:");
    Serial.print(chegada);
  }
}

int equalizer(int digital) {
  int retorno = 0;
  if (digital == 1) {
    float media = 0;
    //    Serial.println("Pino digital ");
    leituras[contador++] = valor_A0 ;
    //    Serial.print("contador: ");
    //    Serial.println(contador);
    //    Serial.print("Leituras: ");
    //    Serial.println(leituras[contador]);
    //    Prospeccao de valores
    if (contador >= 21) {
      contador = 0;
    }
    //    Serial.print("Qtde. Leituras: ");
    //    Serial.println(sizeof(leituras));
    if (contador == 20) {
      float sum = 0;
      media = 0;
      for (int x = 0; x < 20; x++) {
        sum = sum + leituras[x];
      }
      media = sum / (contador - 1);
      //Debug
      //      Serial.print("Media: ");
      //      Serial.println(media);
      //      Serial.print("Milisegundos");
      //      Serial.println(millis());
      //      Serial.print("Soma: ");
      //      Serial.println(sum);
      //      Serial.print("Contador: ");
      //      Serial.println(contador);
    }



    //Intensidade baixa
    if (media > 340.0 && media <= 345.0)
    {
      digitalWrite(pinoled_ver, HIGH);
      digitalWrite(pinoled_ama, LOW);
      digitalWrite(pinoled_verm, LOW);
    }
    //Intensidade media
    if (media > 345 && media <= 348.5)
    {
      digitalWrite(pinoled_ver, HIGH);
      digitalWrite(pinoled_ama, HIGH);
      digitalWrite(pinoled_verm, LOW);
    }
    //Intensidade alta
    if (media > 348.5)
    {
      digitalWrite(pinoled_ver, HIGH);
      digitalWrite(pinoled_ama, HIGH);
      digitalWrite(pinoled_verm, HIGH);
      retorno = 1;
    }

     delay(50);
    //Apaga todos os leds
    digitalWrite(pinoled_ver, LOW);
    digitalWrite(pinoled_ama, LOW);
    digitalWrite(pinoled_verm, LOW);
  }
  return retorno;
}

void setGrupoDisplay(int num)
{
  if (num == 10) {
    Serial.print("Numero: 0 ");
    digitalWrite(PinoA0, LOW);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 1) {
    Serial.print("1 ");
    digitalWrite(PinoA0, HIGH);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 2) {
    Serial.print("2 ");
    digitalWrite(PinoA0, LOW);
    digitalWrite(PinoA1, HIGH);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 3) {
    Serial.print("3 ");
    digitalWrite(PinoA0, HIGH);
    digitalWrite(PinoA1, HIGH);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 4) {
    Serial.print("4 ");
    digitalWrite(PinoA0, LOW);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, HIGH);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 5) {
    Serial.print("5 ");
    digitalWrite(PinoA0, HIGH);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, HIGH);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 6) {
    Serial.print("6 ");
    digitalWrite(PinoA0, LOW);
    digitalWrite(PinoA1, HIGH);
    digitalWrite(PinoA2, HIGH);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 7) {
    Serial.print("7 ");
    digitalWrite(PinoA0, HIGH);
    digitalWrite(PinoA1, HIGH);
    digitalWrite(PinoA2, HIGH);
    digitalWrite(PinoA3, LOW);
  }
  if (num == 8) {
    Serial.print("8 ");
    digitalWrite(PinoA0, LOW);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, HIGH);
  } if (num == 9) {
    Serial.println("9 ");
    digitalWrite(PinoA0, HIGH);
    digitalWrite(PinoA1, LOW);
    digitalWrite(PinoA2, LOW);
    digitalWrite(PinoA3, HIGH);
  }
//  if (num == 10) {
//
//    Serial.println("Limpa o display...");
//    digitalWrite(PinoA0, HIGH);
//    digitalWrite(PinoA1, HIGH);
//    digitalWrite(PinoA2, HIGH);
//    digitalWrite(PinoA3, HIGH);
//  }
}
