/**
 * Régulateur de vitesse pour ventilateur
 * © Vincent Moucadeau 2018
 */

/* Configuration */
const byte info_vitesse_ventilateur = 2;
int maxi = 5700; // RPM max du ventilateur (se met à jour automatiquement)
int ventilateur = 8; // Broche du ventilateur
int debug = 0; // Active ou non les infos débug

void setup() {
  
  /* Initialise le port série  */
  Serial.begin(9600);
   
  pinMode(info_vitesse_ventilateur, INPUT_PULLUP);
  pinMode(ventilateur, OUTPUT);
  
  /* Message de bienvenue */
  Serial.println("Le régulateur a bien démarré !");
  Serial.println("Info : le mode débug vous permet d'afficher les valeurs de tours par minutes du ventilateur");
  Serial.println("       ainsi que la valeur souhaitée via le potentiomettre");
  Serial.println("Utilisation :");
  Serial.println(" - '1' Active le mode débug ");
  Serial.println(" - '2' Désactive le mode débug ");
  Serial.println(" - (N'importe quelle touche) Si actif, mets en pause le mode débug et affiche ce message (sauf commandes)");
  if(debug == 0) {
        Serial.println("Etat du mode débug : INACTIF");
      }
  else {
        Serial.println("Etat du mode débug : ACTIF");
      }

}

/** Fonction loop() */
void loop() {
  if(Serial.available() > 0) {
    digitalWrite(ventilateur, LOW);
    String commande = Serial.readString();
    if(commande == "1") {
      Serial.println("_______________________________");
      Serial.println("Le mode débug a été activé");
      delay(1000);
      debug = 1;
    }
    else if(commande == "2") {
      Serial.println("_______________________________");
      Serial.println("Le mode débug a été désactivé");
      delay(1000);
      debug = 0;
    }
    else {
      Serial.println("_________________________________________________________________________________________________________________________");
      Serial.println("Ce message s'affiche pendant 10 secondes");
      Serial.println("Info : le mode débug vous permet d'afficher les valeurs de tours par minutes du ventilateur");
      Serial.println("       ainsi que la valeur souhaitée via le potentiomettre");
      Serial.println("Utilisation :");
      Serial.println(" - '1' Active le mode débug ");
      Serial.println(" - '2' Désactive le mode débug ");
      Serial.println(" - (N'importe quelle touche) Si actif, mets en pause le mode débug et affiche ce message (sauf commandes)");
      if(debug == 0) {
        Serial.println("Etat du mode débug : INACTIF");
      }
      else {
        Serial.println("Etat du mode débug : ACTIF");
      }
      delay(10000);
    }
    
  }
  int pot = analogRead(5);
  pot = map(pot, 0, 1023, 0, maxi);
  
  /* Mesure la durée de la (demi) période */
  unsigned long periode = pulseIn(info_vitesse_ventilateur, LOW, 1000000) * 2;
  
  long result = 1000000 / periode * 60;
  if(pot > 0){
    digitalWrite(ventilateur, HIGH);
    if(result > pot) {
      if(result == 4294967236) {
        digitalWrite(ventilateur, HIGH);
        result = 0;
      }
      else {
      digitalWrite(ventilateur, LOW);
      }
    }
    else {
      digitalWrite(ventilateur, HIGH);
    }
  }
  else {
    digitalWrite(ventilateur, LOW);
  }
  if(result > maxi) {
    maxi = result;
  }
  /* Serial Debug */
  if(debug == 1) {
  Serial.println("__________________________");
  Serial.print("RPM : ");
  Serial.println(result);
  Serial.print("Potentiomettre : ");
  Serial.println(pot);
  }
}
