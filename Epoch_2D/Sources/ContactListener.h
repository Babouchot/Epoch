#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2D/Box2D.h>


//This class will be use to do contact special contac check
//Uses : Sensor check, background collision check and some other weird stuff
class ContactListener : public b2ContactListener
{
    private :



    public : 
    
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
};

#endif