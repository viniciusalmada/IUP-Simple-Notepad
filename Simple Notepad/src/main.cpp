/* Simple Notepad from IUP Tutorial at
 * <https://www.tecgraf.puc-rio.br/iup/en/tutorial/tutorial3.html>
 * Version 1.0
 * -Dialog with multiline text field
 *
 * Version 1.02
 * -Added menu and submenu options
 *
 * Version 1.03
 * -Used pre-defined dialogs
 *
 * Version 1.04
 * -Custom dialogs for Find and Go to
 *
 * Version 1.05
 * -Added a toolbar and a statusbar
 *
 * Version 1.06
 * -Defined hotkeys to menu options
 *
 * Version 1.07
 * -Recent files and configuration file added
 *
 * Version 1.08
 * -Clipboard actions support
 *
 * Version 1.09
 * -Drag and drop support
 * -Command line support
 * -Check file needs to be saved
 *
 * Version 1.09.1
 * -New files created for better organization of the project
 * -Code refactored to use the new file
 * -Iup components classes created
 * -Files reorganized in a C++ project standard
 *
 * Version 1.09.2
 * -Iup Components completely changes the Iup native calls
 *
 *
 */
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppCStyleCast
// ReSharper disable CppDeprecatedEntity
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedDeclarations
#include <cstdlib>

#include "iup.h"
#include "iup_config.h"

#include "utils/Constants.h"
#include "utils/Callbacks.h"
#include "utils/Utilities.h"
#include "iupcomp/IupTextComp.h"
#include "iupcomp/IupLabelComp.h"
#include "iupcomp/IupConfigComp.h"
#include "iupcomp/IupItemComp.h"
#include "iupcomp/IupFlatButtonComp.h"
#include "iupcomp/IupHBoxComp.h"
#include "iupcomp/IupSubmenuComp.h"

void startIup()
{
	IupOpen(nullptr, nullptr);
	IupImageLibOpen();
}

void setupConfig(IupConfigComp& config)
{
	config.setAppName("simple_notepad");
	config.load();
}

void setupMultilineText(IupTextComp& text, IupConfigComp& config)
{
	text.isMultiline(true);
	text.expand(Expand::YES);
	text.isDirty(false);
	text.setName(Name::MULTITEXT);
	text.setCaretPositionCallback((Icallback)Callbacks::multitextCaretCb);
	text.setValueChangedCallback((Icallback)Callbacks::multitextValueChangedCb);
	text.setDropFileCallback((Icallback)Callbacks::dropFilesCb);
	text.setFont(config);
}

void setupStatusBar(IupLabelComp& lbl)
{
	lbl.setName(Name::STATUSBAR);
	lbl.expand(Expand::HORIZONTAL);
	lbl.padding(10, 5);
}

void setupToolbar(IupFlatButtonComp& newFButton,
				IupFlatButtonComp& openFButton,
				IupFlatButtonComp& saveFButton,
				IupFlatButtonComp& cutFButton,
				IupFlatButtonComp& copyFButton,
				IupFlatButtonComp& pasteFButton,
				IupFlatButtonComp& findFButton)
{
	newFButton.image(IUP::IUP_FILE_NEW);
	newFButton.flatActionCallback(Callbacks::itemNewActionCb);
	newFButton.tip("New (Ctrl+N)");
	newFButton.canFocus(false);
	newFButton.padding(5, 5);

	openFButton.image(IUP::IUP_FILE_OPEN);
	openFButton.flatActionCallback(Callbacks::itemOpenActionCb);
	openFButton.tip("Open (Ctrl+O)");
	openFButton.canFocus(false);
	openFButton.padding(5, 5);

	saveFButton.image(IUP::IUP_FILE_SAVE);
	saveFButton.flatActionCallback(Callbacks::itemSaveActionCb);
	saveFButton.tip("Save (Ctrl+S)");
	saveFButton.canFocus(false);
	saveFButton.padding(5, 5);

	cutFButton.image(IUP::IUP_CUT);
	cutFButton.flatActionCallback(Callbacks::itemCutActionCb);
	cutFButton.tip("Cut (Ctrl+X)");
	cutFButton.canFocus(false);
	cutFButton.padding(5, 5);

	copyFButton.image(IUP::IUP_COPY);
	copyFButton.flatActionCallback(Callbacks::itemCopyActionCb);
	copyFButton.tip("Copy (Ctrl+C)");
	copyFButton.canFocus(false);
	copyFButton.padding(5, 5);

	pasteFButton.image(IUP::IUP_PASTE);
	pasteFButton.flatActionCallback(Callbacks::itemPasteActionCb);
	pasteFButton.tip("Paste (Ctrl+V)");
	pasteFButton.canFocus(false);
	pasteFButton.padding(5, 5);

	findFButton.image(IUP::IUP_EDIT_FIND);
	findFButton.flatActionCallback(Callbacks::itemFindActionCb);
	findFButton.tip("Find (Ctrl+F)");
	findFButton.canFocus(false);
	findFButton.padding(5, 5);
}

void setupFileMenu(IupItemComp& newItem, 
				   IupItemComp& openItem, 
				   IupItemComp& saveItem, 
				   IupItemComp& saveAsItem, 
				   IupItemComp& revertItem,
				   IupItemComp& exitItem,
				   IupMenuComp& recentFilesMenu,
				   IupConfigComp& config)
{
	newItem.image(IUP::IUP_FILE_NEW);
	newItem.actionCallback(Callbacks::itemNewActionCb);
	newItem.padding(5, 5);

	openItem.image(IUP::IUP_FILE_OPEN);
	openItem.actionCallback(Callbacks::itemOpenActionCb);
	openItem.padding(5, 5);

	saveItem.image(IUP::IUP_FILE_SAVE);
	saveItem.actionCallback(Callbacks::itemSaveActionCb);
	saveItem.padding(5, 5);
	
	saveAsItem.actionCallback(Callbacks::itemSaveasActionCb);
	saveAsItem.padding(5, 5);
	revertItem.actionCallback(Callbacks::itemRevertActionCb);
	revertItem.padding(5, 5);
	exitItem.actionCallback(Callbacks::itemExitActionCb);
	exitItem.padding(5, 5);
	
	recentFilesMenu.setAsRecentItems(config);
}

int main(int argc, char* argv[])
{
	/***********************************************/
	/*********** IUP STARTUP AND CONFIG ************/
	/***********************************************/
	startIup();

	IupConfigComp config;
	setupConfig(config);

	/***********************************************/
	/************ MAIN MULTITEXT FIELD *************/
	/***********************************************/
	IupTextComp multitextIupComp{ IupText(nullptr) };
	setupMultilineText(multitextIupComp, config);

	/***********************************************/
	/************ STATUS BAR LABEL *****************/
	/***********************************************/
	IupLabelComp lblStatusBar{ "Lin 1, Col 1" };
	setupStatusBar(lblStatusBar);

	/***********************************************/
	/************ TOOLBAR WITH FLAT BUTTONS ********/
	/***********************************************/
	IupFlatButtonComp newFButton;
	IupFlatButtonComp openFButton;
	IupFlatButtonComp saveFButton;
	IupFlatButtonComp cutFButton;
	IupFlatButtonComp copyFButton;
	IupFlatButtonComp pasteFButton;
	IupFlatButtonComp findFButton;
	IupHBoxComp toolbarHBox{
		IupHbox(
			newFButton.handle(),
			openFButton.handle(),
			saveFButton.handle(),
			IupLabelComp::separatorVert(),
			cutFButton.handle(),
			copyFButton.handle(),
			pasteFButton.handle(),
			IupLabelComp::separatorVert(),
			findFButton.handle(),
			NULL)
	};
	setupToolbar(
		newFButton,
		openFButton,
		saveFButton,
		cutFButton,
		copyFButton,
		pasteFButton,
		findFButton
	);
	/***********************************************/
	/************ MENU *****************************/
	/***********************************************/
	/*************** FILE MENU *********************/
	IupItemComp newItem{ "&New\tCtrl+N" };
	IupItemComp openItem{ "&Open...\tCtrl+O" };
	IupItemComp saveItem{ "&Save\tCtrl+S" };
	IupItemComp saveAsItem{ "Save &As..." };
	IupItemComp revertItem{ "&Revert" };
	IupMenuComp recentFilesMenu{};
	IupSubmenuComp recentFilesSubmenu{ "Re&cent Files", recentFilesMenu };
	IupItemComp exitItem{ "E&xit" };
	IupMenuComp fileMenu{
		IupMenu(
			newItem.handle(),
			openItem.handle(),
			saveItem.handle(),
			saveAsItem.handle(),
			revertItem.handle(),
			IupSeparator(),
			recentFilesSubmenu.handle(),
			exitItem.handle(),
			NULL)
	};
	IupSubmenuComp fileSubmenu{ "&File", fileMenu };
	setupFileMenu(newItem, openItem, saveItem, saveAsItem, revertItem, exitItem,
				  recentFilesMenu, config);

	
	IupItemComp itemNew{ "New\tCtrl+N" };
	itemNew.image(IUP::IUP_FILE_NEW);
	itemNew.actionCallback(Callbacks::itemNewActionCb);

	auto itemOpen = IupItem("&Open...\tCtrl+O", nullptr);
	IupSetAttribute(itemOpen, Attr::IMAGE, IUP::IUP_FILE_OPEN);
	auto itemSave = IupItem("&Save\tCtrl+S", nullptr);
	IupSetAttribute(itemSave, Attr::NAME, Name::ITEM_SAVE);
	IupSetAttribute(itemSave, Attr::IMAGE, IUP::IUP_FILE_SAVE);
	IupSetCallback(itemSave, Attr::ACTION, Callbacks::itemSaveActionCb);
	auto itemSaveas = IupItem("Save &As...", nullptr);
	IupSetAttribute(itemSaveas, Attr::NAME, Name::ITEM_SAVEAS);
	auto itemRevert = IupItem("Revert", nullptr);
	IupSetAttribute(itemRevert, Attr::NAME, Name::ITEM_REVERT);
	IupSetCallback(itemRevert, Attr::ACTION, Callbacks::itemRevertActionCb);
	auto itemExit = IupItem("E&xit", nullptr);
	auto itemFind = IupItem("&Find...\tCtrl+F", nullptr);
	IupSetAttribute(itemFind, Attr::IMAGE, IUP::IUP_EDIT_FIND);
	auto itemGoto = IupItem("&Go To...\tCtrl+G", nullptr);
	auto itemFont = IupItem("&Font...", nullptr);
	auto itemAbout = IupItem("&About...", nullptr);

	auto itemCut = IupItem("Cut\tCtrl+X", nullptr);
	auto itemCopy = IupItem("Copy\tCtrl+C", nullptr);
	auto itemPaste = IupItem("Paste\tCtrl+V", nullptr);
	auto itemDelete = IupItem("Delete\tDel", nullptr);
	auto itemSelectAll = IupItem("Select All\tCtrl+A", nullptr);

	IupSetAttribute(itemCopy, Attr::NAME, Name::ITEM_COPY);
	IupSetAttribute(itemCut, Attr::NAME, Name::ITEM_CUT);
	IupSetAttribute(itemPaste, Attr::NAME, Name::ITEM_PASTE);
	IupSetAttribute(itemDelete, Attr::NAME, Name::ITEM_DELETE);

	IupSetAttribute(itemCopy, Attr::IMAGE, IUP::IUP_COPY);
	IupSetAttribute(itemCut, Attr::IMAGE, IUP::IUP_CUT);
	IupSetAttribute(itemPaste, Attr::IMAGE, IUP::IUP_PASTE);
	IupSetAttribute(itemDelete, Attr::IMAGE, IUP::IUP_ERASE);

	IupSetAttribute(toolbarHBox.handle(), Attr::MARGIN, M_5_X_5);
	IupSetAttribute(toolbarHBox.handle(), Attr::GAP, "2");

	IupSetCallback(itemOpen, Attr::ACTION, Callbacks::itemOpenActionCb);
	IupSetCallback(itemSaveas, Attr::ACTION, Callbacks::itemSaveasActionCb);
	IupSetCallback(itemExit, Attr::ACTION, Callbacks::itemExitActionCb);
	IupSetCallback(itemFind, Attr::ACTION, Callbacks::itemFindActionCb);
	IupSetCallback(itemGoto, Attr::ACTION, Callbacks::itemGotoActionCb);
	IupSetCallback(itemFont, Attr::ACTION, Callbacks::itemFontActionCb);
	IupSetCallback(itemAbout, Attr::ACTION, Callbacks::itemAboutActionCb);
	IupSetCallback(itemCopy, Attr::ACTION, Callbacks::itemCopyActionCb);
	IupSetCallback(itemCut, Attr::ACTION, Callbacks::itemCutActionCb);
	IupSetCallback(itemPaste, Attr::ACTION, Callbacks::itemPasteActionCb);
	IupSetCallback(itemDelete, Attr::ACTION, Callbacks::itemDeleteActionCb);
	IupSetCallback(itemSelectAll, Attr::ACTION, Callbacks::itemSelectAllActionCb);


	// auto fileMenu = IupMenu(itemNew.handle(), itemOpen, itemSave, itemSaveas, itemRevert, IupSeparator(), submenuRecent,
	// 						itemExit, NULL);
	auto editMenu = IupMenu(itemCut, itemCopy, itemPaste, itemDelete, IupSeparator(), itemFind, itemGoto,
							IupSeparator(), itemSelectAll, NULL);
	auto formatMenu = IupMenu(itemFont, NULL);
	auto helpMenu = IupMenu(itemAbout, NULL);

	auto submenuEdit = IupSubmenu("&Edit", editMenu);
	auto submenuFormat = IupSubmenu("F&ormat", formatMenu);
	auto submenuHelp = IupSubmenu("&Help", helpMenu);

	IupSetCallback(fileMenu.handle(), Attr::OPEN_CB, Callbacks::fileMenuOpenCb);
	IupSetCallback(editMenu, Attr::OPEN_CB, Callbacks::editMenuOpenCb);

	auto menu = IupMenu(fileSubmenu.handle(), submenuEdit, submenuFormat, submenuHelp, NULL);
	auto vbox = IupVbox(toolbarHBox.handle(), multitextIupComp.handle(), lblStatusBar.handle(), NULL);

	auto dlg = IupDialog(vbox);
	IupSetAttributeHandle(dlg, Attr::MENU, menu);
	IupSetAttribute(dlg, Attr::TITLE, "Simple Notepad");
	IupSetAttribute(dlg, Attr::SIZE, "HALFxHALF");
	IupSetCallback(dlg, Attr::CLOSE_CB, Callbacks::itemExitActionCb);
	IupSetCallback(dlg, Attr::DROPFILES_CB, (Icallback)Callbacks::dropFilesCb);
	IupSetAttribute(dlg, Attr::CONFIG, (char*)config.handle());

	IupSetCallback(editMenu, Attr::OPEN_CB, Callbacks::editMenuOpenCb);

	/* parent for pre-defined dialogs in closed functions (IupMessage) */
	IupSetAttributeHandle(nullptr, Attr::PARENTDIALOG, dlg);

	IupSetCallback(dlg, "K_cN", Callbacks::itemNewActionCb);
	IupSetCallback(dlg, "K_cO", Callbacks::itemOpenActionCb);
	IupSetCallback(dlg, "K_cS", Callbacks::itemSaveActionCb);
	IupSetCallback(dlg, "K_cF", Callbacks::itemFindActionCb);
	IupSetCallback(dlg, "K_cG", Callbacks::itemGotoActionCb);

	IupConfigDialogShow(config.handle(), dlg, Group::MAIN_WINDOW);

	/* initialize the current file */
	Utils::newFile(dlg);
	/* open a file from the command line (allow file association in Windows) */
	if (argc > 1 && argv[1])
	{
		const char* filename = argv[1];
		Utils::openFile(dlg, filename);
	}

	// IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(dlg, Attr::USERSIZE, nullptr);

	IupMainLoop();

	IupClose();

	return EXIT_SUCCESS;
}
