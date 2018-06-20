#include <Ultrasonic.h>
#include <avr/sleep.h>

int horario = 22;
int anti_horario = 23;
int horario2 = 24;
int anti_horario2 = 25;

int pwm = 7;
int pwm2 = 8;
int Velocidade = 0;
int Velocidade2 = 0;

int echo = 26;
int trigger = 27;

int tempo_virar = 500;

int farol = 28;
int farol2 = 29;
int LDR = 8;

int sensor_otico = 7;

int tempo_max = 30;
int tempo_execucao = 0;

int luminosidade;
float dist;

#define VELO_MAX 255

int luz(int ldr)
{
  return analogRead(ldr);
}

void acender_farois()
{
  digitalWrite(farol, HIGH);
  digitalWrite(farol2, HIGH);
}

void apagar_farois()
{
  digitalWrite(farol, LOW);
  digitalWrite(farol2, LOW);
}

void desligar_motores()
{
  digitalWrite(horario, LOW);
  digitalWrite(horario2, LOW);
  digitalWrite(anti_horario, LOW);
  digitalWrite(anti_horario2, LOW);
}
void frente()
{
  digitalWrite(horario, HIGH);
  digitalWrite(horario2, HIGH);
  digitalWrite(anti_horario, LOW);
  digitalWrite(anti_horario2, LOW);
}

void re()
{
  digitalWrite(horario, LOW);
  digitalWrite(horario2, LOW);
  digitalWrite(anti_horario, HIGH);
  digitalWrite(anti_horario2, HIGH);
}
void mudar_velocidade(int velo)
{
  if(velo > VELO_MAX || velo < 0)
  {
    return;
  }
  mudar_velocidade_mot1(velo);
  mudar_velocidade_mot2(velo-90);
}

void mudar_velocidade_mot1(int velo)
{
  
  if(velo > VELO_MAX || velo < 0)
  {
    return;
  }
  Velocidade = velo;
  analogWrite(pwm, Velocidade);
}

void mudar_velocidade_mot2(int velo)
{
  if(velo > VELO_MAX || velo < 0)
  {
    return;
  }
  Velocidade2 = velo;
  analogWrite(pwm2, Velocidade2);
}

void virar()
{
  digitalWrite(horario, HIGH);
  delay(tempo_virar);
}

void virar_mov()
{
  mudar_velocidade_mot1(Velocidade-180);
  delay(1000);
  mudar_velocidade_mot1(Velocidade+180);
}
float distancia(Ultrasonic sensor)
{
  float cmMsec; // Cria variável para armazenar medição
  long microsec = sensor.timing(); // Inicia medição
  cmMsec = sensor.convert(microsec, Ultrasonic::CM);
  return cmMsec;
}

Ultrasonic sensor(trigger, echo);


void setup()
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 0;              // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts


  
  pinMode(horario, OUTPUT);
  pinMode(anti_horario, OUTPUT);
  pinMode(horario2, OUTPUT);
  pinMode(anti_horario2, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(farol, OUTPUT);
  pinMode(farol2, OUTPUT);
  mudar_velocidade(VELO_MAX);
  Serial.begin(9600);
}
void loop()
{
  dist = distancia(sensor);
  luminosidade = luz(LDR);
  if (dist > 20)
  {
    frente();
  }
  else if (dist > 10)
  {
    desligar_motores();
    delay(2000); //esperar dois segundos
    virar(); 
  }
  else
  {
    desligar_motores();
  }
  if (luminosidade < 500)
  {
    acender_farois();
  }
  else if (luminosidade > 700)
  {
    apagar_farois();
  }
}

ISR(TIMER1_OVF_vect)
{
  if(tempo_execucao == tempo_max)
  {
    desligar_motores();
    apagar_farois();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
  }
  Serial.print("Tempo de execução:");
  Serial.print(tempo_execucao);
  Serial.print("\n");
  Serial.print("Objeto mais proximo:");
  Serial.print(dist);
  Serial.print("\n");
  Serial.print("Luminosidade:");
  Serial.print(luminosidade);
  Serial.print("\n");
  Serial.print("\n");
  tempo_execucao++;
  
}


