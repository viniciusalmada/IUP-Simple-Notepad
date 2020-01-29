#ifndef IUP_V_BOX_COMP_H
#define IUP_V_BOX_COMP_H
#include "IupComp.h"

class IupVBoxComp : public IupComp
{
public:
	static const int IUP_V_BOX_COMP = 10;
	IupVBoxComp(Ihandle* vbox) : IupComp(vbox) {}

	int type() override { return IUP_V_BOX_COMP; }
};
#endif
