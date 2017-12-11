#include "PaintApp.h"
#include "PaintFrame.h"

bool PaintApp::OnInit()
{
	mFrame = new PaintFrame( "ProPaint", wxPoint(50, 50), wxSize(1024, 768) );
	
	return true;
}
