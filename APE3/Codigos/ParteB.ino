const uint8_t PIN_SENAL = 18; // salida de la onda cuadrada
const uint8_t PIN_LED = 19; // LED de latido
// Hz: frecuencia deseada. Probar 100, 1000 y 5000
const uint32_t FREC_SENAL = 100;
// Factor 2: cada alarma es medio periodo de la señal
const uint64_t ALARMA_US = 1000000 / (2 * FREC_SENAL);
hw_timer_t *timer = NULL; // "manija" del temporizador reservado
volatile bool nivel = false; // nivel actual del pin de señal
volatile uint32_t cambios = 0; // cuántas veces cambió el pin
// ISR: invierte el pin en el instante exacto de la alarma y cuenta el cambio
void IRAM_ATTR alSonarAlarma() {
 nivel = !nivel;
 digitalWrite(PIN_SENAL, nivel);
 cambios++;
}
void setup() {
 Serial.begin(115200);
 pinMode(PIN_SENAL, OUTPUT);
 pinMode(PIN_LED, OUTPUT);
 Serial.printf("Frecuencia pedida: %lu Hz\n", (unsigned long)FREC_SENAL);
 Serial.printf("Alarma: %lu ticks (us)\n", (unsigned long)ALARMA_US);
 timer = timerBegin(1000000); // 1 tick = 1 us
 if (timer == NULL) {
 Serial.println("Error: no hay temporizador libre");
 return;
 }
 timerAttachInterrupt(timer, alSonarAlarma); // entregar el callback
 timerAlarm(timer, ALARMA_US, true, 0); // alarma periódica, sin límite
}
void loop() {
 // cambios contados en el último segundo
 static uint32_t ultimo = 0;
 static bool estadoLed = false;
 // En 1 segundo el pin cambia 2 x FREC_SENAL veces
 if (cambios - ultimo >= 2 * FREC_SENAL) {
 ultimo += 2 * FREC_SENAL;
 estadoLed = !estadoLed;
 digitalWrite(PIN_LED, estadoLed);
 Serial.printf("Cambios del pin: %lu\n", (unsigned long)cambios);
 }
}