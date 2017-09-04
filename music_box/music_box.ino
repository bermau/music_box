/*
B. MAUBERT

Revue 2016/10/18.
Une partie du code est inspirée de 
  Melody
 
 Plays a melody
 
 circuit:
 * 8-ohm speaker on digital pin 8
 
 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.
 http://arduino.cc/en/Tutorial/Tone
Improved by Bertrand Maubert, 2102 : another Music ("petit Papa").

Use a 2-dimensions array allowing to indicate simultaneously the note/tune and its duration.
Use function sizeof to determine the size of the song.

TODO : the array contains only floats although the lenght ot the notes could be 
integer.

A améliorer : mon tableau est composé entièrement de nombres flotants, alors que 
la hauteur des notes est contituées de nombres entiers. Cela est 
nécessaire pour un bon fonctionnement de la durée des notes, mais il en résulte 
un gaspillage de place.

air_petit_papa_2 : utilise correctement un array
projet : faire en sorte que la fonction playMusic attende une référecne à un tableau.
air_petit_papa_3 : la fonction playMusic attend un tableau, sa taille et la vitesse.
deux_musiques_2 : joue de façon aléatoire une des deux chansons. 

trois_musiques : j'ajoute à la claire fonctaine.  J'ajoute trois LED qui clignottent en fonction du rythme. OK le 18 nov 2012.
trois_musiques_nov_2012 : pendant l'exécution d'une musique, l'appuie sur le bouton ne fait plus rien. A la fin de la musique, extinction de la LED.
trois_musiques_quatre_boutons : j'ai ajouté 3 autres boutons.
sept 2017 : ajout d'un clignotement quand la boite ne joue pas de musique. 

Principes : 

quatre boutons :

Numérotation des boutons et led : 
Nom du bouton     | Couleur | pin arduino  | fonction 
1) BOUTON_1       | JAUNE   | 2            | chanson 1
2) BOUTON_2       | ROUGE   | 3            | chanson 2
3) BOUTON_3       | VERT    | 4            | chanson 3
4) BOUTON_4       | BLEU    | 5            | chanson aléatoire

Principe : si on appuie sur plusieurs boutons à la fois, le boutons retenu est 
le bouton de numéro le plus faible. En d'autres termes le bouton rouge a priorité 
sur le rouge, qui a lui meme priorité sur les vert.
Si au cours de l'exécution d'une musique on appuie sur un bouton d'une autre musique, 
l'autre musique est lancée. Si au cours de l'exécution d'une musique on appuie sur 
le bouton de la meme musique, celle-ci s'arrête. Pour ces 2 deux dernières lignes, 
l'appuie sur le bouton n'est pris en compte qu'après quelques notes jouées (ce qui résoud le 
problème de la répétition des touches).
- Après 20 secondes d'inactitivé, le Arduino passe en mode sleep. 
Il en sort quand on appuie sur le bouton JAUNE (pin 2). Le mode sleep de diminuer
la consommation du microprocesseur. Cette possibilité est intéressante pour un micro
processeur utilisé sans Arduino. Avec le dispositif Arduino, la consommation ne peut
pas tre réduite de façon significative parce que l'alimentation reste toujours 
active, meme si le CPU est en veille. 
- Pour que l'utilisateur pense à éteindre la boite à musique j'ai donc ajouté un 
clignotement de LED quand la boite est sous tension mais ne joue pas de musique. 


prévu  2017 : Faire en sorte qu'une des LED clignote de façon intermitente
 en absence de musique. Cela servira d'indicateur de mise en tension. 

OK pour Arduino 2:1.0.5
*/

#include "pitches.h"
// definition des
#define HP 8 // Le haut parleur est sur la sortie digitale 8
#define LED_ROUGE 11 // trois led en position 11, 12 et 13
#define LED_JAUNE 12
#define LED_VERTE 13
// definition du branchement des boutons
#define BOUTON_1 2 
#define BOUTON_2 3 
#define BOUTON_3 4
#define BOUTON_4 5 
// etat logique des boutons
int etatBouton1=HIGH; 
int etatBouton2=HIGH; 
int etatBouton3=HIGH; 
int etatBouton4=HIGH;
int presentState=0; // 0 : aucune lecture ; 1 lecture de la chanson 1 ; 2 lecture de la chanson 2, etc.
int prevState=presentState; // etat précédent.

int LED_ACTUELLE=LED_VERTE; // LED allumée
// Je voudrais bien mettre le code des notes dans un fichier à part. Mais je ne sais pas le faire.

// Air : Petit papa
// Vitesse de jeu : Durée de la mesure en millisecondes. 
int SPEED1=1200 ;  
float MySong1[][2]=
{
  {NOTE_B3,2},
  {NOTE_B3,4},
  {NOTE_B3,4},
  {NOTE_D4,2/1.5},
  {NOTE_B3,4},
  {NOTE_G3,4},
  {NOTE_G3,4},
  {NOTE_A3,4},  
  {NOTE_A3,4},
  {NOTE_B3,2},
  {NOTE_G3,2},
  // Maman m'a dit...
  {NOTE_B3,2},
  {NOTE_B3,4},
  {NOTE_B3,4},
  {NOTE_D4,2/1.5},
  {NOTE_B3,4},
  {NOTE_G3,4},
  {NOTE_G3,4},
  {NOTE_A3,4},  
  {NOTE_A3,4},
  {NOTE_G3,1},
  //J'avais des fleurs...
  {NOTE_A3,2},
  {NOTE_F3,4},
  {NOTE_A3,4},
  {NOTE_B3,2},
  {NOTE_G3,4},
  {NOTE_B3,4},
  {NOTE_C4,2},  
  {NOTE_A3,4},
  {NOTE_C4,4},
  {NOTE_B3,2},
  {NOTE_G3,2},
//  Et un bouquet...
  {NOTE_A3,2},
  {NOTE_F3,4},
  {NOTE_A3,4},
  {NOTE_B3,2},
  {NOTE_G3,4},
  {NOTE_B3,4},
  {NOTE_C4,2},  
  {NOTE_A3,4},
  {NOTE_C4,4},
  {NOTE_B3,1},
  {NOTE_B3,2},
  {NOTE_B3,4},
  {NOTE_B3,4},
  {NOTE_D4,1},
  {NOTE_A3,2},
  {NOTE_B3,4},
  {NOTE_A3,4},
  {NOTE_G3,1}, 
};

// air Ah les crocodiles

//Vitesse de jeu : Durée de la mesure en millisecondes. 
int SPEED2=900 ;  
float MySong2[][2]=
{
{NOTE_F3,2},
{NOTE_F3,4/1.5},
{NOTE_A3,8},
{NOTE_C4,2/1.5},
{NOTE_AS3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_AS3,4},
{NOTE_A3,2},
{NOTE_G3,2},
{NOTE_G3,2},
{NOTE_G3,4/1.5},
{NOTE_A3,8},
{NOTE_G3,(2/1.5)},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_B3,4},
{NOTE_C4,1},
{NOTE_F3,2},
{NOTE_F3,(4/1.5)},
{NOTE_A3,8},
{NOTE_C4,(2/1.5)},
{NOTE_AS3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_AS3,4},
{NOTE_A3,2},
{NOTE_G3,2},
{NOTE_G3,2},
{NOTE_G3,(4/1.5)},
{NOTE_A3,8},
{NOTE_G3,2/1.5},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_A3,4},
{NOTE_B3,4},
{NOTE_C4,4},
{NOTE_AS3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_AS3,2},
{NOTE_G3,2},
{NOTE_E3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_E3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_C4,4},
{NOTE_AS3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_A3,1},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_F3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_A3,4},
{NOTE_AS3,2},
{NOTE_G3,2},
{NOTE_E3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_E3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_G3,4},
{NOTE_C4,4},
{NOTE_AS3,4},
{NOTE_A3,4},
{NOTE_G3,4},
{NOTE_F3,4},
{NOTE_C4,4},
{NOTE_C4,4},
{NOTE_C4,4},
{NOTE_F3,1}
};

// à la claire fontaine
int SPEED3=2000 ;  
float MySong3[][2]=
{
{NOTE_C4,2},
{NOTE_C4,4},
{NOTE_E4,4},
{NOTE_E4,4},
{NOTE_D4,4},
{NOTE_E4,4},
{NOTE_D4,4},
{NOTE_C4,2},
{NOTE_C4,4},
{NOTE_E4,4},
{NOTE_E4,4},
{NOTE_D4,4},
{NOTE_E4,2},
//j'ai trouvé l'eau si belle
{NOTE_E4,2},
{NOTE_E4,4},
{NOTE_D4,4},
{NOTE_C4,4},
{NOTE_E4,4},
{NOTE_G4,4},
{NOTE_E4,4},
{NOTE_G4,2},
{NOTE_G4,4},
{NOTE_E4,4},
{NOTE_C4,4},
{NOTE_E4,4},
{NOTE_D4,2},

//Il y a longtemps
{NOTE_C4,2},
{NOTE_C4,4},
{NOTE_E4,4},
{NOTE_E4,4},
{NOTE_D4,8},
{NOTE_C4,8},
{NOTE_E4,4},
{NOTE_C4,4},
{NOTE_E4,2},
{NOTE_E4,4},
{NOTE_D4,8},
{NOTE_C4,8},
{NOTE_E4,4},
{NOTE_D4,4},
{NOTE_C4,2}
};

// choisit un air de façon a éléatoire et le joue.
void chooseAndPlay() {
      long randomNumber;
      randomNumber=random(0,150);
      Serial.print("Random : ");
      Serial.print(randomNumber + "\n");
          if (randomNumber < 50)
          {
    prevState=presentState;
    presentState=1;   
    playMusic(MySong1,sizeof(MySong1)/8, SPEED1);
          }
          else if (randomNumber < 100)
          {
    prevState=presentState;
    presentState=2; 
    playMusic(MySong2,sizeof(MySong2)/8, SPEED2);           
          }
          else
          {
    prevState=presentState;
    presentState=3; 
    playMusic(MySong3,sizeof(MySong3)/8, SPEED3);          
          }
}


void changerCouleurDesLED()
{
  long randomNumber;
  int COULEUR;
  int choix1 ;
  int choix2 ;
  digitalWrite(LED_ACTUELLE,LOW);
  
  switch(LED_ACTUELLE)
  {
    case LED_ROUGE :
     choix1=LED_VERTE; choix2=LED_JAUNE;
     break ;
    case LED_VERTE :
     choix1=LED_ROUGE; choix2=LED_JAUNE;
     break;
    default : 
      choix1=LED_VERTE; choix2=LED_ROUGE;  
  }
  randomNumber=random(0,100);
  if (randomNumber <50)
  {
    // allume la rouge
    COULEUR=choix1;
  }
  else
  {
    // allume la jaune
    COULEUR=choix2;
  }
  digitalWrite(COULEUR,HIGH);
  LED_ACTUELLE=COULEUR;
}
// jouer une musique codée dans un tableau à 2 dimensions
void playMusic(float theArray[][2], int sizeOfSong, int tempo)
{
  //sizeof renvoie la taille en octets ici, il faut diviser par 8 parce que le tableau contient 2 nombres flottants de 4 octets chacun.
  for (int thisNote = 0; thisNote < sizeOfSong; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the type of the note.
    // e.g. quarter note = 1000 / 4, eigth note = 1000/8, etc.
    int noteDuration = tempo/theArray[thisNote][1] *0.95;
    int delayDuration= tempo/theArray[thisNote][1] ;
    tone(HP, theArray[thisNote][0],noteDuration) ;
    // allume une LED 
    changerCouleurDesLED();
    // to distinguish the notes, set a minimum of time between them.
    // the note's duration + 30% seems to work well :
    int pauseBetweenNotes = delayDuration;
    //int pauseBetweenNotes = noteDuration * 1.10;
    delay(pauseBetweenNotes);
    // arreter si on appuie à nouveau sur le bouton
    if (readButtons()>0 and thisNote>4) break;
//     etat_bouton_un = digitalRead(bouton_un); //Rappel : bouton_un = 2
//    if (etat_bouton_un==LOW) break;
    // stop the tone playing:
    noTone(HP);
   }
   // Arret des lumières
     digitalWrite(LED_ACTUELLE,LOW);
}

// Un clignotelent dont on sort par pression d'une touche.
void blink_led(){
  // int nextState=0;
  do 
  {
  digitalWrite(LED_VERTE,HIGH);
  delay(10); 
  digitalWrite(LED_VERTE,LOW);
  delay(3000);  
  } while ( readButtons() == 0) ; 
}

// Lit l'état des boutons et renvoie l'ordre retenu ; 
// c'est ici que l'on décide de ce qu'il se passe en cas d'appui sur 
// plusieurs boutons à la fois.
// renvoie l'état désiré de l'automate.
int readButtons()
{
  etatBouton1=digitalRead(BOUTON_1);
  if (etatBouton1 == LOW) {
      return 1;
  }
  else{
  etatBouton2=digitalRead(BOUTON_2);    
   if (etatBouton2 == LOW) {
      return 2;
  }
  else{
   etatBouton3=digitalRead(BOUTON_3);    
   if (etatBouton3 == LOW) {
      return 3;
  }
  else{
   etatBouton4=digitalRead(BOUTON_4);   
   if (etatBouton4 == LOW) {
      return 4;
  }
  else
  {return 0;
  }
  }
  }
  }
}


void setup() {
  // broche circuit_test, en lecture oui/non.
  pinMode(BOUTON_1, INPUT);
  pinMode(BOUTON_2, INPUT);
  pinMode(BOUTON_3, INPUT);
  pinMode(BOUTON_4, INPUT);
  pinMode(HP,OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_JAUNE, OUTPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600); // Initialisation de la liaison série
  Serial.print("Hello\n");
  Serial.print("Taille du tableau : \n");
  Serial.print(sizeof(MySong1)+"\n");
  blink_led();
}


void loop()
{
   int nextState=0;
   nextState=readButtons();
   Serial.print("etat voulu : ");
   Serial.println(nextState);
   if (nextState != presentState)
   {
     presentState=nextState;
       prevState=presentState;
       Serial.print("playing");
       // jouer l'air
       switch(presentState){
         case 1 :
            playMusic(MySong1,sizeof(MySong1)/8, SPEED1);
            break;
         case 2 :
            playMusic(MySong2,sizeof(MySong2)/8, SPEED2);
            break;
         case 3 :
            playMusic(MySong3,sizeof(MySong3)/8, SPEED3);
            break;
         case 4 : 
            chooseAndPlay();
            break;
         case 0 : 
            blink_led();
            break;
       }
  } 
}
