/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/inputsalescreen_screen/InputSaleScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

InputSaleScreenViewBase::InputSaleScreenViewBase() :
    buttonCallback(this, &InputSaleScreenViewBase::buttonCallbackHandler)
{

    backgroud.setPosition(0, 0, 800, 480);
    backgroud.setColor(touchgfx::Color::getColorFrom24BitRGB(41, 73, 105));

    tool_bar.setPosition(0, 0, 800, 62);
    tool_bar.setColor(touchgfx::Color::getColorFrom24BitRGB(145, 145, 145));

    logo.setBitmap(touchgfx::Bitmap(BITMAP_CONTROLGAS_ID));
    logo.setPosition(0, 1, 153, 60);
    logo.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    pad_numeric.setXY(50, 116);

    button_accept_operation.setXY(322, 331);
    button_accept_operation.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    button_accept_operation.setLabelText(touchgfx::TypedText(T_SINGLEUSEID30));
    button_accept_operation.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_accept_operation.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_accept_operation.setAction(buttonCallback);

    button_cancel_operation.setXY(37, 331);
    button_cancel_operation.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    button_cancel_operation.setLabelText(touchgfx::TypedText(T_SINGLEUSEID31));
    button_cancel_operation.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_cancel_operation.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_cancel_operation.setAction(buttonCallback);

    label_title.setXY(76, 154);
    label_title.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    label_title.setLinespacing(0);
    Unicode::snprintf(label_titleBuffer, LABEL_TITLE_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID33).getText());
    label_title.setWildcard(label_titleBuffer);
    label_title.resizeToCurrentText();
    label_title.setTypedText(touchgfx::TypedText(T_SINGLEUSEID32));

    pop_up.setBackground(touchgfx::BitmapId(BITMAP_BACKGROUND_QWERTY_ID), 150, 105);
    pop_up.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    pop_up.setShadeAlpha(174);
    pop_up.hide();

    btn_popup.setXY(317, 196);
    btn_popup.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    btn_popup.setLabelText(touchgfx::TypedText(T_SINGLEUSEID37));
    btn_popup.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_popup.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_popup.setAction(buttonCallback);
    pop_up.add(btn_popup);

    text_pupup.setPosition(7, 78, 487, 80);
    text_pupup.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    text_pupup.setLinespacing(0);
    text_pupupBuffer[0] = 0;
    text_pupup.setWildcard(text_pupupBuffer);
    text_pupup.setTypedText(touchgfx::TypedText(T_SINGLEUSEID38));
    pop_up.add(text_pupup);

    add(backgroud);
    add(tool_bar);
    add(logo);
    add(pad_numeric);
    add(button_accept_operation);
    add(button_cancel_operation);
    add(label_title);
    add(pop_up);
}

void InputSaleScreenViewBase::setupScreen()
{
    pad_numeric.initialize();
}

void InputSaleScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &button_accept_operation)
    {
        //dispatch_operation
        //When button_accept_operation clicked call virtual function
        //Call send_dispatch_operation
        send_dispatch_operation();
    }
    else if (&src == &button_cancel_operation)
    {
        //back_to_main_screen
        //When button_cancel_operation clicked change screen to MainScreen
        //Go to MainScreen with screen transition towards North
        application().gotoMainScreenScreenCoverTransitionNorth();
    }
    else if (&src == &btn_popup)
    {
        //Interaction1
        //When btn_popup clicked hide pop_up
        //Hide pop_up
        pop_up.setVisible(false);
        pop_up.invalidate();
    }
}
