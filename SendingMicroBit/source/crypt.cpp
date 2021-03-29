/*
     File Name: crypt.h
        Author: Alex Foulds & Rob Fry
   Description: The definition file for the crypt class

*/

#include "crypt.h"
#include "MicroBit.h"
#include <stdlib.h>
#include <time.h>

crypt::crypt(){
  srand(time(NULL));
}
crypt::~crypt(){}

ManagedString crypt::toEncrypt(const ManagedString& unCrypted){
  ManagedString toPass = unCrypted;
  int length = unCrypted.length();

  //Adds a set of random bit chars to the string, doubling its initial length
  for(int i=0;i<length;i++){
    int toAdd = rand()%6;
    if(toAdd <=2){
      toPass = toPass +'1';
    }
    else{
      toPass = toPass +'0';
    }
  }
  return toPass;
}

ManagedString crypt::toDecrypt(const ManagedString& enCrypted){
  ManagedString decrypted = enCrypted;
  int length = enCrypted.length();

  //Strips half of the string to return the actual code
  length = length/2;
  decrypted = decrypted.substring(0,length);
  return decrypted;
}
