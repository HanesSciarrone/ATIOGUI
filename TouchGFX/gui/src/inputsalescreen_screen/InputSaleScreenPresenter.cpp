#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include <gui/inputsalescreen_screen/InputSaleScreenPresenter.hpp>

InputSaleScreenPresenter::InputSaleScreenPresenter(InputSaleScreenView& v)
    : view(v)
{

}

void InputSaleScreenPresenter::activate()
{

}

void InputSaleScreenPresenter::deactivate()
{

}

float InputSaleScreenPresenter::get_liters_fuel_available(void)
{
	return model->get_liters_fuel_available();
}

void InputSaleScreenPresenter::set_liters_to_dispache(uint8_t *liters_selected)
{
	model->set_liters_to_dispache(liters_selected);
}
