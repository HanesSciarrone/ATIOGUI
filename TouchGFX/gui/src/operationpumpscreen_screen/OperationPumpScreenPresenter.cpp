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
