const uint8_t PIN_LED = 19; // LED de la práctica
// ticks hasta la alarma (1 tick = 1 us). Probar 1000000, 250000 y 50000
const uint64_t ALARMA_US = 1000000;
hw_timer_t *timer = NULL; // "manija" del temporizador reservado
volatile bool hayAlarma = false; // bandera: la levanta la ISR, la baja loop()
// ISR (callback): la llama el hardware cada vez que el contador llega a la alarma
void IRAM_ATTR alSonarAlarma() {
hayAlarma = true; // solo avisar
}
void setup() {
Serial.begin(115200);
pinMode(PIN_LED, OUTPUT);
// 1 000 000 ticks por segundo: 1 tick = 1 us
timer = timerBegin(1000000);
// no se pudo reservar un temporizador
if (timer == NULL) {
Serial.println("Error: no hay temporizador libre");
return;
}
timerAttachInterrupt(timer, alSonarAlarma); // entregar el callback
// alarma, autorrecarga, repetir sin límite
timerAlarm(timer, ALARMA_US, true, 0);
}
void loop() {
// recuerda el estado del LED entre vueltas
static bool estadoLed = false;
if (hayAlarma) { // ¿está levantada la bandera?
hayAlarma = false; // bajarla
estadoLed = !estadoLed; // invertir el estado
digitalWrite(PIN_LED, estadoLed); // cambiar el LED
Serial.printf("Cambio del LED en t = %lu ms\n", millis());
}
}