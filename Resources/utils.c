
void setupTimer2(){
    // Fonction pour configurer Timer2
    T2CONbits.T2CKPS = 0b0;  // Prescaler = 1:1
    T2CONbits.T2OUTPS = 0b1000; // Postscaler = 1:8
    PR2 = 249;                // Période de Timer2 pour 1ms
    T2CONbits.TMR2ON = 1;     // Activer Timer2
}
    
    
    /* Note : PIR1 = contient les flags d'interruption ; indique si une interruption a eu lieu et sur quel périphérique
     PIE1bits active ou désactive les interruptions des périphériques (= est-ce que en cas d'interruption on écrit cette interruption dans PIR1)
     */
    // SETUP interruptions
    PIR1bits.TMR2IF = 0;       // Effacer le flag d'interruption Timer2
    PIE1bits.TMR2IE = 1;       // Activer l'interruption Timer2
    INTCONbits.PEIE = 1;       // Activer les interruptions périphériques
    INTCONbits.GIE = 1;        // Activer les interruptions globales
}


void alternance_led_sans_timer() {

  // Configurer les broches des registres B et D (contrôlant les LEDs) comme
  // sorties
  TRISD = 0x00;
  TRISB = 0x00;

  while (1) { // Toujours
    /* Code à exécuter dans une boucle infinie */
    // Allumer les LEDs  5 à 8 (registre B), éteindre les leds 1 à 4 (registre
    // D)
    LATD = 0x00;
    LATB = 0x0F;

    // Attendre 500ms
    delai_approx();

    // Éteindre les LEDs  5 à 8 (registre B), allumer les leds 1 à 4 (registre
    // D)
    LATB = 0x00;
    LATD = 0x0F;
    // Attendre 500 ms. Le temps total approximatif de la boucle est 1s.
    delai_approx();
  }
}

void chenillard_avec_timer() {
  // Configurer les broches des registres B et D (contrôlant les LEDs) comme
  // sorties
  uint8_t ledState = 0b00000001;
  TRISD = 0x00;
  TRISB = 0x00;
  // Configuration du Timer2
  setupTimer2();

  while (1) {
    if (PIR1bits.TMR2IF) { // Vérifier si Timer2 a atteint la période
      PIR1bits.TMR2IF = 0; // Réinitialiser le flag TMR2IF

      // Mettre à jour les LEDs
      LATD = ledState & 0x0F; // État des 4 bits bas (D1 à D4) et croiser avec
                              // 0x0F pour n'allumer que la bonne led
      LATB = (ledState >> 4) &
             0x0F; // État des 4 bits hauts (D5 à D8) : décalage des 4 bits
                   // hauts de ledstate pour correspondre aux 4 bits bas de LATB

      // Décaler l'état des LEDs
      ledState <<= 1;          // Décaler à gauche
      if (ledState == 0) {     // Si aucune led n'est activée
        ledState = 0b00000001; // Recommencer à D1 après D8
      }
    }
  }
}

void demi_chenillard_LED85_sans_timer() {
  // Configurer les broches des registres B et D (contrôlant les LEDs) comme
  // sorties
  TRISD = 0x00;
  TRISB = 0x00;
  // Configuration du Timer2
  setupTimer2();

  while (1) {
    /* Code à exécuter dans une boucle infinie */
    // Allumer LED D8
    LATB = 0b00001000;

    // Attendre 125 ms
    delai_approx();
    //  Allumer LED D7
    LATB = 0b00000100;

    // Attendre 125 ms
    delai_approx();
    //  Allumer LED D6
    LATB = 0b00000010;
    // Attendre 125 ms
    delai_approx();
    // Allumer led D5
    LATB = 0b00000001;

    // Attendre 125 ms
    delai_approx();
  }
}

void interrupt_timer_with_S2() {
  TRISAbits.TRISA5 = 1; // Le bit 5 du registre A est en entrée
  ANSELAbits.ANSA5 = 0; // Désactive le mode analogique

  while (1) {

    if (PORTAbits.RA5 ==
        0) { // Lecture de l'entrée numérique (lorsque le bouton est pressé)
      if (T2CONbits.TMR2ON == 1) { // Si le timer est lancé
        T2CONbits.TMR2ON = 0; // Arrête Timer2, le défilement des leds s'arrête.

      } else {                // Sinon
        T2CONbits.TMR2ON = 1; // Relance Timer2, le défilement des leds reprend.
      }
    }
  }
}
