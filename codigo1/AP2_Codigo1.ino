constexpr uint8_t LEDS[] = {16, 17, 18, 19};
constexpr uint8_t N_LEDS = sizeof(LEDS) / sizeof(LEDS[0]);
constexpr uint32_t PERIODO = 300; // ms entre un paso y el siguiente
uint8_t posicion = 0; // LED encendido (0 a 3)
uint32_t tPaso = 0; // instante del ultimo paso
void setup() {
for (uint8_t i = 0; i < N_LEDS; i++) {
pinMode(LEDS[i], OUTPUT);
}
}
void loop() {
uint32_t ahora = millis();
if (ahora - tPaso >= PERIODO) { 
tPaso = ahora; // anotar la hora
posicion = (posicion + 1) % N_LEDS; 
}
for (uint8_t i = 0; i < N_LEDS; i++) {
digitalWrite(LEDS[i], i == posicion); 
  }
}  