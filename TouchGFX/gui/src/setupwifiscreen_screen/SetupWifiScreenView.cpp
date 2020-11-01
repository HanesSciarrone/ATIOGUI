#include <gui/setupwifiscreen_screen/SetupWifiScreenView.hpp>

/* Include added for user */
#include "touchgfx/Bitmap.hpp"
#include "BitmapDatabase.hpp"
#include <string.h>

SetupWifiScreenView::SetupWifiScreenView()
	: keyboard(),
	textAreaClickedCallback(this, &SetupWifiScreenView::textAreaClickHandler),
	itemListClickedCallback(this, &SetupWifiScreenView::itemListClicked)

{
	keyboard.setPosition(90, 15, 320, 240);
	Pop_up.add(keyboard);
}

void SetupWifiScreenView::setupScreen()
{
	tickCounter = 0;

	list.removeAll();
	list.invalidate();

	ProgressBar.getRange(boxProgressMin, boxProgressMax);

	// Setup action of click listener on textPassword
	textPassword.setClickAction(textAreaClickedCallback);
}

void SetupWifiScreenView::tearDownScreen()
{
    SetupWifiScreenViewBase::tearDownScreen();
}

void SetupWifiScreenView::itemListClicked(ItemList& element)
{
	element.getBuffer(textSSIDBuffer, TEXTSSID_SIZE);
	textSSID.invalidate();
}

void SetupWifiScreenView::textAreaClickHandler(const TextAreaWithOneWildcard& object, const ClickEvent& event)
{
	if (&object == &textPassword)
	{
		Pop_up.setVisible(true);
		Pop_up.invalidate();
	}
}

void SetupWifiScreenView::PopupClickAccept()
{
	uint8_t buffer[TEXTPASSWORD_SIZE];

	memset(buffer, 0, TEXTPASSWORD_SIZE);
	keyboard.GetBuffer(buffer);
	Unicode::fromUTF8(buffer, textPasswordBuffer, strlen((char *)buffer));
	textPassword.invalidate();
	Pop_up.setVisible(false);
	Pop_up.invalidate();
}

void SetupWifiScreenView::ScanNetwork()
{
	presenter->MsgScanNetwork();
}

void SetupWifiScreenView::SetDataNetwork()
{
	uint8_t bufferSSID[TEXTSSID_SIZE];
	uint8_t bufferPassword[TEXTPASSWORD_SIZE];

	memset(bufferSSID, 0, TEXTPASSWORD_SIZE);
	memset(bufferPassword, 0, TEXTPASSWORD_SIZE);
	Unicode::toUTF8(textSSIDBuffer, bufferSSID, TEXTSSID_SIZE);
	Unicode::toUTF8(textPasswordBuffer, bufferPassword, TEXTPASSWORD_SIZE);

	presenter->MsgConnectNetwork(bufferSSID, bufferPassword);
}

void SetupWifiScreenView::FillOptionNetwork(WifiMessage_t *networks)
{
	int index, count = 0;

	list.removeAll();

	for (index = 0; index < numberOfListElements; index++)
	{
		if (strlen((char *)networks->listNetwork[index].ssid)) {
			if ( networks->listNetwork[index].rssi > -50 && networks->listNetwork[index].rssi <= -30)
			{
				elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_4_ID), (char *)networks->listNetwork[index].ssid);
			}
			else if (networks->listNetwork[index].rssi > -67 && networks->listNetwork[index].rssi <= -50)
			{
				elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_3_ID), (char *)networks->listNetwork[index].ssid);
			}
			else if (networks->listNetwork[index].rssi > -80 && networks->listNetwork[index].rssi <= -67)
			{
				elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_2_ID), (char *)networks->listNetwork[index].ssid);
			}
			else if (networks->listNetwork[index].rssi > -90 && networks->listNetwork[index].rssi <= -80)
			{
				elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_1_ID), (char *)networks->listNetwork[index].ssid);
			}
			else
			{
				elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_0_ID), (char *)networks->listNetwork[index].ssid);
			}

			count++;
		}
	}

	for (index = 0; index < count; index++)
	{
		elements[index].setAction(itemListClickedCallback);
		list.add(elements[index]);
	}

	list.invalidate();

	BackgroundProgress.setVisible(false);
	BackgroundProgress.invalidate();
	ProgressBar.setVisible(false);
	ProgressBar.setValue(0);
	ProgressBar.invalidate();
	tickCounter = 0;
}

void SetupWifiScreenView::ShowStatusConnection(uint8_t value)
{
	BackgroundProgress.setVisible(false);
	BackgroundProgress.invalidate();
	ProgressBar.setVisible(false);
	ProgressBar.setValue(0);
	tickCounter = 0;

	if (value == 1) {
		application().gotoMainScreenScreenCoverTransitionNorth();
	}
	else {
		warning_connection.setVisible(true);
		warning_connection.invalidate();
	}
}

/* Operation of progress bar */
void SetupWifiScreenView::ShowProgress()
{
	BackgroundProgress.setVisible(true);
	ProgressBar.setVisible(true);
	BackgroundProgress.invalidate();
	ProgressBar.invalidate();
}

void SetupWifiScreenView::updateProgress(uint16_t tick)
{
	ProgressBar.setValue(tick % (boxProgressMax+1));

	if (ProgressBar.getValue() == 100)
	{
		ProgressBar.setValue(0);
	}
}

void SetupWifiScreenView::handleTickEvent()
{
	if (BackgroundProgress.isVisible() && ProgressBar.isVisible())
	{
	    tickCounter++;
	    updateProgress(tickCounter);
	}
}



