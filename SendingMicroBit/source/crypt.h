/*
     File Name: crypt.h
        Author: Alex Foulds & Rob Fry
   Description: The header file for the crypt class declarations

*/

#ifndef CRYPT_H
#define CRYPT_H
#include "MicroBit.h"

class crypt{
public:
  crypt();
  ~crypt();

  ManagedString toEncrypt (const ManagedString &unCrypted);
  ManagedString toDecrypt (const ManagedString &enCrypted);
};


#endif
