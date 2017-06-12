#include <SPI.h>

//Les librairies du module nRF24
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//Les variables reliés au module radio
#define RF24_CEPIN 9      // pin ce du spi
#define RF24_CSPIN 10     // pin cs du spi

RF24 radio(RF24_CEPIN, RF24_CSPIN);     //Le module RF24 est initialisé. C'est la bibliothèque qui gère ça. Elle nous demande juste sur quel pin est branchée le RF24 

const uint64_t txPipe = 0xE8E8E8E8E8LL; //les "tuyaux" de communication (tx pour transmission et rx pour réception) 
const uint64_t rxPipe = 0xF6F6F6F6F1LL; //Pour l'autre arduino, il devront être inversés (L'autre recevra le flux du "tuyau" tx de lautre et transmettra dans le "tuyau" rx de cet arduino)


/***Réglage des logs */

#define TAILLELOG 60
String logs[TAILLELOG] = {""}; 
int compteurlogs = 0;

void initialiserModuleRadio()
{
  ///ANCIEN CODE DE LA HCAR
    radio.begin(); //Cela permet d'initialiser la radio
    radio.printDetails(); //Cela permet d'avoir les details de la radio, utile en mode debug

    //Paramètre de l'émetteur 
    radio.enableDynamicPayloads();    // payload de taille variable
    radio.setDataRate(RF24_1MBPS);    // 1 Mbits/s
    radio.setPALevel(RF24_PA_MAX);    // puissance maxi
    radio.setChannel(0x10);           // la fréquence 2,4G 
    
    // delay 4 ms, 15 retries
    // 0 - 250us, 15 - 4000us
    //On essaiera de se connecter 15 fois à la voiture et on attendra 15ms une réponse de sa part
    radio.setRetries(15,15);      // retrie & timeout

    radio.openWritingPipe(txPipe); //On défini le tunnel d'écriture
    radio.openReadingPipe(1,rxPipe); //On défini le tunnel de lecture
    radio.setAutoAck(true); //Aucune idée de l'utilité de ce truc...
}
/*****
 * Permet d'envoyer un message au PC qui écoute
 * 
 * @param typeLe type de donnée (Accelero, capteurs de distance, GPS,...)
 * @param data La donnée à envoyer
 */
void envoyerMessage(int type,byte data[])
{
  /****************************On concatène le message:                   **************************************/
    byte buffer[sizeof(data)+1] = {0}; //On crée un nouveau buffer qui contiendra le message
    buffer[0] = (byte)type; //On met le type comme premier octet
    for(int i=0;sizeof(data)<i;i++) //On rajoute les data ensuite
    {
      buffer[i+1]=data[i];
    }
    //On arrête d'écouter
    radio.stopListening(); 
    //On écrit
    radio.write((void *)buffer,sizeof(buffer));
    //On réécoute
    radio.startListening();
}
void registerToLog(int type, String message)
{
  String msg = itoa(type) + ":" + itoa(millis()) + "/" + message;
  if(compteur>=TAILLELOG)//Si on a dépassé la taille des logs
  {
    compteur = 1; //On remet le compteur de log à 1
    logs[0] = msg //On réécrit par dessus le 1 er log
  }else{
    logs[compteur]= msg;//On écrit le log
    compteur++;
  }
}

void setup() {
  

}

void loop() {


}
