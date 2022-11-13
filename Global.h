#ifndef GlobalESP_h
#define GlobalESP_h

#include <String>
#include <IRremoteESP8266.h>
#include <IRsend.h>
extern String COMMAND;
extern IRsend irsend;  // Set the GPIO to be used to sending the message.
extern bool SEND_DATA;

#endif