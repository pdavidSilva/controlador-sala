#ifndef Hardware_h
#define Hardware_h

#include "Config.h"

class Hardware
{
    private:
        int __id;
        String __uuid;
        String __token;

    public:       
        Hardware();

        void   restart();
        String getMacAddress() const;

        void   setId(int);
        void   setUuid(String);
        void   setToken(String);
        
        int    getId() const; 
        String getUuid() const; 
        String getToken() const;
        
};

#endif
