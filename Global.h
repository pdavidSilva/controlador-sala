#ifndef GlobalESP_h
#define GlobalESP_h

#include <String>
#include <IRremoteESP8266.h>
#include <IRsend.h>
extern bool HAS_IR_TO_SEND;
extern String COMMAND_IR;
extern IRsend irsend;  // Set the GPIO to be used to sending the message.

#endif