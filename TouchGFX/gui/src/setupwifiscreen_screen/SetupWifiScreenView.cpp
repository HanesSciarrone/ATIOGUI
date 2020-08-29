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
	int index;

	// Setup action of click listener on textPassword
	textPassword.setClickAction(textAreaClickedCallback);

	for (index = 0; index < numberOfListElements; index++)
	{
		if (index%2 == 0)
		{
			elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_2_ID), "Hanes y Euge");
		}
		else if (index%3 == 0)
		{
			elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_4_ID), "Hanes ama a Euge");
		}
		else
		{
			elements[index].SetupListElement(Bitmap(BITMAP_SIGNAL_0_ID), "No se si Euge me ama");
		}
	}

	for (index = 0; index < numberOfListElements; index++)
	{
		elements[index].setAction(itemListClickedCallback);
		list.add(elements[index]);
	}
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
