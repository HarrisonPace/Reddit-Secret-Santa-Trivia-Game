/*
  Reddit Secret Santa 2017 Trivia Box

  Written by Harrison Pace 2017

  This Program is a project made for /u/pianoplunkster/ for SS 2017.

  The program implements Trivia Questions with an LCD Display and 3 Momentary Buttons.
  The questions are sequentially generated; upon answering all questions (correctly)
  a servo is triggered and the Box containing the gifts can be opened.

  Furthermore, as well as opening a physical box, upon completion the giftee shall receive
  digital gifts through a web interface, which will be activated using the ESP8266 WIFI AP.

*/

//For Testing purposes an AVR Platform is used (Arduino Nano)
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    // initialize lcd
const int button1 = 7;                    // pushbutton 1
const int button2 = 8;                    // pushbutton 2
const int button3 = 9;                    // pushbutton 3
const int piezo = 10;                     // piezo buzzer
const int espPower = 13;                  // ESP8266 Mosfet Controlled Power

//Setup Servo
Servo myservo;


/*
  Trivia Questions

  Trivia Questions are stored in PROGMEM to conserve memory.
  The questions will be sequentially selected, and are selected to be fun
  rather than excessively challenging. --(Giftee is knowledgeable).

  The LCD is limited to 32 Characters, thus questions are short to avoid
  long scrolling text.
*/

//Questions
const char question_0[] PROGMEM = "Long Island is a part of which US state?";
const char question_1[] PROGMEM = "Which shark is the biggest?";
const char question_2[] PROGMEM = "Who do the god orcs worship?";
const char question_3[] PROGMEM = "Fe is the chemical symbol for what element?";
const char question_4[] PROGMEM = "What was Java called before it was called Java?";
const char question_5[] PROGMEM = "Who founded the Free Software Foundation?";
const char question_6[] PROGMEM = "The Daleks are...?";
const char question_7[] PROGMEM = "What is the Capital of Australia?";
const char question_8[] PROGMEM = "Who kissed Leia first?";
const char question_9[] PROGMEM = "How many people have walked on the moon?";
const char question_10[] PROGMEM = "What global food franchise has the most locations?";
const char question_11[] PROGMEM = "In Reservoir Dogs, which     actor played Mr. White?";
const char question_12[] PROGMEM = "How many calories in a      glass of champagne?";
const char question_13[] PROGMEM = "What is the Symbol/Emblem of Hufflepuff?";
const char question_14[] PROGMEM = "What is the Vulcan ritual to eliminate all emotion?";
const char question_15[] PROGMEM = "How many keys are on the      Common Grand Piano?";
const char question_16[] PROGMEM = "What is a Voronoi Diagram    based on?";
const char question_17[] PROGMEM = "What is the target for       Average PM2.5 by W.H.O.?";
const char question_18[] PROGMEM = "Have you ever been in an instituion? Cells.";
const char question_19[] PROGMEM = "What is the highest upvoted picture (in /r/pics)?";
const char question_20[] PROGMEM = "\"Linux is a Cancer ...\"         was said by ...?";
const char question_21[] PROGMEM = "The Aussie slang phrase \"fair dinkum\" means?";
const char question_22[] PROGMEM = "Quote: \"We think too much and feel too little.\":";
const char question_23[] PROGMEM = "What is to best way to search text files?";
const char question_24[] PROGMEM = "What was the codename of     Android 2.1?";
const char question_25[] PROGMEM = "How do we find Happiness?";

//Question Table
const char * const question_table[] PROGMEM =
{
  question_0,
  question_1,
  question_2,
  question_3,
  question_4,
  question_5,
  question_6,
  question_7,
  question_8,
  question_9,
  question_10,
  question_11,
  question_12,
  question_13,
  question_14,
  question_15,
  question_16,
  question_17,
  question_18,
  question_19,
  question_20,
  question_21,
  question_22,
  question_23,
  question_24,
  question_25 };

char lcdbuffer[100]; //memory buffer to store questions and answers

//Question Topics
const char questionh_0[] PROGMEM = "US Geography";
const char questionh_1[] PROGMEM = "Biology";
const char questionh_2[] PROGMEM = "DnD Board Game";
const char questionh_3[] PROGMEM = "Chemistry";
const char questionh_4[] PROGMEM = "Programming";
const char questionh_5[] PROGMEM = "Technology";
const char questionh_6[] PROGMEM = "Doctor Who";
const char questionh_7[] PROGMEM = "World Geography";
const char questionh_8[] PROGMEM = "Star Wars";
const char questionh_9[] PROGMEM = "Space";
const char questionh_10[] PROGMEM = "Food";
const char questionh_11[] PROGMEM = "Movies";
const char questionh_12[] PROGMEM = "Healthy Living";
const char questionh_13[] PROGMEM = "Harry Potter";
const char questionh_14[] PROGMEM = "Star Trek";
const char questionh_15[] PROGMEM = "Music";
const char questionh_16[] PROGMEM = "Computer Science";
const char questionh_17[] PROGMEM = "Climate";
const char questionh_18[] PROGMEM = "Blade Runner";
const char questionh_19[] PROGMEM = "Reddit";
const char questionh_20[] PROGMEM = "Buiness";
const char questionh_21[] PROGMEM = "Language";
const char questionh_22[] PROGMEM = "Classic Movies";
const char questionh_23[] PROGMEM = "Linux";
const char questionh_24[] PROGMEM = "Android";
const char questionh_25[] PROGMEM = "Life Skills";

//Question Topics Table
const char * const questiontitle_table[] PROGMEM =
{
  questionh_0,
  questionh_1,
  questionh_2,
  questionh_3,
  questionh_4,
  questionh_5,
  questionh_6,
  questionh_7,
  questionh_8,
  questionh_9,
  questionh_10,
  questionh_11,
  questionh_12,
  questionh_13,
  questionh_14,
  questionh_15,
  questionh_16,
  questionh_17,
  questionh_18,
  questionh_19,
  questionh_20,
  questionh_21,
  questionh_22,
  questionh_23,
  questionh_24,
  questionh_25 };


//Question 1 Ans
const char ans1_0[] PROGMEM = "Florida";
const char ans1_1[] PROGMEM = "New York"; //answer
const char ans1_2[] PROGMEM = "Maine";

//Question 2 Ans
const char ans2_0[] PROGMEM = "Tiger";
const char ans2_1[] PROGMEM = "Great White";
const char ans2_2[] PROGMEM = "Megalodon"; //answer

//Question 3 Ans
const char ans3_0[] PROGMEM = "Gruumsh"; //answer
const char ans3_1[] PROGMEM = "Tiamat";
const char ans3_2[] PROGMEM = "Lolth";

//Question 4 Ans
const char ans4_0[] PROGMEM = "Gold";
const char ans4_1[] PROGMEM = "Silver";
const char ans4_2[] PROGMEM = "Iron"; //answer

//Question 5 Ans
const char ans5_0[] PROGMEM = "Sun";
const char ans5_1[] PROGMEM = "Oak";  //answer
const char ans5_2[] PROGMEM = "Maple";

//Question 6 Ans
const char ans6_0[] PROGMEM = "Richard Stallman"; //answer
const char ans6_1[] PROGMEM = "Linus Torvalds";
const char ans6_2[] PROGMEM = "Dennis Ritchie";

//Question 7 Ans
const char ans7_0[] PROGMEM = "Robots";
const char ans7_1[] PROGMEM = "Mutants"; //answer
const char ans7_2[] PROGMEM = "Aliens";

//Question 8 Ans
const char ans8_0[] PROGMEM = "Sydney";
const char ans8_1[] PROGMEM = "Kookaburra"; //lol
const char ans8_2[] PROGMEM = "Canberra"; //answer

//Question 9 Ans
const char ans9_0[] PROGMEM = "Chewbacca";
const char ans9_1[] PROGMEM = "Hans";
const char ans9_2[] PROGMEM = "Luke"; //answer

//Question 10 Ans
const char ans10_0[] PROGMEM = "12"; //answer
const char ans10_1[] PROGMEM = "13";
const char ans10_2[] PROGMEM = "14";

//Question 11 Ans
const char ans11_0[] PROGMEM = "McDonalds";
const char ans11_1[] PROGMEM = "Subway"; //answer
const char ans11_2[] PROGMEM = "Pizza Hut";

//Question 12 Ans
const char ans12_0[] PROGMEM = "Harvey Keitel"; //answer
const char ans12_1[] PROGMEM = "Tim Roth";
const char ans12_2[] PROGMEM = "Steve Buscemi";

//Question 13 Ans
const char ans13_0[] PROGMEM = "50-100"; //answer
const char ans13_1[] PROGMEM = "0-50";
const char ans13_2[] PROGMEM = "100-200";

//Question 14 Ans
const char ans14_0[] PROGMEM = "Lion";
const char ans14_1[] PROGMEM = "Badger"; //answer
const char ans14_2[] PROGMEM = "Raven";

//Question 15 Ans
const char ans15_0[] PROGMEM = "Jibaltra";
const char ans15_1[] PROGMEM = "Macaraz";
const char ans15_2[] PROGMEM = "Kolinahr"; //answer

//Question 16 Ans
const char ans16_0[] PROGMEM = "76";
const char ans16_1[] PROGMEM = "91";
const char ans16_2[] PROGMEM = "88"; //answer

//Question 17 Ans
const char ans17_0[] PROGMEM = "Path Coverage";
const char ans17_1[] PROGMEM = "Proximity"; //answer
const char ans17_2[] PROGMEM = "Time";

//Question 18 Ans
const char ans18_0[] PROGMEM = "20-25 ug/m3"; //answer
const char ans18_1[] PROGMEM = "30-50 ug/m3";
const char ans18_2[] PROGMEM = "35-65 ug/m3";

//Question 19 Ans
const char ans19_0[] PROGMEM = "Interlinked.";
const char ans19_1[] PROGMEM = "Cells."; //answer
const char ans19_2[] PROGMEM = "Dreadfully distinct.";

//Question 20 Ans
const char ans20_0[] PROGMEM = "Free Internet Ad"; //answer
const char ans20_1[] PROGMEM = "Flight Attendant";
const char ans20_2[] PROGMEM = "President Obama";

//Question 21 Ans
const char ans21_0[] PROGMEM = "Steve Jobs";
const char ans21_1[] PROGMEM = "Steve Balmer"; //answer
const char ans21_2[] PROGMEM = "Peter Thiel";

//Question 22 Ans
const char ans22_0[] PROGMEM = "Definitely! (Yes)";
const char ans22_1[] PROGMEM = "Baby Sheep";
const char ans22_2[] PROGMEM = "Really? (True)"; //answer

//Question 23 Ans
const char ans23_0[] PROGMEM = "The Great Dictator"; //answer
const char ans23_1[] PROGMEM = "Casablanca";
const char ans23_2[] PROGMEM = "Wonderful Life";

//Question 24 Ans
const char ans24_0[] PROGMEM = "grep"; //answer
const char ans24_1[] PROGMEM = "dd";
const char ans24_2[] PROGMEM = "cat";

//Question 25 Ans
const char ans25_0[] PROGMEM = "Froyo";
const char ans25_1[] PROGMEM = "Gingerbread";
const char ans25_2[] PROGMEM = "Eclair"; //answer

//Question 26 Ans
const char ans26_0[] PROGMEM = "Following Others";
const char ans26_1[] PROGMEM = "Giving up";
const char ans26_2[] PROGMEM = "Our own way :)"; //answer

//2D Array - Ans Table
const char * const ans[][26] PROGMEM =
{
    {
      ans1_0,  ans2_0,  ans3_0,  ans4_0,  ans5_0,
      ans6_0,  ans7_0,  ans8_0,  ans9_0,  ans10_0,
      ans11_0, ans12_0, ans13_0, ans14_0, ans15_0,
      ans16_0, ans17_0, ans18_0, ans19_0, ans20_0,
      ans21_0, ans22_0, ans23_0, ans24_0, ans25_0,
      ans26_0
    },
    {
      ans1_1,  ans2_1,  ans3_1,  ans4_1,  ans5_1,
      ans6_1,  ans7_1,  ans8_1,  ans9_1,  ans10_1,
      ans11_1, ans12_1, ans13_1, ans14_1, ans15_1,
      ans16_1, ans17_1, ans18_1, ans19_1, ans20_1,
      ans21_1, ans22_1, ans23_1, ans24_1, ans25_1,
      ans26_1
    },
    {
      ans1_2,  ans2_2,  ans3_2,  ans4_2,  ans5_2,
      ans6_2,  ans7_2,  ans8_2,  ans9_2,  ans10_2,
      ans11_2, ans12_2, ans13_2, ans14_2, ans15_2,
      ans16_2, ans17_2, ans18_2, ans19_2, ans20_2,
      ans21_2, ans22_2, ans23_2, ans24_2, ans25_2,
      ans26_2
    },

};


//Correct Answer Table
const int correctans[] PROGMEM =
{
  2, 3, 1, 3, 2, 1, 2, 3, 3, 1, 2, 1, 1, 2, 3, 3, 2, 1, 2, 1, 2, 3, 1, 1, 3, 3
};

/*
  Trivia Dialouge

  This dialogue is used at the start of the game, for introductory purposes.
*/

//Introduction dialogue
const char dialouge_0[]  PROGMEM = "Wow, What a long flight!";
const char dialouge_1[]  PROGMEM = "Hello, pianoplunkster!";
const char dialouge_2[]  PROGMEM = "Welcome to SS Trivia!";
const char dialouge_3[]  PROGMEM = "Are you ready to play?";
const char dialouge_4[]  PROGMEM = "Hold Red to Begin...";
const char dialouge_5[]  PROGMEM = "I'm your host, Snoo...";
const char dialouge_6[]  PROGMEM = "Enjoying Christmas?";
const char dialouge_7[]  PROGMEM = "I hope so!";
const char dialouge_8[]  PROGMEM = "Today is your chance to win great prizes!";
const char dialouge_9[]  PROGMEM = "Press Red to Continue.";
const char dialouge_10[] PROGMEM = "Let's play a game...";
const char dialouge_11[] PROGMEM = "... of Trivia!";
const char dialouge_12[] PROGMEM = "Here are the rules:";
const char dialouge_13[] PROGMEM = "There are 20 Multiple Choice Questions."; //answer 20 mc
const char dialouge_14[] PROGMEM = "With 3 Answers";
const char dialouge_15[] PROGMEM = "Answer using the Buttons";
const char dialouge_16[] PROGMEM = "Answer correctly to advance. :D";
const char dialouge_17[] PROGMEM = "...Incorrectly and you decline :(";
const char dialouge_18[] PROGMEM = "Don't worry, no time limits. ";
const char dialouge_19[] PROGMEM = "Ready?";
const char dialouge_20[] PROGMEM = "Let's Play! Good Luck";
const char dialouge_21[] PROGMEM = "Press Red to Begin...";

//Introduction dialogue Table
const char * const dialouge_table[] PROGMEM =
{
  dialouge_0,
  dialouge_1,
  dialouge_2,
  dialouge_3,
  dialouge_4,
  dialouge_5,
  dialouge_6,
  dialouge_7,
  dialouge_8,
  dialouge_9,
  dialouge_10,
  dialouge_11,
  dialouge_12,
  dialouge_13,
  dialouge_14,
  dialouge_15,
  dialouge_16,
  dialouge_17,
  dialouge_18,
  dialouge_19,
  dialouge_20,
  dialouge_21 };

char dialougebuffer[100]; //memory buffer to store dialogue

/*
  Trivia Conclusion

  This dialogue is used at the end of the game, for explainatory purposes.
*/

//Conclusion dialogue
const char msg_0[]  PROGMEM = "Congratulations!";
const char msg_1[]  PROGMEM = "The Game is Complete!";
const char msg_2[]  PROGMEM = "The Box is now open!!!";
const char msg_3[]  PROGMEM = "Enjoy your Gifts!";
const char msg_4[]  PROGMEM = "AP: Secret_Santa";
const char msg_5[]  PROGMEM = "IP: 42.42.42.42";

//Conclusion dialogue Table
const char * const conclusion_table[] PROGMEM =
{
msg_0,
msg_1,
msg_2,
msg_3,
msg_4,
msg_5 };

  /*
    displayQuestionLCD Function

    Parameter: Display Text (String)
    Returns: Next Line break

    This function takes a string as a parameter and outputs it to the LCD.
    To ensure the text is displayed correctly, linebreaks are made on
    whitespaces. Thus achieving text wrapping on the LCD.

    Text scrolling offers a viable alternative however ghosting is an issue
    on the low cost 16x2 display used.
  */

int buttonDetection() {
  int buttonPressed = 0;
  if (digitalRead(button1) == 1) {
      delay(20);
      if (digitalRead(button1) == 1) {
        buttonPressed = 1;
      }
  } else if (digitalRead(button2) == 1) {
    delay(20);
    if (digitalRead(button2) == 1) {
      buttonPressed = 2;
    }
  } else if (digitalRead(button3) == 1) {
    delay(20);
    if (digitalRead(button3) == 1) {
      buttonPressed = 3;
    }
  } else {
    buttonPressed = 0;
  }
  return buttonPressed;
}

void openBox () {
    myservo.write(155);
}

void closeBox () {
    myservo.write(60);
}

int displayQuestionLCD(String s) {

  if (s.length() <= 16) {
    lcd.clear();
    for (int unsigned i = 0; i < s.length(); i++){
        lcd.write(s[i]);
    }
    return 0;
  } else {
    lcd.clear();

    //Set Linebreak
    int breakpos = 16;
    for (int i = 0; i < 17; i++){
        if (s[i] == ' ') {
          breakpos = i;
        }
    }
    breakpos = breakpos + 1; // add 1 for < operator

    if (static_cast< unsigned int >( breakpos ) > s.length()) {
      breakpos = s.length();
    }

    lcd.setCursor(0, 0);
    for (int i = 0; i < breakpos; i++){
        lcd.write(s[i]);
    }

    //Set 2nd Linebreak
    int breakpos2 = 32;
    for (int i = breakpos; i < 33; i++){
        if (s[i] == ' ') {
          breakpos2 = i;
        }
    }
    breakpos2 = breakpos2 + 1; // add 1 for < operator

    if (static_cast < unsigned int >( breakpos2 ) > s.length()) {
      breakpos2 = s.length();
    }

    lcd.setCursor(0, 1);
    for (int i = breakpos; i < breakpos2; i++){
        lcd.write(s[i]);
    }

    if (static_cast < unsigned int >( breakpos2 ) == s.length()){
      return 0;
    }
    return breakpos2;
  }
}

/*
  outputText Function

  Parameter: Text (String), Time Delay

  This function takes a string as a parameter and calls the displayQuestionLCD
  Function. It uses the last linebreak returned to produce a trimmed substring.
  Using this as the new parameter. The function iterates until all the text has
  been displayed on the LCD.
*/

void outputText (String s, int timeDelay) {
  int i = 1;
  while (i != 0) {
    i = displayQuestionLCD(s);
    delay(timeDelay); //use delay instead of millis() timer for simplicity
    if (i != 0) {
      s = s.substring(i);
    }
  }
}

/*
  centreText Function

  Parameter: Text (String)

  This function takes a string as a parameter and uses its length to determine
  how to centre position it on the lcd screen by setting the cursor position.
*/

int centerText (String s) {
  //sets cursor pos to centre text
  int cursorpos = 0;
  if (s.length() <= 16) {
    cursorpos = (16 - s.length())/2;
  }
  return cursorpos;
}

/*
  questionTitle Function

  Parameter: question_no (Int)

  This function takes an int as a parameter and uses its to determine Which
  question title to display on the lcd.
*/

void questionTitle (int question_no) {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Question " + String(question_no + 1));
  strcpy_P(lcdbuffer, (char*)pgm_read_word(&(questiontitle_table[question_no])));
  lcd.setCursor(centerText(lcdbuffer), 1);
  lcd.print(lcdbuffer);
  delay(2000);
}

/*
  successTune Function

  This function plays a basic tune through a piezo buzzer, (using hard delays
  for timing). The tune is played at the start of the session and upon
  completion.
*/

void successTune () {
  int notes[] =
    {261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 698, 783, 880};
  int duration = 200;
  for (int i = 0; i < 9; i++)
  {
    tone(piezo, notes[i], duration);
    delay(100);
    tone(piezo, notes[i+3], duration);
    delay(100);
  }
}

/*
  triviaIntro Function

  This function prints the trivia intro dialouge and handles button inputs.
*/

void triviaIntro() {

  lcd.clear();
  for (int i = 0; i < 3; i++) {
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[i]))); // Facilitates necessary casts and dereferencing.
    //Serial.println( dialougebuffer ); //debug
    outputText(dialougebuffer, 4000);
  }
  successTune();
  strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[3])));
  //Serial.println( dialougebuffer ); //debug
  outputText(dialougebuffer, 3000);

  strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[4])));
  outputText(dialougebuffer, 0);
  while (buttonDetection() != 1) {
    //do nothing (until button pressed)
  };
  for (int i = 4; i < 9; i++) {
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[i])));
    //Serial.println( dialougebuffer ); //debug
    outputText(dialougebuffer, 3000);
  }
  strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[9])));
  outputText(dialougebuffer, 0);
  while (buttonDetection() != 1) {
    //do nothing (until button pressed)
  };
  for (int i = 10; i < 21; i++) {
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[i])));
    //Serial.println( dialougebuffer ); //debug
    outputText(dialougebuffer, 3000);
  }
  strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[21])));
  outputText(dialougebuffer, 0);
  while (buttonDetection() != 1) {
    //do nothing (until button pressed)
  };
}

/*
  triviaIntro Function

  This function prints the trivia completion dialouge and calls the completion
  tasks.
*/

void triviaFinish() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(conclusion_table[i]))); // Facilitates necessary casts and dereferencing.
    //Serial.println( dialougebuffer ); //debug
    outputText(dialougebuffer, 3500);
  }
    openBox();
    successTune();
    lcd.setCursor(0, 0);
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(conclusion_table[4])));
    lcd.print(dialougebuffer);
    lcd.setCursor(0, 1);
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(conclusion_table[5])));
    lcd.print(dialougebuffer);
    while (buttonDetection() != 1) {
      //do nothing (until button pressed)
    };
}

void correctScreen () {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Correct! :D");
  delay(2000);
}

void incorrectScreen () {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Incorrect! :(");
  delay(2000);
}

/*
  questionScreen Function

  Parameter: question_no (Int), Progress (Int),

  This function takes question_no & progress (int's) as parameters, to
  handle the display of questions on the LCD screen, as well as button
  selection. The code is rather arduous, due to handling of timing events
  and calls to PROGMEM. Its function is simple however, print questions &
  imediately handle inputs.
*/


int questionScreen(int question_no, int progress) {
  int sel = 0;
  int state = 0;
  int displayupdate = 0;
  int i = 1;
  int unsigned questiontime;
  String stringbuffer;
  unsigned long previousMillis = 0;
  strcpy_P(lcdbuffer, (char*)pgm_read_word(&(question_table[question_no])));

  if ((String(lcdbuffer)).length() <= 25 ) { //Setting a fixed size is a poor way to handle this (will lead to trimming)
    questiontime = 2000;
  } else if ((String(lcdbuffer)).length() <= 50 ) {
    questiontime = 4000;
  } else {
    questiontime = 6000;
  }

  while (sel == 0) {
    //Update Timer
    unsigned long currentMillis = millis();
    //Read Button press
    sel = buttonDetection();

    //Print Question + Answers
    if ((currentMillis - previousMillis) >= 0 && (currentMillis - previousMillis) <= questiontime ) {
      if (state != 1) {
        strcpy_P(lcdbuffer, (char*)pgm_read_word(&(question_table[question_no])));
        stringbuffer = String(lcdbuffer);
        state = 1;
      }
      if (((currentMillis - previousMillis) >= 0) && displayupdate == 0) {
          i = displayQuestionLCD(stringbuffer);
          if (i != 0) {
            stringbuffer = stringbuffer.substring(i);
          }
          displayupdate = 1;
      }
      if (((currentMillis - previousMillis) >= 2000) && displayupdate == 1) {
          i = displayQuestionLCD(stringbuffer);
          if (i != 0) {
            stringbuffer = stringbuffer.substring(i);
          }
          displayupdate = 2;
      }
      if (((currentMillis - previousMillis) >= 4000) && displayupdate == 2) {
          i = displayQuestionLCD(stringbuffer);
          if (i != 0) {
            stringbuffer = stringbuffer.substring(i);
          }
          displayupdate = 3;
      }

    } else if ((currentMillis - previousMillis) > questiontime && (currentMillis - previousMillis) <= (questiontime + 2000) ) {
      if (state != 2) {
        strcpy_P(lcdbuffer, (char*)pgm_read_word(&(ans[0][question_no])));
        stringbuffer = String("A: ") + String(lcdbuffer);
        outputText(stringbuffer, 0);
        state = 2;
      }
    } else if ((currentMillis - previousMillis) > (questiontime + 2000) && (currentMillis - previousMillis) <= (questiontime + 4000) ) {
      if (state != 3) {
        strcpy_P(lcdbuffer, (char*)pgm_read_word(&(ans[1][question_no])));
        stringbuffer = String("B: ") + String(lcdbuffer);
        outputText(stringbuffer, 0);
        state = 3;
      }
    } else if ((currentMillis - previousMillis) > (questiontime + 4000) && (currentMillis - previousMillis) <= (questiontime + 6000) ) {
      if (state != 4) {
        strcpy_P(lcdbuffer, (char*)pgm_read_word(&(ans[2][question_no])));
        stringbuffer = String("C: ") + String(lcdbuffer);
        outputText(stringbuffer, 0);
        state = 4;
      }
    } else {
      previousMillis = currentMillis;
      state = 0;
      displayupdate = 0;
    }
  }

  if (sel != 0) {
    //button management
    if (sel == (pgm_read_byte(&(correctans[question_no])))) {
      progress = progress + 1;
      correctScreen();
    } else {
      incorrectScreen();
    }
  }
  return progress;
}

void displayScore(int question_no, int progress) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Score " + String(progress) + " / " + String(question_no + 1));
  delay(2000);
}


void gameFailure () {
  lcd.clear();
  lcd.setCursor(0, 0);
  outputText("Game Over! :(", 2000);
  outputText("Please try again.", 2000);
}

int checkProgress(int question_no, int progress) {
  int state = 0;
  if (abs(question_no-progress) > 3) {
    state = 1;
  }
  if (progress == 20) {
    state = 2;
  }
  return state;
}

void espPowerOn() {
  digitalWrite(espPower, LOW); //inverse logic (reset)
}

void espPowerOff() {
  digitalWrite(espPower, HIGH); //inverse logic (reset)
}

void setup() {

  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("I'm Alive!");
  myservo.attach(6);
  myservo.write(60); // set pos locked
  espPowerOff();
  triviaIntro(); //disable for debugging
}

void loop() {

  int progress = 0; //variable defines user progression (score)
  int question_no = 0; //variable defines question
  int state = 0; //Game Progression State
  int exitState = 0; //Trivia Completion State
  closeBox();
  for (; question_no < 26 && exitState == 0; question_no++) {
    questionTitle(question_no);
    progress = questionScreen(question_no, progress);
    displayScore(question_no, progress);
    state = checkProgress(question_no, progress);
    if (state == 1) {
      gameFailure();
      progress = 0;
      question_no = 0;
      exitState = 1;
    } else if (state == 2) {
      triviaFinish();
      espPowerOn();
      openBox();
      progress = 0;
      question_no = 0;
      exitState = 1;
    }
  }
  if ((progress < 20) && (question_no == 25)) {
      gameFailure();
      progress = 0;
      question_no = 0;
      exitState = 1;
      state = 1;
  }
  if (state == 1) {
    strcpy_P(dialougebuffer, (char*)pgm_read_word(&(dialouge_table[9])));
    outputText(dialougebuffer, 0);
    while (buttonDetection() != 1) {
      //do nothing (until button pressed)
    };
  }
}
