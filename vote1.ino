#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define VOTE_BUTTON_1 9  
#define VOTE_BUTTON_2 10  
#define VOTE_BUTTON_3 11  

#define RX_PIN 0        
#define TX_PIN 1         

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int votes[3] = {0, 0, 0};        
int registeredUsers = 0;          
int fingerprintsPerUser[3] = {0, 0, 0}; 
int totalFingerprints = 0;        
bool registrationPhase = true;
int userVoted[3] = {0, 0, 0};    
votedParty ='D';
void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  lcd.begin(16, 2);

  pinMode(VOTE_BUTTON_1, INPUT_PULLUP);
  pinMode(VOTE_BUTTON_2, INPUT_PULLUP);
  pinMode(VOTE_BUTTON_3, INPUT_PULLUP);


  if (finger.verifyPassword()) {
    lcd.print("Fingerprint enrollment");
  } else {
    lcd.print("Sensor not found");
    while (1);
  }
}

void loop() {
  if (registrationPhase) {
    registerUser();
  } else if (userVoted[0] && userVoted[1] && userVoted[2]) { 
    displayResults();
  } else {
    loginAndVote();
  }
}

void registerUser() {
  int userID = registeredUsers;  
  if (fingerprintsPerUser[userID] < 3) {
    lcd.clear();
    lcd.print("Place finger");
    lcd.setCursor(0, 1);
    lcd.print("to register");

    int newID = totalFingerprints + 1;
    if (getFingerprintEnroll(newID)) {  
      if (checkFingerprintExists(newID)) {  
        lcd.clear();
        lcd.print("finger register");
    	lcd.setCursor(0, 1);
    	lcd.print("NOT OK");
        delay(2000);
        return;
      }

      fingerprintsPerUser[userID]++;
      totalFingerprints++;
      lcd.clear();
      lcd.print("finger register");
      lcd.setCursor(0, 1);
      lcd.print("OK");
      delay(2000);
    }

    if (fingerprintsPerUser[userID] == 3) { 
      registeredUsers++;
      lcd.print("user:");
      lcd.print(registeredUsers);
      delay(2000);
    }
  }
}

void loginAndVote() {
  lcd.clear();
  lcd.print("Place finger");
  lcd.setCursor(0, 1);
  lcd.print("to login");

  int userID = getFingerprintID();
  if (userID >= 0 && userID < 3 && !userVoted[userID]) {
    userVoted[userID] = 1;   // Mark this user as voted
    lcd.clear();
    lcd.print("you may ");
    lcd.setCursor(0, 1);
    lcd.print("VOTE");
    delay(2000);

    lcd.clear();
    lcd.print("Party A, Party B);
    lcd.setCursor(0, 1);
    lcd.print("Party C");

    bool voted = false;
    while (!voted) {
      if (digitalRead(VOTE_BUTTON_1) == LOW) {
        votes[0]++;
        votedParty='A';
        voted = true;
      } else if (digitalRead(VOTE_BUTTON_2) == LOW) {
        votes[1]++;
        votedParty='B';
        voted = true;
      } else if (digitalRead(VOTE_BUTTON_3) == LOW) {
        votes[2]++;
        votedParty='C';
        voted = true;
      }
    }

    lcd.clear();
    lcd.print("Voted for party");
    lcd.setCursor(0, 1);
    lcd.print("Party ")
    lcd.print(votedParty);
    votedParty ='D';
    delay(2000);
  } else if (userID >= 0 && userVoted[userID]) {
    lcd.clear();
    lcd.print("you may ");
    lcd.setCursor(0, 1);
    lcd.print("NOT VOTE");
    delay(2000);
  }
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return (finger.fingerID - 1) / 3; 
}

bool checkFingerprintExists(int id) {
  for (int i = 1; i <= totalFingerprints; i++) {
    if (finger.loadModel(i) == FINGERPRINT_OK && finger.fingerID == id) {
      return true;
    }
  }
  return false;
}

bool getFingerprintEnroll(int id) {
  int p = -1;
  lcd.clear();
  lcd.print("Enrolling ID ");
  lcd.print(id);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) {
      lcd.clear();
      lcd.print("finger register");
    	lcd.setCursor(0, 1);
    	lcd.print("NOT OK");
        delay(2000);
    } else if (p == FINGERPRINT_NOFINGER) {
      lcd.clear();
      lcd.print("finger register");
    	lcd.setCursor(0, 1);
    	lcd.print("NOT OK");
        delay(2000);
    } else {
      lcd.print("finger register");
      lcd.setCursor(0, 1);
      lcd.print("NOT OK");
      delay(2000);
      return false;
    }
  }

  }



void displayResults() {
  lcd.clear();
  lcd.print("Voting Completed");
  delay(2000);

  int maxVotes = votes[0];
  int secondMaxVotes = 0;
  char winningParty = 'A';  

  if (votes[1] > maxVotes) {
    secondMaxVotes = maxVotes;
    maxVotes = votes[1];
    winningParty = 'B';
  } else {
    secondMaxVotes = votes[1];
  }

  if (votes[2] > maxVotes) {
    secondMaxVotes = maxVotes;
    maxVotes = votes[2];
    winningParty = 'C';
  } else if (votes[2] > secondMaxVotes) {
    secondMaxVotes = votes[2];
  }

  int voteDifference = maxVotes - secondMaxVotes;

  // Display the results
  lcd.clear();
  lcd.print("Party ");
  lcd.print(winningParty);
  lcd.setCursor(0, 1);
  lcd.print("won by ");
  lcd.print(voteDifference);
  lcd.print(" vote");
  delay(5000);
}
