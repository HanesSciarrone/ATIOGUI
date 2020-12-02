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

uint32_t InputSaleScreenPresenter::get_liters_fuel_available(void)
{
	return model->get_liters_fuel_available();
}
