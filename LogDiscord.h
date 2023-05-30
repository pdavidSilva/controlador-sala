#ifndef LogDiscord_h
#define LogDiscord_h

#include <iostream>
#include <HTTPClient.h>
#include "Controller.h"


class DiscordLogService
{
  public:
    DiscordLogService();
    void sendLog(String message);
    void log_vida();
};

#endif
