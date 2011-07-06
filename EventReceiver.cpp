#include "EventReceiver.h"

// This is the one method that we have to implement
bool EventReceiver::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			LeftButtonDown = true;
			break;

		case EMIE_LMOUSE_LEFT_UP:
			LeftButtonDown = false;
			break;

		case EMIE_RMOUSE_PRESSED_DOWN:
			RightButtonDown = true;
			break;

		case EMIE_RMOUSE_LEFT_UP:
			RightButtonDown = false;
			break;

		default:
			break;
		}
	}

	return false;
}

// This is used to check whether a key is being held down
bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

bool EventReceiver::IsLeftButtonDown() const
{
	return LeftButtonDown;
}

bool EventReceiver::IsRightButtonDown() const
{
	return RightButtonDown;
}
	
EventReceiver::EventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;

	LeftButtonDown = false;
	RightButtonDown = false;
}
