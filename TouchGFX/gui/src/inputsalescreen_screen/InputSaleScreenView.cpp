#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include <stdlib.h>
#include <string.h>

InputSaleScreenView::InputSaleScreenView()
{

}

void InputSaleScreenView::setupScreen()
{
    InputSaleScreenViewBase::setupScreen();

    liters = presenter->get_liters_fuel_available();

    Unicode::snprintfFloat(label_titleBuffer, LABEL_TITLE_SIZE, "%.3f", liters);
    label_title.resizeToCurrentText();
    label_title.invalidate();
}

void InputSaleScreenView::tearDownScreen()
{
    InputSaleScreenViewBase::tearDownScreen();
}

void InputSaleScreenView::send_dispatch_operation()
{
	float liters_fuel = 0;
	uint8_t buffer[20], text[TEXT_PUPUP_SIZE];

	pad_numeric.get_buffer(buffer);
	liters_fuel = atof((char *)buffer);

	if (liters_fuel <= liters) {
		presenter->set_liters_to_dispache(buffer);
		application().gotoOperationPumpScreenCoverTransitionSouth();
	}
	else {
		strcpy((char *)text, "You can't dispense this amount\n of fuel, please enter less liters");
		Unicode::fromUTF8(text, text_pupupBuffer, TEXT_PUPUP_SIZE);
		pop_up.setVisible(true);
		pop_up.invalidate();
	}

}


