// AP2 - Codigo 2: juego de luces interactivo
// El pulsador cambia la velocidad: lento -> medio -> rapido -> lento...
// Placa: ESP32 Dev Module | core Espressif 3.x | sin librerias
// Pines: LEDs en GPIO16, 17, 18 y 19 (330 ohm); pulsador entre GPIO21 y GND
constexpr uint8_t LEDS[] = {16, 17, 18, 19};
constexpr uint8_t N_LEDS = sizeof(LEDS) / sizeof(LEDS[0]);
constexpr uint8_t PIN_BTN = 21;
constexpr uint32_t T_REBOTE = 30; // ms de antirrebote
constexpr uint32_t PERIODOS[] = {400, 150, 60}; // ms: lento, medio, rapido
constexpr uint8_t N_VEL = sizeof(PERIODOS) / sizeof(PERIODOS[0]);
uint8_t posicion = 0; // LED encendido (0 a 3)
uint8_t velocidad = 0; // indice dentro de PERIODOS
uint32_t tPaso = 0; // instante del ultimo paso
bool lecturaAnterior = HIGH; // ultima lectura cruda del pulsador
bool estadoEstable = HIGH; // estado filtrado (HIGH = suelto)
uint32_t tUltimoCambio = 0;
// Devuelve true solo en el instante en que se presiona el pulsador
bool botonPresionado() {
 bool lectura = digitalRead(PIN_BTN);
 if (lectura != lecturaAnterior) { // la senal se movio
 tUltimoCambio = millis();
 lecturaAnterior = lectura;
 }
 if ((millis() - tUltimoCambio) >= T_REBOTE && lectura != estadoEstable) {
 estadoEstable = lectura; // lleva 30 ms quieta: cambio real
 return estadoEstable == LOW; // flanco de bajada = se presiono
 }
 return false;
}
void setup() {
 Serial.begin(115200);
 for (uint8_t i = 0; i < N_LEDS; i++) {
 pinMode(LEDS[i], OUTPUT);
 }
 pinMode(PIN_BTN, INPUT_PULLUP); // suelto = HIGH, presionado = LOW
 Serial.println("AP2 lista. Presiona el pulsador.");
}
void loop() {
 // 1) Entradas
 if (botonPresionado()) {
 velocidad = (velocidad + 1) % N_VEL;
 Serial.print("Periodo: ");
 Serial.print(PERIODOS[velocidad]);
 Serial.println(" ms");
 }
 // 2) Logica temporizada
 uint32_t ahora = millis();
 if (ahora - tPaso >= PERIODOS[velocidad]) {
 tPaso = ahora;
 posicion = (posicion + 1) % N_LEDS;
 }
 // 3) Salidas
 for (uint8_t i = 0; i < N_LEDS; i++) {
 digitalWrite(LEDS[i], i == posicion);
 }
}
