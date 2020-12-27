#include <gui/mainscreen_screen/MainScreenView.hpp>

#include <gui/inputsalescreen_screen/InputSaleScreenView.hpp>
#include "ModuleWifi.h"

MainScreenView::MainScreenView()
{

}

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();

    progress_bar.getRange(boxProgressMin, boxProgressMax);
}

void MainScreenView::tearDownScreen()
{
    MainScreenViewBase::tearDownScreen();
}

void MainScreenView::ShowProgreessBar()
{
	background_progress.setVisible(true);
	progress_bar.setVisible(true);
	progress_bar.setValue(0);
	background_progress.invalidate();
	progress_bar.invalidate();
}

void MainScreenView::show_status_credential(uint8_t result)
{
	background_progress.setVisible(false);
	progress_bar.setVisible(false);
	progress_bar.setValue(0);
	background_progress.invalidate();
	progress_bar.invalidate();

	if (result == VALID_USER) {
		application().gotoInputSaleScreenScreenCoverTransitionSouth();
	}
	else if (result == INVALID_USER) {
		Popup_windows.setVisible(true);
		Unicode::fromUTF8((const uint8_t *)"Invalid user, please\n input new ID", label_popupBuffer, LABEL_POPUP_SIZE);
		label_popup.resizeToCurrentText();
		label_popup.setXY(110, 64);
		label_popup.invalidate();
		Popup_windows.invalidate();
	}
	else {
		Popup_windows.setVisible(true);
		Unicode::fromUTF8((const uint8_t *)"Problem with service", label_popupBuffer, LABEL_POPUP_SIZE);
		label_popup.resizeToCurrentText();
		label_popup.setXY(87, 64);
		label_popup.invalidate();
		Popup_windows.invalidate();
	}
}

void MainScreenView::update_progress(uint16_t tick)
{
	progress_bar.setValue(tick % (boxProgressMax+1));

	if (progress_bar.getValue() >= 100) {
		progress_bar.setValue(0);
	}
}

void MainScreenView::handleTickEvent()
{
	if (background_progress.isVisible() && progress_bar.isVisible()) {
		tickCounter++;
		update_progress(tickCounter);
	}
}
