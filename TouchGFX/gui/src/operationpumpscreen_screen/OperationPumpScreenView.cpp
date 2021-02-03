#include <gui/operationpumpscreen_screen/OperationPumpScreenView.hpp>

#include <stdlib.h>
#include <string.h>

OperationPumpScreenView::OperationPumpScreenView()
: liters_dispensed(0.0),
  selector_enable(true)
{
	strncpy((char *)type_fuel, "Regular", strlen("Regular"));
}

void OperationPumpScreenView::setupScreen()
{
    OperationPumpScreenViewBase::setupScreen();
    dispache_liters = atof((char *)presenter->get_liters_to_dispahe());

    Unicode::snprintfFloat(lbl_titleBuffer, LBL_TITLE_SIZE, "%.3f", dispache_liters);
    lbl_title.resizeToCurrentText();
    lbl_title.invalidate();
    status_operation.setValue(0);
}

void OperationPumpScreenView::tearDownScreen()
{
    OperationPumpScreenViewBase::tearDownScreen();
}

void OperationPumpScreenView::pay_sale_action()
{
	// Sent liters dispensed
}

void OperationPumpScreenView::cancel_sale_action()
{
	uint8_t pump[LBL_TITLE_PUMP_SELECTED_SIZE];

	memset(pump, 0, LBL_TITLE_PUMP_SELECTED_SIZE);
	Unicode::toUTF8(lbl_title_pump_selectedBuffer, pump, LBL_TITLE_PUMP_SELECTED_SIZE);
	presenter->stop_dispatch_action(pump);

	application().gotoMainScreenScreenCoverTransitionNorth();
}

void OperationPumpScreenView::dispatch_fuel_action()
{
	uint8_t pump[LBL_TITLE_PUMP_SELECTED_SIZE];

	memset(pump, 0, LBL_TITLE_PUMP_SELECTED_SIZE);
	Unicode::toUTF8(lbl_title_pump_selectedBuffer, pump, LBL_TITLE_PUMP_SELECTED_SIZE);
	selector_enable = false;
	presenter->dispatch_fuel_action(pump, type_fuel);
}

void OperationPumpScreenView::stop_dispache_action()
{
	uint8_t pump[LBL_TITLE_PUMP_SELECTED_SIZE];

	memset(pump, 0, LBL_TITLE_PUMP_SELECTED_SIZE);
	Unicode::toUTF8(lbl_title_pump_selectedBuffer, pump, LBL_TITLE_PUMP_SELECTED_SIZE);
	presenter->stop_dispatch_action(pump);
}

void OperationPumpScreenView::set_pump_selected(int value)
{
	if (selector_enable) {
		Unicode::snprintfFloat(lbl_title_pump_selectedBuffer, LBL_TITLE_PUMP_SELECTED_SIZE, "%d", value);
		lbl_title_pump_selected.resizeToCurrentText();
		lbl_title_pump_selected.invalidate();
	}
}

void OperationPumpScreenView::select_types_fuel()
{
	if (selector_enable) {
		if (checkbox_regular.getSelected()) {
			strcpy((char *)type_fuel, "Regular");
		}
		else if (checkbox_premium.getSelected()) {
			strcpy((char *)type_fuel, "Premium");
		}
		else if (checkbox_regular_diesel.getSelected()) {
			strcpy((char *)type_fuel, "Regular diesel");
		}
		else if (checkbox_premium_diesel.getSelected()) {
			strcpy((char *)type_fuel, "Premium diesel");
		}
	}
}

void OperationPumpScreenView::show_mesage_pump_controller(uint8_t *message)
{
	popup.setVisible(true);
	Unicode::fromUTF8(message, lbl_popupBuffer, LBL_POPUP_SIZE);
	lbl_popup.resizeToCurrentText();
	lbl_popup.invalidate();
	popup.invalidate();
}

void OperationPumpScreenView::update_state_pump_controller(uint8_t *fuel_dispensed)
{
	int porcent = 0;

	Unicode::fromUTF8(fuel_dispensed, lbl_titleBuffer, LBL_TITLE_SIZE);
	porcent = (100.0*atof((char *)fuel_dispensed))/(this->dispache_liters);
	status_operation.setValue(porcent);
	lbl_title.invalidate();
	status_operation.invalidate();
}

