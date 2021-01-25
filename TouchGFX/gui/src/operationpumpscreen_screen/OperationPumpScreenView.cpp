#include <gui/operationpumpscreen_screen/OperationPumpScreenView.hpp>

#include <stdlib.h>

OperationPumpScreenView::OperationPumpScreenView()
{

}

void OperationPumpScreenView::setupScreen()
{
    OperationPumpScreenViewBase::setupScreen();
    dispache_liters = atof((char *)presenter->get_liters_to_dispahe());

    Unicode::snprintfFloat(lbl_titleBuffer, LBL_TITLE_SIZE, "%.3f", dispache_liters);
    lbl_title.resizeToCurrentText();
    lbl_title.invalidate();
    status_operation.setValue(0);
    pump_dispaching = false;
    Unicode::fromUTF8((const uint8_t *)"Start", lbl_status_operationBuffer, LBL_STATUS_OPERATION_SIZE);
    lbl_status_operation.resizeToCurrentText();
    lbl_status_operation.invalidate();
}

void OperationPumpScreenView::tearDownScreen()
{
    OperationPumpScreenViewBase::tearDownScreen();
}

void OperationPumpScreenView::pay_sale_action()
{

}

void OperationPumpScreenView::cancel_sale_action()
{

}

void OperationPumpScreenView::dispache_fuel_action()
{

}

void OperationPumpScreenView::stop_dispache_action()
{

}
