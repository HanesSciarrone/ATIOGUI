#include <gui/setupwifiscreen_screen/SetupWifiScreenView.hpp>

/* Include added for user */
#include "touchgfx/Bitmap.hpp"
#include "BitmapDatabase.hpp"

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
	textPassword.setWildcard(keyboard.GetBuffer());
	textPassword.invalidate();
	Pop_up.setVisible(false);
	Pop_up.invalidate();
}

void SetupWifiScreenView::ScanNetwork()
{
	presenter->MsgScanNetwork();
}

void SetupWifiScreenView::FillOptionNetwork(WifiMessage_t *networks)
{
	int index;

	for (index = 0; index < numberOfListElements; index++)
	{
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
	}

	for (index = 0; index < numberOfListElements; index++)
	{
		elements[index].setAction(itemListClickedCallback);
		list.add(elements[index]);
	}
}
