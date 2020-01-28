#include <iup.h>
#include "IupComp.h"
#ifndef IUP_FLAT_BUTTON_H
#define IUP_FLAT_BUTTON_H

class IupFlatButtonComp : public IupComp
{
public:
	IupFlatButtonComp() : IupComp(IupFlatButton(nullptr)){}

	// void tip(const char* text) const;
};

#endif
