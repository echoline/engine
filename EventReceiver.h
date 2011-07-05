#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event);
        bool IsKeyDown(EKEY_CODE keyCode) const;
	bool IsLeftButtonDown() const;
	EventReceiver();

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool LeftButtonDown;
};

#endif
