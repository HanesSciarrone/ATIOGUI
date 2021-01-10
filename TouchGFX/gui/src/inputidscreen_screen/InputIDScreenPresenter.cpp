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

void InputIDScreenPresenter::show_status_credential(uint8_t result)
{
	view.show_status_credential(result);
}

void InputIDScreenPresenter::hide_progreess_bar()
{
	view.hide_progreess_bar();
}



void InputIDScreenPresenter::active_reader()
{
	model->active_reader();
}

void InputIDScreenPresenter::getting_data_read_card(uint8_t *id, uint8_t length_id)
{
	view.getting_data_read_card(id, length_id);
}
