#include "ZomApp.h"
#include "ZomFrame.h"

bool ZomApp::OnInit()
{
	mFrame = new ZomFrame( "Zombie Apocalypse Simulator", wxPoint(50, 50), wxSize(800, 730) );
	
	return true;
}
