#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include <stdlib.h>

InputSaleScreenView::InputSaleScreenView()
{

}

void InputSaleScreenView::setupScreen()
{
    InputSaleScreenViewBase::setupScreen();

    liters = presenter->get_liters_fuel_available();

    Unicode::snprintf(label_titleBuffer, LABEL_TITLE_SIZE, "%u", liters);
    label_title.invalidate();
}

void InputSaleScreenView::tearDownScreen()
{
    InputSaleScreenViewBase::tearDownScreen();
}

void InputSaleScreenView::send_dispatch_operation()
{
	uint32_t liters_fuel = 0;
	uint8_t buffer[20];

	pad_numeric.get_buffer(buffer);
	liters_fuel = atoi((char *)buffer);

	if (liters_fuel <= liters) {

	}
	else {
		Unicode::fromUTF8((uint8_t *)"You can´t dispense this amount\n of fuel, please enter less liters", text_pupupBuffer, TEXT_PUPUP_SIZE);
		pop_up.setVisible(true);
		pop_up.invalidate();
	}

}

