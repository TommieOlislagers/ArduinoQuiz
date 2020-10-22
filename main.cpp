const int redLedPin = 7;
const int greenLedPin = 6;
const int knopPin1 = 2;
const int knopPin2 = 3;
const int knopPin3 = 4;

const long eventInterval = 1000;
long voorgaandeTijd = 0;

int score = 0;
int gekozenAntwoord = 0;
bool vraagIsNogNietBeantwoord = true;
bool beginQuiz = false;

const String Vragen[] = {
                    "Welke programmeertaal is gemaakt door Microsoft?", 
                    "In welk jaar is PHP ontwikkeld?", 
                    "Wie is de ontwikelaar van Python?",
                    "Wat is de meest recent ontwikkelde programmeertaal?",
                    "Door wie is Github een paar jaar geleden overgenomen?"
                  };
int vragenArrayLengte = sizeof(Vragen)/sizeof(Vragen[0]);

const String Antwoorden[][3][2]  =  {
                              { 
                                {"C#", "true" }, {"PHP", "false" } , {"C++", "false" } 
                              }, 
                              { 
                                {"1992", "false" } , { "1994", "true" }, { "1995", "false" }
                              },
                              { 
                                {"Bjarne Stroustrup", "false" } , { "Guido van Rossum", "true" }, { "Linus Torvalds", "false" }
                              },
                              { 
                                {"Ruby", "false" } , { "Google's GO", "false" }, { "Rust", "true" }
                              },
                              { 
                                {"Google", "false" } , { "Apple", "false" }, { "Microsoft", "true" }
                              }
                            };

void setup() {
  pinMode(redLedPin, OUTPUT); 
  pinMode(greenLedPin, OUTPUT); 
  pinMode(knopPin1, INPUT); 
  pinMode(knopPin2, INPUT); 
  pinMode(knopPin3, INPUT); 
  
  Serial.begin(9600);
  Serial.println(F("Welkom bij de quiz! De quiz bestaat uit 5 vragen. Je hebt 10 seconden om een vraag te beantwoorden."));
  Serial.println(F("Je hebt 3 knoppen om de vragen mee te beantwoorden, A(LINKS), B(MIDDEN), C(RECHTS)"));
  Serial.println(F("Wilt u de quiz starten? j/n"));
  
} 

void loop() { 
     quizBeginnenOfEindigen();
     StartQuiz();
}

// De startquiz functie is bedoeld voor het printen van de antwoorden en vragen en resterende variabelen initalisaties en veranderingen.
void StartQuiz()
{
     while(beginQuiz)
     {
       
       for (int i = 0; i < vragenArrayLengte; i++) {
        
         // Dit zorgt er voor dat bij elke nieuwe vraag, er niet meer wordt gekeken
         // naar of de oudere vraag wel of niet beantwoord is.
         vraagIsNogNietBeantwoord = true;
         
         // Print de vraag en het aantal keuzes qua antwoorden.
         Serial.println(Vragen[i]);
         Serial.println("A(" + Antwoorden[i][0][0] + "), B(" + Antwoorden[i][1][0] + "), C(" + Antwoorden[i][2][0] + ")");
        
         while(vraagIsNogNietBeantwoord)
         {
           welkeKnopIsErIngedrukt();
         }
       
       isHetAntwoordCorrect(i, gekozenAntwoord);
       beeindigDeQuiz(i);
     }

       Serial.println(F("Wilt u de quiz herstarten? j/n"));
       quizBeginnenOfEindigen();
     }
}



// Deze functie leest de waardes af van de knoppen en baseert daarop welke knop binnen het tijdslimiet is ingedrukt.
void welkeKnopIsErIngedrukt()
{
  // Geef hier aan hoe lang je wilt dat je per vraag de tijd hebt in seconden.
  int tijdInSeconden = 10;
  
  // Initialiseren van beginwaardes knoppen
  int knopStatus1 = 0; 
  int knopStatus2 = 0; 
  int knopStatus3 = 0;
  
  tijdInSeconden = telOmlaag(tijdInSeconden);
            
  if(tijdInSeconden != 0)
  {
    knopStatus1 = digitalRead(knopPin1);
    knopStatus2 = digitalRead(knopPin2);
    knopStatus3 = digitalRead(knopPin3);

    if(knopStatus1 == HIGH)
    {
      gekozenAntwoord = 0;
      vraagIsNogNietBeantwoord = false;
    }
    else if(knopStatus2 == HIGH)
    {
      gekozenAntwoord = 1;
      vraagIsNogNietBeantwoord = false;
    }
    else if(knopStatus3 == HIGH)
    {
      gekozenAntwoord = 2;
      vraagIsNogNietBeantwoord = false;
    }

    delay(200);
  }
  else
  {
    vraagIsNogNietBeantwoord = false;
    Serial.println(F("Je hebt de vraag niet optijd beantwoord. Er is geen score toegevoegd."));
    gekozenAntwoord = -1;
  }
  
}

// Laat per vraag zien hoeveel seconden je nog over hebt om de vraag te beantwoorden.
// Hij verwijdert hier 1 seconden per seconden, dat houdt dus in dat hij pas na 1 seconden, -1 doet van het totaal aantal seconden dat je hebt
// om de vraag te beantwoorden.
int telOmlaag(int seconden)
{
  long actueleTijdInMillis = millis();
            
  if(actueleTijdInMillis - voorgaandeTijd >= eventInterval)
  {
    int actueleTijdInSeconden = seconden;
    String timeInStringFirst = "Nog ";
    String timeInStringLast = " seconden te gaan.";
    String timeInString = timeInStringFirst + actueleTijdInSeconden + timeInStringLast;
    Serial.println(timeInString);
    voorgaandeTijd = actueleTijdInMillis;
    seconden--;
  }
  return seconden;
}

// Deze functie kijkt naar de waardes in de Antwoorden array en bepaald of het gekozen antwoord correct of incorrect is.
void isHetAntwoordCorrect(int vraagIndex, int Antwoord)
{
  if(Antwoord != -1)
  {
    if(Antwoorden[vraagIndex][Antwoord][1] == "true")
    {
      Serial.println(F("Dat is correct!"));
      score++;
      for(int i = 0; i < 3; i++)
      {
        digitalWrite(greenLedPin, HIGH);
        delay(500);
        digitalWrite(greenLedPin, LOW);
        delay(500);
      }
    }
    else if(Antwoorden[vraagIndex][Antwoord][1] == "false")
    {
      Serial.println(F("Dat is incorrect."));
      digitalWrite(redLedPin, HIGH);
      delay(3000);
      digitalWrite(redLedPin, LOW);
    }
  }
}

// Deze functie kijkt naar de vraagIndex om te bepalen of de quiz wel of niet beeindigd moet worden.
void beeindigDeQuiz(int vraagIndex)
{
   if(vraagIndex == vragenArrayLengte - 1)
   {
     beginQuiz = false;
     String msg = "Je bent geeindigd met een eindscore van: ";
     String msgconc = msg + score;
     Serial.println(msgconc);
   }
}

// Deze functie kijkt naar de input van de gebruiker om te bepalen of de quiz moet beginnen of eindigen.
void quizBeginnenOfEindigen()
{
  while(!beginQuiz)
  {
     char antwoord = Serial.read();
   
     if(antwoord == 'j')
     {
       Serial.println(F("Het spel start nu!"));
       beginQuiz = true;
     }
     else if(antwoord == 'n')
     {
       Serial.println(F("De quiz is voorbij."));
       break;
     }
  }
}
