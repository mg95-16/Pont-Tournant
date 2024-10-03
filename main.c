#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Définition des pins pour le contrôle du moteur
#define DIR_PIN 2    // Pin direction
#define STEP_PIN 4   // Pin step

// Nombre de pas pour un tour complet (360°)
#define STEPS_PER_REV 3200

// Table des positions en pas pour les angles : 0°, 130°, 180°, 210°, 240°
// Les angles sont ajustés légèrement pour correspondre à un nombre entier de pas.
int steps_for_angles[] = {
    0,     // 0° exact
    1156,  // ~130.5° (nombre entier de pas pour être proche de 130°)
    1600,  // 180° exact
    1866,  // ~210.1° (nombre entier de pas pour être proche de 210°)
    2133   // ~240.1° (nombre entier de pas pour être proche de 240°)
};

// Variable pour stocker la position actuelle (en pas)
int current_position = 0;

// Délai en millisecondes pour 20 pas par seconde (90° en 40 secondes)
const int step_delay_ms = 12;

// Fonction pour déplacer le moteur vers la nouvelle position avec la direction appropriée
void move_to_position(int target_position) {
    int steps_to_move = target_position - current_position;

    // Déterminer la direction (1 pour avant, 0 pour arrière)
    if (steps_to_move > 0) {
        gpio_set_level(DIR_PIN, 1);  // Mouvement dans une direction (avant)
    } else {
        gpio_set_level(DIR_PIN, 0);  // Mouvement dans l'autre direction (arrière)
        steps_to_move = -steps_to_move;  // Inverser le nombre de pas pour être positif
    }

    // Effectuer le mouvement avec un délai entre chaque pas
    for (int i = 0; i < steps_to_move; i++) {
        gpio_set_level(STEP_PIN, 1);  // Impulsion haute
        vTaskDelay(pdMS_TO_TICKS(1));  // Maintenir l'impulsion pendant 1 ms
        gpio_set_level(STEP_PIN, 0);  // Impulsion basse

        // Délai entre chaque pas (50 ms pour 20 pas par seconde)
        vTaskDelay(pdMS_TO_TICKS(step_delay_ms));
    }

    // Mettre à jour la position actuelle après le mouvement
    current_position = target_position;
}

void app_main() {
    // Initialiser les GPIO pour la direction et le pas
    gpio_reset_pin(DIR_PIN);
    gpio_reset_pin(STEP_PIN);
    gpio_set_direction(DIR_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(STEP_PIN, GPIO_MODE_OUTPUT);

    // Exemple de mouvement : déplacer vers la position correspondant à 240° (~2133 pas)
    move_to_position(steps_for_angles[0]);    
    move_to_position(steps_for_angles[2]);
    vTaskDelay(pdMS_TO_TICKS(3000)); 
    move_to_position(steps_for_angles[1]);
    vTaskDelay(pdMS_TO_TICKS(3000)); 
    move_to_position(steps_for_angles[3]);
      vTaskDelay(pdMS_TO_TICKS(3000)); 
  move_to_position(steps_for_angles[0]);
    // Autres déplacements possibles vers les autres angles (par exemple)
    // move_to_position(steps_for_angles[1]);  // ~130.5° (1156 pas)
    // move_to_position(steps_for_angles[2]);  // 180° (1600 pas)
    // move_to_position(steps_for_angles[3]);  // ~210.1° (1866 pas)
}
