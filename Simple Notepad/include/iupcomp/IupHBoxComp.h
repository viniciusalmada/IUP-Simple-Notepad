#ifndef IUP_H_BOX_H
#define IUP_H_BOX_H
#include "IupComp.h"
#include <cstdlib>

class IupHBoxComp : public IupComp
{
public:
	static const int IUP_H_BOX = 4;
	IupHBoxComp(Ihandle* children,...) : IupComp(IupHbox(children, NULL)) {};

	int type() override { return IUP_H_BOX; }
};

#endif
