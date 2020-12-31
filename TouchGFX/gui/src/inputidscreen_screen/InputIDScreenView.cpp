#include <gui/inputidscreen_screen/InputIDScreenView.hpp>
#include <gui_generated/inputsalescreen_screen/InputSaleScreenViewBase.hpp>

#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include "ModuleWifi.h"

InputIDScreenView::InputIDScreenView()
: card_manual(false)
{

}

void InputIDScreenView::setupScreen()
{
    InputIDScreenViewBase::setupScreen();

    progress_bar.getRange(boxProgressMin, boxProgressMax);
}

void InputIDScreenView::tearDownScreen()
{
    InputIDScreenViewBase::tearDownScreen();
}

void InputIDScreenView::send_credential()
{
	uint8_t credential[PadNumericID.get_buffer_length()];

	PadNumericID.get_buffer(credential);
	presenter->sent_credential_to_IoT(credential, PadNumericID.get_buffer_length());
}

void InputIDScreenView::send_card_id()
{

}

void InputIDScreenView::active_reader()
{
	timeout1.setVisible(true);
	timeout1.invalidate();
	timeout_counter = 0;
	card_manual = true;

	presenter->active_reader();
}

void InputIDScreenView::select_mode_id()
{
	if (Selector.getState()) {
		Input_manual.setVisible(false);
		Input_card.setVisible(true);
	}
	else {
		Input_manual.setVisible(true);
		Input_card.setVisible(false);
	}

	Input_manual.invalidate();
	Input_card.invalidate();
}

void InputIDScreenView::ShowProgreessBar()
{
	background_progress.setVisible(true);
	progress_bar.setVisible(true);
	background_progress.invalidate();
	progress_bar.invalidate();
}

void InputIDScreenView::getting_data_read_card(uint8_t *id, uint8_t length_id)
{
	card_manual = false;
	Unicode::snprintf(card_idBuffer, CARD_ID_SIZE, "%s", id);
	card_id.invalidate();
}

void InputIDScreenView::show_status_credential(uint8_t result)
{
	background_progress.setVisible(false);
	progress_bar.setVisible(false);
	progress_bar.setValue(0);
	background_progress.invalidate();
	progress_bar.invalidate();

	if (result == VALID_USER) {
		application().gotoInputSaleScreenScreenCoverTransitionSouth();
	}
	else if (result == INVALID_USER){
		label1_pop_up.setVisible(true);
		label2_pop_up.setVisible(false);
		label1_pop_up.invalidate();
		label2_pop_up.invalidate();
		pop_up.setVisible(true);
		pop_up.invalidate();
	}
	else {
		label1_pop_up.setVisible(false);
		label2_pop_up.setVisible(true);
		label1_pop_up.invalidate();
		label2_pop_up.invalidate();
		pop_up.setVisible(true);
		pop_up.invalidate();
	}
}

void InputIDScreenView::update_progress(uint16_t tick)
{
	progress_bar.setValue(tick % (boxProgressMax+1));

	if (progress_bar.getValue() >= 100) {
		progress_bar.setValue(0);
	}
}

void InputIDScreenView::handleTickEvent()
{
	if (background_progress.isVisible() && progress_bar.isVisible()) {
		tickCounter++;
		update_progress(tickCounter);
	}

	if (card_manual) {
		timeout_counter++;
		update_clock(timeout_counter);
	}
}

void InputIDScreenView::update_clock(uint16_t timeout_counter)
{
	switch(timeout_counter) {
	case 62: {
		timeout1.setVisible(false);
		timeout2.setVisible(true);
		timeout1.invalidate();
		timeout2.invalidate();
	}
	break;

	case 125: {
		timeout2.setVisible(false);
		timeout3.setVisible(true);
		timeout2.invalidate();
		timeout3.invalidate();
	}
	break;

	case 187: {
		timeout3.setVisible(false);
		timeout4.setVisible(true);
		timeout3.invalidate();
		timeout4.invalidate();
	}
	break;

	case 250: {
		timeout4.setVisible(false);
		timeout5.setVisible(true);
		timeout4.invalidate();
		timeout5.invalidate();
	}
	break;

	case 312: {
		timeout5.setVisible(false);
		timeout6.setVisible(true);
		timeout5.invalidate();
		timeout6.invalidate();
	}
	break;

	case 375: {
		timeout6.setVisible(false);
		timeout7.setVisible(true);
		timeout6.invalidate();
		timeout7.invalidate();
	}
	break;

	case 437: {
		timeout7.setVisible(false);
		timeout8.setVisible(true);
		timeout7.invalidate();
		timeout8.invalidate();
	}
	break;

	case 500: {
		timeout8.setVisible(false);
		timeout9.setVisible(true);
		timeout8.invalidate();
		timeout9.invalidate();
	}
	break;

	case 562: {
		timeout9.setVisible(false);
		timeout10.setVisible(true);
		timeout9.invalidate();
		timeout10.invalidate();
	}
	break;
	}

	if (timeout_counter == 625) {
		timeout10.setVisible(false);
		timeout10.invalidate();
		card_manual = false;
	}
}
