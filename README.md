# fingerVote
This fingerprint-based voting system is built using an Arduino Uno, Adafruit Fingerprint Sensor, and an LCD screen, along with push buttons for voting. Here’s a detailed breakdown of the hardware used and how each component functions within the system:
# 1.Arduino Uno
The Arduino Uno microcontroller is the main processing unit of the system. It manages the registration and voting processes, interfaces with the fingerprint sensor and LCD screen, and monitors the buttons for voting input.
It controls the flow of the voting program and ensures only authorized users can vote by validating fingerprints.
# 2.AS608 Fingerprint Sensor (connected to RX and TX pins of Arduino)
The fingerprint sensor is used to register and authenticate each user. During the registration phase, it stores each fingerprint with a unique ID associated with a user.
In the voting phase, users place their finger on the sensor to log in. The sensor reads the fingerprint, matches it against stored records, and identifies the user’s ID.
RX (Receive) and TX (Transmit) pins on the sensor are connected to the Arduino’s RX and TX pins (configured for Serial1), allowing serial communication to transmit fingerprint data.
# 3.LCD Screen (16x2)
The LCD screen displays messages to guide users through registration and voting, as well as the results after voting is complete.
It provides visual feedback, such as instructions to place a finger on the sensor, the party voted for, and the winning party along with the vote difference.
# 4. Push Buttons
Three push buttons are connected to the Arduino to allow users to vote for their preferred party. Each button corresponds to a specific party: Party A, Party B, or Party C.
When a registered user logs in successfully, they are prompted to press a button to cast their vote.
These buttons are set up with pull-up resistors to ensure stable input readings. When a button is pressed, it sends a LOW signal to the Arduino, registering the vote for the associated party.
# 5. Voting Process and Logic
## Registration Phase: Up to three users can register using their fingerprints, with each user allowed to register three unique fingerprints. Duplicate fingerprint registrations are checked to ensure each entry is unique.
## Voting Phase: After all fingerprints are registered, users can log in using any of their registered fingerprints. Each user can vote only once. When a valid fingerprint is detected, the system prompts the user to vote by pressing one of the three buttons.
## Result Display: Once three votes from three distinct users are collected, the system calculates the winner based on vote counts and displays the winning party with the margin over the runner-up party on the LCD screen.
# Hardware Connections Summary:
## Fingerprint Sensor RX/TX: 
Connected to Arduino’s Serial pins.
## LCD Screen: 
Connected to Arduino digital pins for data lines and control (RS, EN, etc.).
## Vote Buttons: 
Connected to digital pins with pull-up resistors enabled.
# Working Example:
User Registration:

The system displays "Fingerprint Enrollment" on the LCD, prompting each user to place their finger on the sensor for registration.
The Arduino reads and stores each unique fingerprint with a user ID.
After three fingerprints per user (and three users), the registration phase ends, and voting begins.
Voting:

A registered user places their finger on the sensor, which identifies their ID.
After successful login, the user is instructed to vote by pressing a button for Party A, B, or C.
The vote is recorded, and the system ensures the user cannot vote again with another registered fingerprint.
Result Calculation:

Once three unique votes are cast, the system stops further voting.
It calculates the winning party based on vote counts and displays the result, indicating the winning party and the margin by which they won over the second-highest party.
This setup is a standalone, secure voting system that leverages biometric fingerprint authentication to ensure each user’s identity and voting integrity, providing real-time feedback via the LCD screen.
