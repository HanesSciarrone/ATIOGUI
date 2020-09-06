/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/setupwifiscreen_screen/SetupWifiScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

SetupWifiScreenViewBase::SetupWifiScreenViewBase() :
    buttonCallback(this, &SetupWifiScreenViewBase::buttonCallbackHandler)
{

    background.setPosition(0, 0, 800, 480);
    background.setColor(touchgfx::Color::getColorFrom24BitRGB(41, 73, 105));

    ToolBar.setPosition(0, 0, 800, 62);
    ToolBar.setColor(touchgfx::Color::getColorFrom24BitRGB(145, 145, 145));

    logo.setXY(0, 1);
    logo.setBitmap(touchgfx::Bitmap(BITMAP_CONTROLGAS_ID));

    btnSearch.setXY(738, 1);
    btnSearch.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_SEARCH_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_SEARCH_32_ID));
    btnSearch.setIconXY(15, 15);
    btnSearch.setAction(buttonCallback);

    bckScroll.setXY(376, 130);
    bckScroll.setBitmap(touchgfx::Bitmap(BITMAP_BACKGROUND_SCROLL_LIST_ID));

    CntScroll.setPosition(400, 166, 360, 194);
    CntScroll.enableHorizontalScroll(false);
    CntScroll.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(253, 253, 253));

    list.setDirection(touchgfx::SOUTH);
    list.setPosition(0, 0, 250, 250);
    CntScroll.add(list);
    CntScroll.setScrollbarsPermanentlyVisible();

    titile.setXY(30, 96);
    titile.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    titile.setLinespacing(0);
    titile.setTypedText(touchgfx::TypedText(T_SINGLEUSEID21));

    titleSSID.setXY(171, 154);
    titleSSID.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    titleSSID.setLinespacing(0);
    titleSSID.setTypedText(touchgfx::TypedText(T_SINGLEUSEID22));

    titlePassword.setXY(150, 251);
    titlePassword.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    titlePassword.setLinespacing(0);
    titlePassword.setTypedText(touchgfx::TypedText(T_SINGLEUSEID23));

    btnCancel.setXY(30, 342);
    btnCancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_REMOVE_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_REMOVE_32_ID));
    btnCancel.setIconXY(16, 16);
    btnCancel.setAction(buttonCallback);

    btnAccept.setXY(288, 342);
    btnAccept.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DONE_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DONE_32_ID));
    btnAccept.setIconXY(15, 15);

    background_SSID.setPosition(14, 192, 351, 40);
    background_SSID.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    background_SSID.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    background_SSID.setBorderSize(5);

    background_Password.setPosition(14, 291, 351, 40);
    background_Password.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    background_Password.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    background_Password.setBorderSize(5);

    textSSID.setPosition(22, 203, 335, 20);
    textSSID.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textSSID.setLinespacing(0);
    textSSIDBuffer[0] = 0;
    textSSID.setWildcard(textSSIDBuffer);
    textSSID.setTypedText(touchgfx::TypedText(T_SINGLEUSEID24));

    textPassword.setPosition(22, 301, 335, 20);
    textPassword.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textPassword.setLinespacing(0);
    textPasswordBuffer[0] = 0;
    textPassword.setWildcard(textPasswordBuffer);
    textPassword.setTypedText(touchgfx::TypedText(T_SINGLEUSEID25));

    Pop_up.setBackground(touchgfx::BitmapId(BITMAP_BACKGROUND_QWERTY_ID), 150, 105);
    Pop_up.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Pop_up.setShadeAlpha(178);
    Pop_up.hide();

    PopupCancel.setXY(9, 206);
    PopupCancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID));
    PopupCancel.setLabelText(touchgfx::TypedText(T_SINGLEUSEID26));
    PopupCancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    PopupCancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    PopupCancel.setAction(buttonCallback);
    Pop_up.add(PopupCancel);

    PopupAccept.setXY(430, 206);
    PopupAccept.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_ICON_BUTTON_PRESSED_ID));
    PopupAccept.setLabelText(touchgfx::TypedText(T_SINGLEUSEID27));
    PopupAccept.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    PopupAccept.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    PopupAccept.setAction(buttonCallback);
    Pop_up.add(PopupAccept);

    add(background);
    add(ToolBar);
    add(logo);
    add(btnSearch);
    add(bckScroll);
    add(CntScroll);
    add(titile);
    add(titleSSID);
    add(titlePassword);
    add(btnCancel);
    add(btnAccept);
    add(background_SSID);
    add(background_Password);
    add(textSSID);
    add(textPassword);
    add(Pop_up);
}

void SetupWifiScreenViewBase::setupScreen()
{

}

void SetupWifiScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnSearch)
    {
        //Scan
        //When btnSearch clicked call virtual function
        //Call ScanNetwork
        ScanNetwork();
    }
    else if (&src == &btnCancel)
    {
        //BackMain
        //When btnCancel clicked change screen to MainScreen
        //Go to MainScreen with screen transition towards North
        application().gotoMainScreenScreenCoverTransitionNorth();
    }
    else if (&src == &PopupCancel)
    {
        //HideQWERTY
        //When PopupCancel clicked hide Pop_up
        //Hide Pop_up
        Pop_up.setVisible(false);
        Pop_up.invalidate();
    }
    else if (&src == &PopupAccept)
    {
        //AcceptPopup
        //When PopupAccept clicked call virtual function
        //Call PopupClickAccept
        PopupClickAccept();
    }
}
