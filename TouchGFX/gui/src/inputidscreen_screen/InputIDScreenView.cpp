#include <gui/inputidscreen_screen/InputIDScreenView.hpp>
#include <gui_generated/inputsalescreen_screen/InputSaleScreenViewBase.hpp>

#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>

InputIDScreenView::InputIDScreenView()
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

void InputIDScreenView::ShowProgreessBar()
{
	background_progress.setVisible(true);
	progress_bar.setVisible(true);
	background_progress.invalidate();
	progress_bar.invalidate();
}

void InputIDScreenView::show_status_credential(uint8_t result)
{
	InputSaleScreenView sale_screen;

	background_progress.setVisible(false);
	progress_bar.setVisible(false);
	progress_bar.setValue(0);
	background_progress.invalidate();
	progress_bar.invalidate();

	if (result == 1) {
		sale_screen.setupScreen();
	}
	else {
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
}
