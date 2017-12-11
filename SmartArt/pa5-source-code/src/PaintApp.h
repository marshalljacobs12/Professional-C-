#pragma once
#include <wx/app.h>

class PaintApp : public wxApp
{
public:
	bool OnInit() override;
private:
	class PaintFrame* mFrame = nullptr;
};
