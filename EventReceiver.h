#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event);
        virtual bool IsKeyDown(EKEY_CODE keyCode) const;
	EventReceiver();

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif
