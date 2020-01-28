#include <iup.h>
#include "IupComp.h"
#ifndef IUP_FLAT_BUTTON_H
#define IUP_FLAT_BUTTON_H

class IupFlatButtonComp : public IupComp
{
public:
	IupFlatButtonComp(Ihandle* button) : IupComp(button){}
	IupFlatButtonComp(Ihandle* button,
					  const char* image,
					  Icallback flatAction,
					  const char* tip,
					  bool canFocus,
					  int padHor,
					  int padVer);

	void tip(const char* text);
};

#endif
