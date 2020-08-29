#ifndef ITEMLIST_HPP
#define ITEMLIST_HPP

#include <gui_generated/containers/ItemListBase.hpp>

/* Include added for user */
#include "touchgfx/Bitmap.hpp"
#include <iostream>
#include <string>

using namespace std;

class ItemList : public ItemListBase
{
public:
    ItemList();
    virtual ~ItemList() {}

    virtual void initialize();

    void SetupListElement(const Bitmap &iconBMP, const string text);

    /**
     * @brief Function to get buffer of label element.
     */
    void getBuffer(Unicode::UnicodeChar *text, uint16_t length);

    /**
     * @brief Function used to set callback of class
     */
    void setAction(GenericCallback< ItemList& >& callback);

    /**
     * @brief Override function from ItemListBase
     */
    virtual void SelectItem();

protected:
    /**
     * Callback that notifies the creator of the list element of an event
     */
    GenericCallback< ItemList& >* viewCallback;
};

#endif // ITEMLIST_HPP
