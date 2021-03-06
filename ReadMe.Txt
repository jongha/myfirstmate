MyFirstMateBar
=====

MyFirstMateBar is a sample Internet Explorer 5 toolband written in C++ with combined ATL and MFC code. MyFirstMateBar demonstrates hosting a combo box and two buttons on the toolband that use the IWebBrowser interface of the browser to navigate and execute search functionality for Knowledge Base articles and the MSDN site.

MORE INFORMATION
================

Usage

As a sample, MyFirstMateBar has been kept intentionally simple. It contains two toolbar buttons and a combo box. The first two buttons navigate to the MSDN web site and the MSDN Web Workshop site, respectively. These locations are currently hardcoded.

The combo box is used for searching the entire knowledge base for either a set of keywords or a direct KB article by Q number. To search for keywords, just type them into the edit portion of the toolband and hit enter. To search for a Q article, prefix the KB with a "#" sign, as in "#Q234234". 

The combo box remembers all of the searches entered into it. These can be recalled by dropping down the list and selecting the desired entry.

Architecture

There are 3 main classes in MyFirstMateBar:

CMyFirstMateBarBand: (ATL) base level band object that implements the two core interfaces of toolbands, IInputObject and IDeskband. This class contains most of the generic band hosting code.

CNavigateToolBarCtrl: (MFC) an override of the MFC CToolBarCtrl class that encapsulates the creation of the combo box, buttons, and any other associated toolbar buttons.

This class handles commands generated by the toolbar control using MFC message reflection. This requires a mostly invisible reflection window that sits as a parent between the toolbar control window and the IE-owned rebar band site.

CSearchComboBox: (MFC) an override of the MFC CComboBoxEx class that encapsulates the processing of messages in the combo box, as well as actions taken when a string is entered or an item is selected from the dropdown list.

Sections of Interest:

1. IInputObject::TranslateAccelerator - keystrokes are passed through to input objects (our toolband is considered an "input object" by the IE shell interfaces) via this very important interface. MyFirstMateBar funnels messages from the low-level band up through the toolbar control on to the combo box control.

2. IInputObject::UIActivateIO & IInputObject::HasFocusIO - The IE shell interfaces notify the band object when it has been activated via a TAB by calling UIActivateIO. MyFirstMateBar simply sets the focus to the combo box edit control.

When the edit control gets the focus -- either via UIActivateIO or by a direct mouse click -- it will be notified via a CBEN_BEGINEDIT notification message. To signal a change in focus ownership, MyFirstMateBar needs to call back to the IInputObjectSite::OnFocusChangeIS interface. MyFirstMateBar centralizes this call in the CMyFirstMateBarBand::FocusChange method.

Whenever the IE shell interfaces need to check focus, IInputObject::HasFocusIO is called. This method merely needs to determine if the combo edit control has focus or not, as it is the only control that can keep the focus. This implementation would need to change if further focusable controls are added to the band.

3. DllUnregisterServer hand removes the toolbar key from the registry as the registry scrip does not do this.

REFERENCES
==========

"Creating Custom Explorer Bars, Tool Bands, and Desk Bands": http://msdn.microsoft.com/workshop/browser/ext/overview/bands.asp
