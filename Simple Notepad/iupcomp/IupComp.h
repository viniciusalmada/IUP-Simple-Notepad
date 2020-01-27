#ifndef IUP_COMP_H
#define IUP_COMP_H
#include <iup.h>
#include "../utils/Enums.h"

class IupComp
{
protected:
	Ihandle* iupComp;
public:
	IupComp(Ihandle* iupComp) { this->iupComp = iupComp; }
	void setName(const char* name);
	void expand(Expand expand);
	void padding(int padHor, int padVer);

	char* getName();
	Ihandle* handle() { return iupComp; }
};
#endif
