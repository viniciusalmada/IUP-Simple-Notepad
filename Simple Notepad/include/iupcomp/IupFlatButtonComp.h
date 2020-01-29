#ifndef IUP_FLAT_BUTTON_COMP_H
#define IUP_FLAT_BUTTON_COMP_H

#include "IupComp.h"

class IupFlatButtonComp : public IupComp
{
public:
	static const int IUP_FLAT_BUTTON_CODE = 3;
	IupFlatButtonComp() : IupComp(IupFlatButton(nullptr)){}

	int type() override { return IUP_FLAT_BUTTON_CODE; }
};

#endif
