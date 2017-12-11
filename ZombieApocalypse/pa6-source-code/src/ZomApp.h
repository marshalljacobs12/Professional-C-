#pragma once
#include <wx/app.h>

class ZomApp : public wxApp
{
public:
	bool OnInit() override;
private:
	class ZomFrame* mFrame = nullptr;
};
