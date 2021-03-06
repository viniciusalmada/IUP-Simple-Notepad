﻿#include "iupcomp/IupComp.h"
#include "utils/Constants.h"

#include <string>
#include "iupcomp/IupConfigComp.h"
#include "utils/Callbacks.h"


void IupComp::setName(const char* name)
{
	IupSetAttribute(iupComp, Attr::NAME, name);
}

void IupComp::expand(Expand expand)
{
	char const* expandOp[] = {
		"YES",
		"HORIZONTAL",
		"VERTICAL",
		"HORIZONTALFREE",
		"VERTICALFREE",
		"NO"
	};

	IupSetAttribute(iupComp, Attr::EXPAND, expandOp[(int)expand]);
}

void IupComp::padding(int padHor, int padVer)
{
	std::string padding = std::to_string(padHor);
	padding.append("x").append(std::to_string(padVer));
	IupSetAttribute(iupComp, Attr::PADDING, padding.c_str());
}

void IupComp::image(const char* imageFile)
{
	IupSetAttribute(iupComp, Attr::IMAGE, imageFile);
}

void IupComp::canFocus(bool value)
{
	IupSetAttribute(iupComp, Attr::CANFOCUS,
					value ? Val::YES : Val::NO);
}

void IupComp::margin(int marginHor, int marginVer)
{
	std::string margin = std::to_string(marginHor);
	margin.append("x").append(std::to_string(marginVer));
	IupSetAttribute(iupComp, Attr::MARGIN, margin.c_str());
}

void IupComp::gap(int space)
{
	std::string spaceS = std::to_string(space);
	IupSetAttribute(iupComp, Attr::GAP, spaceS.c_str());
}

void IupComp::tip(const char* text)
{
	IupSetAttribute(iupComp, Attr::TIP, text);
}

void IupComp::setAsRecentItems(IupComp& config)
{
	IupConfigRecentInit(config.handle(), iupComp, Callbacks::configRecentCb, 6);
}

char* IupComp::getName()
{
	return IupGetAttribute(iupComp, Attr::NAME);
}

Ihandle* IupComp::handle()
{
	return iupComp;
}

void IupComp::actionCallback(Icallback action)
{
	IupSetCallback(iupComp, Attr::ACTION, action);
}

void IupComp::flatActionCallback(Icallback action)
{
	IupSetCallback(iupComp, Attr::FLAT_ACTION, action);
}
