#ifndef IUP_FLAT_BUTTON_H
#define IUP_FLAT_BUTTON_H

#include <iup.h>
#include "IupComp.h"

class IupFlatButtonComp : public IupComp
{
public:
	static const int IUP_FLAT_BUTTON = 3;
	IupFlatButtonComp() : IupComp(IupFlatButton(nullptr)){}

	int type() override { return IUP_FLAT_BUTTON; }
};

#endif
