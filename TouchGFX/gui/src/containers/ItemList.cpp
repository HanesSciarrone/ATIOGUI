#include <gui/containers/ItemList.hpp>

/* Include added for user */
#include "string.h"

ItemList::ItemList()
{

}

void ItemList::initialize()
{
    ItemListBase::initialize();
}

void ItemList::SetupListElement(const Bitmap &iconBMP, const char *text)
{
	uint8_t index;

	// If network name is greater than buffer I ignore it
	if (strlen(text) >= LABEL_SIZE-1)
	{
		return;
	}

	image.setBitmap(iconBMP);

	// Set text on label
	for (index = 0; index < strlen(text); index++)
	{
		labelBuffer[index] = text[index];
	}
	labelBuffer[index] = '\0';

	invalidate();
}

void ItemList::setAction(GenericCallback< ItemList& >& callback)
{
	viewCallback = &callback;
}

void ItemList::getBuffer(Unicode::UnicodeChar *text, uint16_t length)
{
	uint8_t index;

	if (length <= LABEL_SIZE)
	{
		for (index = 0; index < length; index++)
		{
			text[index] = labelBuffer[index];
		}
	}
	else
	{
		for (index = 0; index < LABEL_SIZE; index++)
		{
			text[index] = labelBuffer[index];
		}
	}
}

void ItemList::SelectItem()
{
	// Inform the view of the event
	if (viewCallback->isValid())
	{
		viewCallback->execute(*this);
;	}
}
