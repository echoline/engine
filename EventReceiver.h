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
	bool IsRightButtonDown() const;
	EventReceiver();

	core::array<EKEY_CODE> keypresses;

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool LeftButtonDown;
	bool RightButtonDown;
};

#endif
