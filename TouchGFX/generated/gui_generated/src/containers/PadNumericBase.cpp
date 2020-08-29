/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/PadNumericBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

PadNumericBase::PadNumericBase() :
    buttonCallback(this, &PadNumericBase::buttonCallbackHandler)
{
    setWidth(700);
    setHeight(294);
    ContainerKeyboard.setPosition(472, 0, 228, 294);

    background.setPosition(0, 0, 228, 294);
    background.setColor(touchgfx::Color::getColorFrom24BitRGB(98, 98, 98));
    ContainerKeyboard.add(background);

    button_N1.setXY(13, 12);
    button_N1.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N1.setLabelText(touchgfx::TypedText(T_SINGLEUSEID1));
    button_N1.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N1.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N1.setAction(buttonCallback);
    ContainerKeyboard.add(button_N1);

    button_N2.setXY(83, 12);
    button_N2.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N2.setLabelText(touchgfx::TypedText(T_SINGLEUSEID2));
    button_N2.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N2.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N2.setAction(buttonCallback);
    ContainerKeyboard.add(button_N2);

    button_N3.setXY(153, 12);
    button_N3.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N3.setLabelText(touchgfx::TypedText(T_SINGLEUSEID3));
    button_N3.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N3.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N3.setAction(buttonCallback);
    ContainerKeyboard.add(button_N3);

    button_N4.setXY(13, 82);
    button_N4.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N4.setLabelText(touchgfx::TypedText(T_SINGLEUSEID4));
    button_N4.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N4.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N4.setAction(buttonCallback);
    ContainerKeyboard.add(button_N4);

    button_N5.setXY(83, 82);
    button_N5.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N5.setLabelText(touchgfx::TypedText(T_SINGLEUSEID5));
    button_N5.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N5.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N5.setAction(buttonCallback);
    ContainerKeyboard.add(button_N5);

    button_N6.setXY(153, 82);
    button_N6.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N6.setLabelText(touchgfx::TypedText(T_SINGLEUSEID6));
    button_N6.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N6.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N6.setAction(buttonCallback);
    ContainerKeyboard.add(button_N6);

    button_N7.setXY(13, 152);
    button_N7.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N7.setLabelText(touchgfx::TypedText(T_SINGLEUSEID7));
    button_N7.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N7.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N7.setAction(buttonCallback);
    ContainerKeyboard.add(button_N7);

    button_N8.setXY(83, 152);
    button_N8.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N8.setLabelText(touchgfx::TypedText(T_SINGLEUSEID8));
    button_N8.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N8.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N8.setAction(buttonCallback);
    ContainerKeyboard.add(button_N8);

    button_N9.setXY(153, 152);
    button_N9.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N9.setLabelText(touchgfx::TypedText(T_SINGLEUSEID9));
    button_N9.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N9.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N9.setAction(buttonCallback);
    ContainerKeyboard.add(button_N9);

    button_N0.setXY(83, 222);
    button_N0.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_N0.setLabelText(touchgfx::TypedText(T_SINGLEUSEID10));
    button_N0.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N0.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_N0.setAction(buttonCallback);
    ContainerKeyboard.add(button_N0);

    button_decimal.setXY(13, 222);
    button_decimal.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    button_decimal.setLabelText(touchgfx::TypedText(T_SINGLEUSEID16));
    button_decimal.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_decimal.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    button_decimal.setAction(buttonCallback);
    ContainerKeyboard.add(button_decimal);

    button_delete.setXY(153, 222);
    button_delete.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_DARK_ICONS_TRASH_32_ID), touchgfx::Bitmap(BITMAP_DARK_ICONS_TRASH_32_ID));
    button_delete.setIconXY(17, 14);
    button_delete.setAction(buttonCallback);
    ContainerKeyboard.add(button_delete);

    bkgTextID.setPosition(15, 122, 404, 53);
    bkgTextID.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    bkgTextID.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    bkgTextID.setBorderSize(5);

    textArea.setPosition(22, 128, 392, 40);
    textArea.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea.setLinespacing(0);
    textAreaBuffer[0] = 0;
    textArea.setWildcard(textAreaBuffer);
    textArea.setTypedText(touchgfx::TypedText(T_SINGLEUSEID19));

    add(ContainerKeyboard);
    add(bkgTextID);
    add(textArea);
}

void PadNumericBase::initialize()
{

}

void PadNumericBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &button_N1)
    {
        //Button1
        //When button_N1 clicked call virtual function
        //Call Btn1Click
        Btn1Click();
    }
    else if (&src == &button_N2)
    {
        //Button2
        //When button_N2 clicked call virtual function
        //Call Btn2Click
        Btn2Click();
    }
    else if (&src == &button_N3)
    {
        //Button3
        //When button_N3 clicked call virtual function
        //Call Btn3Click
        Btn3Click();
    }
    else if (&src == &button_N4)
    {
        //Button4
        //When button_N4 clicked call virtual function
        //Call Btn4Click
        Btn4Click();
    }
    else if (&src == &button_N5)
    {
        //Button5
        //When button_N5 clicked call virtual function
        //Call Btn5Click
        Btn5Click();
    }
    else if (&src == &button_N6)
    {
        //Button6
        //When button_N6 clicked call virtual function
        //Call Btn6Click
        Btn6Click();
    }
    else if (&src == &button_N7)
    {
        //Button7
        //When button_N7 clicked call virtual function
        //Call Btn7Click
        Btn7Click();
    }
    else if (&src == &button_N8)
    {
        //Button8
        //When button_N8 clicked call virtual function
        //Call Btn8Click
        Btn8Click();
    }
    else if (&src == &button_N9)
    {
        //Button9
        //When button_N9 clicked call virtual function
        //Call Btn9Click
        Btn9Click();
    }
    else if (&src == &button_N0)
    {
        //Button0
        //When button_N0 clicked call virtual function
        //Call Btn0Click
        Btn0Click();
    }
    else if (&src == &button_decimal)
    {
        //ButtonDecimal
        //When button_decimal clicked call virtual function
        //Call BtnDecimalClick
        BtnDecimalClick();
    }
    else if (&src == &button_delete)
    {
        //ButtonDelete
        //When button_delete clicked call virtual function
        //Call BtnDeleteClick
        BtnDeleteClick();
    }
}