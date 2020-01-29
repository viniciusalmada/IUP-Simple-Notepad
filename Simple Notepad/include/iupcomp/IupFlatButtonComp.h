#include <iup.h>
#include "IupComp.h"
#ifndef IUP_FLAT_BUTTON_H
#define IUP_FLAT_BUTTON_H

class IupFlatButtonComp : public IupComp
{
public:
	static const int IUP_FLAT_BUTTON = 3;
	IupFlatButtonComp() : IupComp(IupFlatButton(nullptr)){}

	// void tip(const char* text) const;

	int type() override { return IUP_FLAT_BUTTON; }
};

#endif
