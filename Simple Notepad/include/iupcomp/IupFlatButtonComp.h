#include <iup.h>
#include "IupComp.h"
#ifndef IUP_FLAT_BUTTON_H
#define IUP_FLAT_BUTTON_H

class IupFlatButtonComp : public IupComp
{
public:
	IupFlatButtonComp(Ihandle* button) : IupComp(button){}

	void tip(const char* text);
};

#endif
