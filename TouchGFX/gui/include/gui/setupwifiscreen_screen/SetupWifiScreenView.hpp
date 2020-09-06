#ifndef SETUPWIFISCREENVIEW_HPP
#define SETUPWIFISCREENVIEW_HPP

#include <gui_generated/setupwifiscreen_screen/SetupWifiScreenViewBase.hpp>
#include <gui/setupwifiscreen_screen/SetupWifiScreenPresenter.hpp>

/* Include added for user */
#include "gui/containers/ItemList.hpp"
#include "gui/common/CustomKeyboard.hpp"
#include "ModuleWifi.h"

class SetupWifiScreenView : public SetupWifiScreenViewBase
{
public:
    SetupWifiScreenView();
    virtual ~SetupWifiScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    /**
     * Declaring callback handler for TextArea
     */
    void textAreaClickHandler(const TextAreaWithOneWildcard& object, const ClickEvent& event);

    virtual void PopupClickAccept();

    /* Action of search button */
    virtual void ScanNetwork();

    /* Action when you have result of scan network */
    virtual void FillOptionNetwork(WifiMessage_t *networks);

protected:

    static const int numberOfListElements = 30;
    ItemList elements[numberOfListElements];

    CustomKeyboard keyboard;

    // Declaring callback type of box and clickEvent
    Callback<SetupWifiScreenView, const TextAreaWithOneWildcard&, const ClickEvent&> textAreaClickedCallback;


private:

    /**
     * Handler of list element clicks.
     */
    void itemListClicked(ItemList& element);

    // Callback that is assigned to each list element
    Callback<SetupWifiScreenView, ItemList&> itemListClickedCallback;

};

#endif // SETUPWIFISCREENVIEW_HPP
