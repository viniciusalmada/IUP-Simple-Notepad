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
 */
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppCStyleCast
// ReSharper disable CppDeprecatedEntity
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedDeclarations
#include <cstdlib>

#include "iup.h"
#include "iup_config.h"
#include "Constants.h"
#include "iupcomp/IupTextComp.h"
#include "utils/Utilities.h"
#include "utils/Callbacks.h"

int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);
	IupImageLibOpen();

	auto config = IupConfig();
	IupSetAttribute(config, Attr::APP_NAME, "simple_notepad");
	IupConfigLoad(config);

	auto multitextIupComp = IupTextComp(IupText(nullptr));
	multitextIupComp.isMultiline(true);
	multitextIupComp.isExpanded(true);
	multitextIupComp.isDirty(false);
	multitextIupComp.setName(Name::MULTITEXT);
	multitextIupComp.setCaretPositionCallback((Icallback)Callbacks::multitextCaretCb);
	multitextIupComp.setValueChangedCallback((Icallback)Callbacks::multitextValueChangedCb);
	multitextIupComp.setDropFileCallback((Icallback)Callbacks::dropFilesCb);

	auto font = IupConfigGetVariableStr(config, Group::MAIN_WINDOW, Key::FONT);
	multitextIupComp.setFont(font);

	auto lblStatusbar = IupLabel("Lin 1, Col 1");
	IupSetAttribute(lblStatusbar, Attr::NAME, Name::STATUSBAR);
	IupSetAttribute(lblStatusbar, Attr::EXPAND, Val::HORIZONTAL);
	IupSetAttribute(lblStatusbar, Attr::PADDING, P_10_X_5);

	auto itemNew = IupItem("New\tCtrl+N", nullptr);
	IupSetAttribute(itemNew, Attr::IMAGE, IUP::IUP_FILE_NEW);
	IupSetCallback(itemNew, Attr::ACTION, Callbacks::itemNewActionCb);
	auto buttonNew = IupFlatButton(nullptr);
	IupSetAttribute(buttonNew, Attr::IMAGE, IUP::IUP_FILE_NEW);
	IupSetCallback(buttonNew, Attr::FLAT_ACTION, Callbacks::itemNewActionCb);
	IupSetAttribute(buttonNew, Attr::TIP, "New (Ctrl+N");
	IupSetAttribute(buttonNew, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(buttonNew, Attr::PADDING, M_5_X_5);

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

	auto btnCut = IupFlatButton(nullptr);
	IupSetAttribute(btnCut, Attr::IMAGE, IUP::IUP_CUT);
	IupSetCallback(btnCut, Attr::FLAT_ACTION, Callbacks::itemCutActionCb);
	IupSetAttribute(btnCut, Attr::TIP, "Cut (Ctrl+X)");
	IupSetAttribute(btnCut, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnCut, Attr::PADDING, M_5_X_5);

	auto btnCopy = IupFlatButton(nullptr);
	IupSetAttribute(btnCopy, Attr::IMAGE, IUP::IUP_COPY);
	IupSetCallback(btnCopy, Attr::FLAT_ACTION, Callbacks::itemCopyActionCb);
	IupSetAttribute(btnCopy, Attr::TIP, "Copy (Ctrl+C)");
	IupSetAttribute(btnCopy, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnCopy, Attr::PADDING, M_5_X_5);

	auto btnPaste = IupFlatButton(nullptr);
	IupSetAttribute(btnPaste, Attr::IMAGE, IUP::IUP_PASTE);
	IupSetCallback(btnPaste, Attr::FLAT_ACTION, Callbacks::itemPasteActionCb);
	IupSetAttribute(btnPaste, Attr::TIP, "Paste (Ctrl+V)");
	IupSetAttribute(btnPaste, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnPaste, Attr::PADDING, M_5_X_5);

	auto btnOpen = IupFlatButton(nullptr);
	IupSetAttribute(btnOpen, Attr::IMAGE, IUP::IUP_FILE_OPEN);
	IupSetAttribute(btnOpen, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnOpen, Attr::TIP, "Open (Ctrl+O)");
	IupSetAttribute(btnOpen, Attr::PADDING, M_5_X_5);

	auto btnSave = IupFlatButton(nullptr);
	IupSetAttribute(btnSave, Attr::IMAGE, IUP::IUP_FILE_SAVE);
	IupSetAttribute(btnSave, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnSave, Attr::TIP, "Save (Ctrl+S)");
	IupSetAttribute(btnSave, Attr::PADDING, M_5_X_5);

	auto btnFind = IupFlatButton(nullptr);
	IupSetAttribute(btnFind, Attr::IMAGE, IUP::IUP_EDIT_FIND);
	IupSetAttribute(btnFind, Attr::CANFOCUS, Val::NO);
	IupSetAttribute(btnFind, Attr::TIP, "Find (Ctrl+F)");
	IupSetAttribute(btnFind, Attr::PADDING, M_5_X_5);


	auto toolbar = IupHbox(buttonNew, btnOpen, btnSave,
							IupSetAttributes(IupLabel(nullptr), "SEPARATOR=VERTICAL"),
							btnCut, btnCopy, btnPaste,
							IupSetAttributes(IupLabel(nullptr), "SEPARATOR=VERTICAL"),
							btnFind, NULL);
	IupSetAttribute(toolbar, Attr::MARGIN, M_5_X_5);
	IupSetAttribute(toolbar, Attr::GAP, "2");

	IupSetCallback(itemOpen, Attr::ACTION, Callbacks::itemOpenActionCb);
	IupSetCallback(btnOpen, Attr::FLAT_ACTION, Callbacks::itemOpenActionCb);
	IupSetCallback(itemSaveas, Attr::ACTION, Callbacks::itemSaveasActionCb);
	IupSetCallback(btnSave, Attr::FLAT_ACTION, Callbacks::itemSaveActionCb);
	IupSetCallback(itemExit, Attr::ACTION, Callbacks::itemExitActionCb);
	IupSetCallback(itemFind, Attr::ACTION, Callbacks::itemFindActionCb);
	IupSetCallback(btnFind, Attr::FLAT_ACTION, Callbacks::itemFindActionCb);
	IupSetCallback(itemGoto, Attr::ACTION, Callbacks::itemGotoActionCb);
	IupSetCallback(itemFont, Attr::ACTION, Callbacks::itemFontActionCb);
	IupSetCallback(itemAbout, Attr::ACTION, Callbacks::itemAboutActionCb);
	IupSetCallback(itemCopy, Attr::ACTION, Callbacks::itemCopyActionCb);
	IupSetCallback(itemCut, Attr::ACTION, Callbacks::itemCutActionCb);
	IupSetCallback(itemPaste, Attr::ACTION, Callbacks::itemPasteActionCb);
	IupSetCallback(itemDelete, Attr::ACTION, Callbacks::itemDeleteActionCb);
	IupSetCallback(itemSelectAll, Attr::ACTION, Callbacks::itemSelectAllActionCb);

	auto recentMenu = IupMenu(nullptr);
	auto submenuRecent = IupSubmenu("Recent &Files", recentMenu);

	auto fileMenu = IupMenu(itemNew, itemOpen, itemSave, itemSaveas, itemRevert, IupSeparator(), submenuRecent,
							itemExit, NULL);
	auto editMenu = IupMenu(itemCut, itemCopy, itemPaste, itemDelete, IupSeparator(), itemFind, itemGoto,
							IupSeparator(), itemSelectAll, NULL);
	auto formatMenu = IupMenu(itemFont, NULL);
	auto helpMenu = IupMenu(itemAbout, NULL);

	auto submenuFile = IupSubmenu("&File", fileMenu);
	auto submenuEdit = IupSubmenu("&Edit", editMenu);
	auto submenuFormat = IupSubmenu("F&ormat", formatMenu);
	auto submenuHelp = IupSubmenu("&Help", helpMenu);

	IupSetCallback(fileMenu, Attr::OPEN_CB, Callbacks::fileMenuOpenCb);
	IupSetCallback(editMenu, Attr::OPEN_CB, Callbacks::editMenuOpenCb);

	auto menu = IupMenu(submenuFile, submenuEdit, submenuFormat, submenuHelp, NULL);

	auto vbox = IupVbox(toolbar, multitextIupComp.handle(), lblStatusbar, NULL);

	auto dlg = IupDialog(vbox);
	IupSetAttributeHandle(dlg, Attr::MENU, menu);
	IupSetAttribute(dlg, Attr::TITLE, "Simple Notepad");
	IupSetAttribute(dlg, Attr::SIZE, "HALFxHALF");
	IupSetCallback(dlg, Attr::CLOSE_CB, Callbacks::itemExitActionCb);
	IupSetCallback(dlg, Attr::DROPFILES_CB, (Icallback)Callbacks::dropFilesCb);
	IupSetAttribute(dlg, Attr::CONFIG, (char*)config);

	IupSetCallback(editMenu, Attr::OPEN_CB, Callbacks::editMenuOpenCb);

	/* parent for pre-defined dialogs in closed functions (IupMessage) */
	IupSetAttributeHandle(nullptr, Attr::PARENTDIALOG, dlg);

	IupSetCallback(dlg, "K_cN", Callbacks::itemNewActionCb);
	IupSetCallback(dlg, "K_cO", Callbacks::itemOpenActionCb);
	IupSetCallback(dlg, "K_cS", Callbacks::itemSaveActionCb);
	IupSetCallback(dlg, "K_cF", Callbacks::itemFindActionCb);
	IupSetCallback(dlg, "K_cG", Callbacks::itemGotoActionCb);

	IupConfigRecentInit(config, recentMenu, Callbacks::configRecentCb, 10);

	IupConfigDialogShow(config, dlg, Group::MAIN_WINDOW);

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
