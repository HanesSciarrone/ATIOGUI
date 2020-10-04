#include <gui/inputidscreen_screen/InputIDScreenView.hpp>
#include <gui/inputidscreen_screen/InputIDScreenPresenter.hpp>

InputIDScreenPresenter::InputIDScreenPresenter(InputIDScreenView& v)
    : view(v)
{

}

void InputIDScreenPresenter::activate()
{

}

void InputIDScreenPresenter::deactivate()
{

}

void InputIDScreenPresenter::sent_credential_to_IoT(uint8_t *buffer, uint16_t length)
{
	model->sent_credential_to_IoT(buffer, length);
}

void InputIDScreenPresenter::ShowProgreessBar()
{
	view.ShowProgreessBar();
}
