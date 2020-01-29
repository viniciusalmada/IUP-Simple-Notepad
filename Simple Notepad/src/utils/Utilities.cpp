#include "utils/Utilities.h"

#include <cstdio>
#include <cstring>
#include <cctype>

#include "iup_config.h"
#include "utils/Constants.h"
#include "iupcomp/IupComp.h"

const char* Utils::strFileTitle(const char* filename)
{
	/* Start at the last character */
	auto len = (int)strlen(filename);
	auto offset = len - 1;
	while (offset != 0)
	{
		if (filename[offset] == '\\' || filename[offset] == '/')
		{
			offset++;
			break;
		}
		offset--;
	}
	return filename + offset;
}

int Utils::stringCompare(const char* l, const char* r, int case_sensitive)
{
	if (!l || !r) return 0;


	while (*l && *r)
	{
		int diff;
		auto l_char = *l;
		auto r_char = *r;

		/* computes the difference of both characters */
		if (case_sensitive)
			diff = l_char - r_char;

		else
			diff = tolower((int)l_char) - tolower((int)r_char);

		/* if they differ we have a result */
		if (diff != 0) return 0;

		/* otherwise process the next character */
		++l;
		++r;
	}

	/* check also for terminator */
	if (*l == *r) return 1;

	/* if second string is at terminator, then it is partially equal */
	if (*r == 0) return 1;

	return 0;
}

int Utils::findString(const char* str, const char* strToFind, int casesensitive)
{
	if (!str || str[0] == 0 || !strToFind || strToFind[0] == 0) return -1;

	auto strLen = (int)strlen(str);
	auto strToFindLen = (int)strlen(strToFind);
	auto count = strLen - strToFindLen;

	if (count < 0) return -1;

	count++;

	for (auto i = 0; i < count; i++)
	{
		if (stringCompare(str, strToFind, casesensitive)) return i;

		str++;
	}

	return -1;
}

char* Utils::readFile(const char* filename)
{
	const auto file = fopen(filename, "rb");
	if (!file)
	{
		IupMessagef("Error!", "Can't open file: %s", filename);
		return nullptr;
	}

	/* calculate file size */
	fseek(file, 0, SEEK_END);
	const auto size = ftell(file);
	fseek(file, 0, SEEK_SET);

	/* Allocate memory for the file contents + nul terminator */
	const auto textFromFile = new char[size + 1];
	/* read all data at once */
	fread(textFromFile, size, 1, file);
	textFromFile[size] = 0;

	if (ferror(file))
		IupMessagef("Error!", "Fail when reading from file: %s", filename);

	fclose(file);
	return textFromFile;
}

int Utils::writeFile(const char* filename, const char* str, int count)
{
	auto file = fopen(filename, "w");
	if (!file)
	{
		IupMessagef("Error", "Can't open file: %s", filename);
		return 0;
	}

	fwrite(str, 1, count, file);

	if (ferror(file))
	{
		IupMessagef("Error", "Fail when writing to file: %s", filename);
	}

	fclose(file);
	return 1;
}

void Utils::newFile(Ihandle* self)
{
	auto dlg = IupGetDialog(self);
	auto multitext = IupGetDialogChild(dlg, Name::MULTITEXT);

	IupSetAttribute(dlg, Attr::TITLE, DEFAULT_TITLE);
	IupSetAttribute(multitext, Attr::FILENAME, nullptr);
	IupSetAttribute(multitext, Attr::DIRTY, Val::NO);
	IupSetAttribute(multitext, Attr::VALUE, "");
}

void Utils::openFile(Ihandle* self, const char* filename)
{
	auto str = readFile(filename);
	if (str)
	{
		auto dlg = IupGetDialog(self);
		auto multitext = IupGetDialogChild(self, Name::MULTITEXT);

		auto config = (Ihandle*)IupGetAttribute(multitext, Attr::CONFIG);
		IupSetfAttribute(dlg, Attr::TITLE, "%s - Simple Notepad", strFileTitle(filename));
		IupSetStrAttribute(multitext, Attr::FILENAME, filename);
		IupSetAttribute(multitext, Attr::DIRTY, Val::NO);
		IupSetStrAttribute(multitext, Attr::VALUE, str);
		IupConfigRecentUpdate(config, filename);
		delete str;
	}
}

void Utils::saveFile(Ihandle* multitext)
{
	auto filename = IupGetAttribute(multitext, Attr::FILENAME);
	auto str = IupGetAttribute(multitext, Attr::VALUE);
	auto count = IupGetInt(multitext, Attr::COUNT);
	if (writeFile(filename, str, count))
		IupSetAttribute(multitext, Attr::DIRTY, Val::NO);
}

void Utils::saveasFile(Ihandle* multitext, const char* filename)
{
	auto str = IupGetAttribute(multitext, Attr::VALUE);
	auto count = IupGetInt(multitext, Attr::COUNT);
	if (writeFile(filename, str, count))
	{
		auto config = (Ihandle*)IupGetAttribute(multitext, Attr::CONFIG);

		IupSetfAttribute(IupGetDialog(multitext), "TITLE", "%s - Simple Notepad", strFileTitle(filename));
		IupSetStrAttribute(multitext, Attr::FILENAME, filename);
		IupSetAttribute(multitext, Attr::DIRTY, Val::NO);
		IupConfigRecentUpdate(config, filename);
	}
}

int Utils::saveCheck(Ihandle* ih)
{
	auto multitext = IupGetDialogChild(ih, Name::MULTITEXT);
	if (IupGetInt(multitext, Attr::DIRTY))
	{
		switch (IupAlarm("Warning", "File not saved! Save it now?", "Yes", "No", "Cancel"))
		{
		case 1: /* save the changes and continue */
			saveFile(multitext);
			break;
		case 2: /* ignore the changes and continue */
			break;
		case 3: /* cancel */
			return 0;
		default: ;
		}
	}
	return 1;
}
