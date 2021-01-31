/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/operationpumpscreen_screen/OperationPumpScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

OperationPumpScreenViewBase::OperationPumpScreenViewBase() :
    buttonCallback(this, &OperationPumpScreenViewBase::buttonCallbackHandler),
    sliderValueChangedCallback(this, &OperationPumpScreenViewBase::sliderValueChangedCallbackHandler),
    radioButtonSelectedCallback(this, &OperationPumpScreenViewBase::radioButtonSelectedCallbackHandler)
{

    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    background.setPosition(0, 0, 800, 480);
    background.setColor(touchgfx::Color::getColorFrom24BitRGB(41, 73, 105));

    toolbar.setPosition(0, 0, 800, 62);
    toolbar.setColor(touchgfx::Color::getColorFrom24BitRGB(145, 145, 145));

    logo.setBitmap(touchgfx::Bitmap(BITMAP_CONTROLGAS_ID));
    logo.setPosition(0, 1, 153, 60);
    logo.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    btn_dispatch.setXY(616, 128);
    btn_dispatch.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    btn_dispatch.setLabelText(touchgfx::TypedText(T_SINGLEUSEID68));
    btn_dispatch.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_dispatch.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_dispatch.setAction(buttonCallback);

    bth_stop.setXY(616, 232);
    bth_stop.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    bth_stop.setLabelText(touchgfx::TypedText(T_SINGLEUSEID69));
    bth_stop.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    bth_stop.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    bth_stop.setAction(buttonCallback);

    btn_cancel.setXY(400, 232);
    btn_cancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    btn_cancel.setLabelText(touchgfx::TypedText(T_SINGLEUSEID70));
    btn_cancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_cancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_cancel.setAction(buttonCallback);

    status_operation.setXY(400, 342);
    status_operation.setProgressIndicatorPosition(0, 0, 104, 104);
    status_operation.setRange(0, 100);
    status_operation.setCenter(52, 52);
    status_operation.setRadius(50);
    status_operation.setLineWidth(0);
    status_operation.setStartEndAngle(0, 360);
    status_operation.setBackground(touchgfx::Bitmap(BITMAP_BLUE_PROGRESSINDICATORS_BG_MEDIUM_CIRCLE_INDICATOR_BG_LINE_FULL_ID));
    status_operationPainter.setBitmap(touchgfx::Bitmap(BITMAP_BLUE_PROGRESSINDICATORS_FILL_MEDIUM_CIRCLE_INDICATOR_FILL_LINE_FULL_ID));
    status_operation.setPainter(status_operationPainter);
    status_operation.setValue(0);

    btn_pay_sale.setXY(400, 128);
    btn_pay_sale.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    btn_pay_sale.setLabelText(touchgfx::TypedText(T_SINGLEUSEID71));
    btn_pay_sale.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pay_sale.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pay_sale.setAction(buttonCallback);

    lbl_title.setXY(528, 374);
    lbl_title.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title.setLinespacing(0);
    Unicode::snprintf(lbl_titleBuffer, LBL_TITLE_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID73).getText());
    lbl_title.setWildcard(lbl_titleBuffer);
    lbl_title.resizeToCurrentText();
    lbl_title.setTypedText(touchgfx::TypedText(T_SINGLEUSEID72));

    pump_number_select.setXY(12, 83);
    pump_number_select.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_MEDIUM_SLIDER_ROUND_BACK_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_MEDIUM_SLIDER_ROUND_FILL_ID), touchgfx::Bitmap(BITMAP_BLUE_SLIDER_HORIZONTAL_MEDIUM_INDICATORS_SLIDER_ROUND_NOB_ID));
    pump_number_select.setupHorizontalSlider(2, 19, 2, 0, 310);
    pump_number_select.setValueRange(1, 10);
    pump_number_select.setValue(1);
    pump_number_select.setNewValueCallback(sliderValueChangedCallback);

    lbl_title_pump_selected.setXY(12, 138);
    lbl_title_pump_selected.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title_pump_selected.setLinespacing(0);
    Unicode::snprintf(lbl_title_pump_selectedBuffer, LBL_TITLE_PUMP_SELECTED_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID80).getText());
    lbl_title_pump_selected.setWildcard(lbl_title_pump_selectedBuffer);
    lbl_title_pump_selected.resizeToCurrentText();
    lbl_title_pump_selected.setTypedText(touchgfx::TypedText(T_SINGLEUSEID75));

    checkbox_regular.setXY(20, 196);
    checkbox_regular.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID));
    checkbox_regular.setSelected(true);
    checkbox_regular.setDeselectionEnabled(false);

    checkbox_premium.setXY(20, 262);
    checkbox_premium.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID));
    checkbox_premium.setSelected(false);
    checkbox_premium.setDeselectionEnabled(false);

    checkbox_regular_diesel.setXY(20, 330);
    checkbox_regular_diesel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID));
    checkbox_regular_diesel.setSelected(false);
    checkbox_regular_diesel.setDeselectionEnabled(false);

    checkbox_premium_diesel.setXY(20, 394);
    checkbox_premium_diesel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID));
    checkbox_premium_diesel.setSelected(false);
    checkbox_premium_diesel.setDeselectionEnabled(false);

    lbl_title_regular.setXY(82, 198);
    lbl_title_regular.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title_regular.setLinespacing(0);
    lbl_title_regular.setTypedText(touchgfx::TypedText(T_SINGLEUSEID76));

    lbl_title_premium.setXY(82, 264);
    lbl_title_premium.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title_premium.setLinespacing(0);
    lbl_title_premium.setTypedText(touchgfx::TypedText(T_SINGLEUSEID77));

    lbl_title_regular_diesel.setXY(82, 332);
    lbl_title_regular_diesel.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title_regular_diesel.setLinespacing(0);
    lbl_title_regular_diesel.setTypedText(touchgfx::TypedText(T_SINGLEUSEID78));

    lbl_title_premium_diesel.setXY(82, 396);
    lbl_title_premium_diesel.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    lbl_title_premium_diesel.setLinespacing(0);
    lbl_title_premium_diesel.setTypedText(touchgfx::TypedText(T_SINGLEUSEID79));

    add(background);
    add(toolbar);
    add(logo);
    add(btn_dispatch);
    add(bth_stop);
    add(btn_cancel);
    add(status_operation);
    add(btn_pay_sale);
    add(lbl_title);
    add(pump_number_select);
    add(lbl_title_pump_selected);
    add(checkbox_regular);
    add(checkbox_premium);
    add(checkbox_regular_diesel);
    add(checkbox_premium_diesel);
    add(lbl_title_regular);
    add(lbl_title_premium);
    add(lbl_title_regular_diesel);
    add(lbl_title_premium_diesel);
    radioButtonGroup1.add(checkbox_regular);
    radioButtonGroup1.add(checkbox_premium);
    radioButtonGroup1.add(checkbox_regular_diesel);
    radioButtonGroup1.add(checkbox_premium_diesel);
    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
}

void OperationPumpScreenViewBase::setupScreen()
{

}

void OperationPumpScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btn_dispatch)
    {
        //Dispache_fuel
        //When btn_dispatch clicked call virtual function
        //Call dispatch_fuel_action
        dispatch_fuel_action();
    }
    else if (&src == &bth_stop)
    {
        //Stop_dispache
        //When bth_stop clicked call virtual function
        //Call stop_dispache_action
        stop_dispache_action();
    }
    else if (&src == &btn_cancel)
    {
        //cancel_sale_action
        //When btn_cancel clicked call virtual function
        //Call Cancel_sale
        Cancel_sale();
    }
    else if (&src == &btn_pay_sale)
    {
        //Pay_sale
        //When btn_pay_sale clicked call virtual function
        //Call pay_sale_action
        pay_sale_action();
    }
}

void OperationPumpScreenViewBase::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &pump_number_select)
    {
        //Select_pump
        //When pump_number_select value changed call virtual function
        //Call set_pump_selected
        set_pump_selected(value);
    }
}

void OperationPumpScreenViewBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &checkbox_regular)
    {
        //Select_regular_fuel
        //When checkbox_regular selected call virtual function
        //Call select_types_fuel
        select_types_fuel();
    }
    else if (&src == &checkbox_premium)
    {
        //Select_premium_fuel
        //When checkbox_premium selected call virtual function
        //Call select_types_fuel
        select_types_fuel();
    }
    else if (&src == &checkbox_regular_diesel)
    {
        //Select_regular_diesel_fuel
        //When checkbox_regular_diesel selected call virtual function
        //Call select_types_fuel
        select_types_fuel();
    }
    else if (&src == &checkbox_premium_diesel)
    {
        //Select_premium_diesel_fuel
        //When checkbox_premium_diesel selected call virtual function
        //Call select_types_fuel
        select_types_fuel();
    }
}
