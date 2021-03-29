/*
 Project  Name: Challenge 2
     File Name: main.cpp
        Author: Alex Foulds & Rob Fry
   Description: This program allows a MicroBit to send a message to another.
                This is done through a map corresponding to the message being
                sent, translating each character into a set of digital pin
                instructions. There is basic encryption on the message however
                due to the poor clock on the MicroBit, it is not implemented
                exactly as intended or defined in the code.

*/

#include "MicroBit.h"
#include "crypt.h"
#include <stdlib.h>
#include <time.h>
#include <map>


MicroBit uBit;
//DEBUGGING MicroBit serial(USBTX,USBRX);
MicroBitButton ButtonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);
MicroBitPin P1(MICROBIT_ID_IO_P1,MICROBIT_PIN_P1,PIN_CAPABILITY_DIGITAL);

//Global variables/instances for the program
static uint8_t sendPress= 0;
static uint8_t charAppend = 4;
crypt* messageCrypt = new crypt;


//Function declarations and definitions
//MicroBit listener function
void send(MicroBitEvent) {
  sendPress=1;
}
void appendDot(MicroBitEvent){
  charAppend=1;
}
void appendDash(MicroBitEvent){
  charAppend=2;
}
void appendNewChar(MicroBitEvent){
  charAppend=3;
}

//Functions for message reading and translating
char inMap(const ManagedString &morse,  map <char, ManagedString> &codes){
    for (auto iter = codes.begin(); iter!=codes.end(); ++iter){
    if (iter->second == morse){
      return iter->first;
    }
  }
  return '$';
}

void sendingMapChar(const char &toSend,map <char, ManagedString> &codes ){
  ManagedString code = codes.find(toSend)->second;

  code = messageCrypt->toEncrypt(code);
  // DEBUGGING // DEBUGGING serial.send(code);
  // DEBUGGING // DEBUGGING serial.send("\r\n");

  for(int x = 0; x<code.length();x++){
    char bit = code.charAt(x);

    if(bit=='1'){ //send dash
      uBit.io.P1.setDigitalValue(1);
      uBit.sleep(200);
    }
    else{ //send dot
      uBit.io.P1.setDigitalValue(1);
      uBit.sleep(100);
    }

    uBit.io.P1.setDigitalValue(0);
    uBit.sleep(50);
  }
}
void translateString(const ManagedString &toTranslate, ManagedString &finalMessage, map <char, ManagedString> &codes){
  if (toTranslate.length()==0){
    return;
  }
  char toAdd = inMap(toTranslate, codes);
  // DEBUGGING serial.send("adding");
  // DEBUGGING serial.send( toAdd );
  // DEBUGGING serial.send("\r\n");
  if (toAdd=='$'){
    //error
  }
  else{
    finalMessage = finalMessage + toAdd;
  }

  // DEBUGGING serial.send("finalMessage is" + finalMessage +"\r\n");
}






int main()
{
  uBit.init();
  ManagedString morseBuff;
  ManagedString sendBuff;

  //Initialisation of Button event listeners
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_HOLD, send);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, appendDot);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, appendDash);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, appendNewChar);


  map <char, ManagedString> codes {
    // 0 - dot
    // 1 - dash
      {'A',"01"},
      {'B',"1000"},
      {'C',"1010"},
      {'D',"100"},
      {'E',"0"},
      {'F',"0010"},
      {'G',"110"},
      {'H',"0000"},
      {'I',"00"},
      {'J',"0111"},
      {'K',"101"},
      {'L',"0100"},
      {'M',"11"},
      {'N',"10"},
      {'O',"111"},
      {'P',"0110"},
      {'Q',"1101"},
      {'R',"010"},
      {'S',"000"},
      {'T',"1"},
      {'U',"001"},
      {'V',"0001"},
      {'W',"011"},
      {'X',"1001"},
      {'Y',"1011"},
      {'Z',"1100"},
      {'1',"01111"},
      {'2',"00111"},
      {'3',"00011"},
      {'4',"00001"},
      {'5',"00000"},
      {'6',"10000"},
      {'7',"11000"},
      {'8',"11100"},
      {'9',"11110"},
      {'0',"11111"}
  };

  uBit.io.P1.setDigitalValue(0);
  // DEBUGGING serial.baud(115200);
  while(1){
    uBit.sleep(5);

  // ----------------------------------
  // -------- Create Message ----------
  // ----------------------------------

  switch (charAppend) {
    case 1://Append Dot
      morseBuff = morseBuff + '0';

      // DEBUGGING serial.send("adding . (0)\r\n");
      charAppend=4;
      break;
    case 2://Append Dash
      morseBuff = morseBuff + '1';
      // DEBUGGING serial.send("adding - (1)\r\n");
      charAppend=4;
      break;
    case 3://Append NewChar
      if (morseBuff.length()==0){
        sendBuff = sendBuff + ' ';
      }
      else{
        //check against a map
        char tmp = inMap(morseBuff, codes);
        if (tmp!='$'){
          sendBuff = sendBuff + tmp;
          // DEBUGGING serial.send(sendBuff);
          // DEBUGGING serial.send("\r\n");
        }
        else{
          // DEBUGGING serial.send("ERROR\r\n");
        }
      }
      morseBuff="";
      charAppend=4;
      break;
    default:
      break;
  }



  // ----------------------------------
  // ---------- End Message -----------
  // ----------------------------------




  // ----------------------------------
  // ------- Handshake Protocol -------
  // ----------------------------------
  if(sendPress){
    sendPress=0;
    uBit.io.P1.setDigitalValue(1);
    uBit.sleep(50);
    uBit.io.P1.setDigitalValue(0);
    uBit.sleep(10);
    if (uBit.io.P1.getDigitalValue()==1){
      while(uBit.io.P1.getDigitalValue()){}
      uBit.display.image.setPixelValue(2,2,255);





      for (int it = 0;it<sendBuff.length();it++){

        switch (sendBuff.charAt(it)) {
          case ' '://send space
            // DEBUGGING serial.send("Sending space\r\n");
            uBit.io.P1.setDigitalValue(1);
            uBit.sleep(500);
            uBit.io.P1.setDigitalValue(0);
            break;
          default:
            //send map character
            char temp = sendBuff.charAt(it);
            sendingMapChar(temp,codes);
        }

        //Next character pause
        uBit.io.P1.setDigitalValue(1);
        uBit.sleep(1000);
        uBit.io.P1.setDigitalValue(0);


        //End transmission
        if (it == sendBuff.length()-1){

          uBit.io.P1.setDigitalValue(1);
          uBit.sleep(2000);
          uBit.io.P1.setDigitalValue(0);
        }
        uBit.io.P1.setDigitalValue(1);
      }
    }
    else
    {
      uBit.display.image.setPixelValue(1,2,255);
    }
  }

  //Receiving MicroBit
  if(uBit.io.P1.getDigitalValue()==1){
    uBit.io.P1.setDigitalValue(1);
    uBit.display.image.setPixelValue(4,2,255);
    uBit.sleep(600);
    uBit.io.P1.setDigitalValue(0);



    ManagedString toTranslate;
    ManagedString finalMessage;

    // Ready to get message
    while(1){
      uint64_t reading = system_timer_current_time();
      if(uBit.io.P1.getDigitalValue()){
        while(uBit.io.P1.getDigitalValue()==1){}
        uint64_t delta = system_timer_current_time() - reading;


        if(delta<=650 && delta>=240){
          finalMessage=finalMessage+' ';
          // DEBUGGING serial.send("finalMessage append space\r\n");
        }
        if(delta<240 && delta>170){ //dash
          toTranslate = toTranslate + '1';
          // DEBUGGING serial.send("toTranslate append 1\r\n");
        }
        if(delta<160 && delta >70){ //dot
          toTranslate = toTranslate + '0';
          // DEBUGGING serial.send("toTranslate append 0\r\n");
        }
        if(delta<1500 && delta >650){
          //Once message decrypted, it is translated from binary string to char values
          // DEBUGGING serial.send("toTranslate = "+toTranslate+"\r\n");
          toTranslate= messageCrypt->toDecrypt(toTranslate);
          // DEBUGGING serial.send("toTranslate2 = "+toTranslate+"\r\n");
          translateString(toTranslate,finalMessage,codes);
          toTranslate = "";

        }
        if(delta>=1500){
          //end of transmission
          uBit.display.scroll(finalMessage);
          // DEBUGGING serial.send("end of message \r\n");
          // DEBUGGING serial.send( finalMessage + "\r\n");
          break;
        }
      }



    }


  }

  // ----------------------------------
  // -- End Handshake/ Transmission  --
  // ----------------------------------

  }
  delete messageCrypt;
  release_fiber();
}
