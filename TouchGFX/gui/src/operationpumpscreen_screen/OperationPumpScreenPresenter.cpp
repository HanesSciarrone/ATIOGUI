#include <gui/operationpumpscreen_screen/OperationPumpScreenView.hpp>
#include <gui/operationpumpscreen_screen/OperationPumpScreenPresenter.hpp>

OperationPumpScreenPresenter::OperationPumpScreenPresenter(OperationPumpScreenView& v)
    : view(v)
{

}

void OperationPumpScreenPresenter::activate()
{

}

void OperationPumpScreenPresenter::deactivate()
{

}

uint8_t *OperationPumpScreenPresenter::get_liters_to_dispahe()
{
	return model->get_liters_to_dispahe();
}

void OperationPumpScreenPresenter::dispatch_fuel_action(uint8_t *pump, uint8_t *type)
{
	model->dispatch_fuel_action(pump, type);
}

void OperationPumpScreenPresenter::stop_dispatch_action(uint8_t *pump)
{
	model->stop_dispatch_action(pump);
}

void OperationPumpScreenPresenter::show_mesage_pump_controller(uint8_t *message)
{
	view.show_mesage_pump_controller(message);
}

void OperationPumpScreenPresenter::update_state_pump_controller(uint8_t *fuel_dispensed)
{
	view.update_state_pump_controller(fuel_dispensed);
}
