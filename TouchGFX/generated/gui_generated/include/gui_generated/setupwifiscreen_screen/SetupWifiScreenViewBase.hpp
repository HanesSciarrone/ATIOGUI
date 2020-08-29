/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SETUPWIFISCREENVIEWBASE_HPP
#define SETUPWIFISCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/setupwifiscreen_screen/SetupWifiScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

class SetupWifiScreenViewBase : public touchgfx::View<SetupWifiScreenPresenter>
{
public:
    SetupWifiScreenViewBase();
    virtual ~SetupWifiScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void PopupClickAccept()
    {
        // Override and implement this function in SetupWifiScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box background;
    touchgfx::Box ToolBar;
    touchgfx::Image logo;
    touchgfx::ButtonWithIcon btnSearch;
    touchgfx::Image bckScroll;
    touchgfx::ScrollableContainer CntScroll;
    touchgfx::ListLayout list;
    touchgfx::TextArea titile;
    touchgfx::TextArea titleSSID;
    touchgfx::TextArea titlePassword;
    touchgfx::ButtonWithIcon btnCancel;
    touchgfx::ButtonWithIcon btnAccept;
    touchgfx::BoxWithBorder background_SSID;
    touchgfx::BoxWithBorder background_Password;
    touchgfx::TextAreaWithOneWildcard textSSID;
    touchgfx::ClickListener< touchgfx::TextAreaWithOneWildcard > textPassword;
    touchgfx::ModalWindow Pop_up;
    touchgfx::ButtonWithLabel PopupCancel;
    touchgfx::ButtonWithLabel PopupAccept;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTSSID_SIZE = 22;
    touchgfx::Unicode::UnicodeChar textSSIDBuffer[TEXTSSID_SIZE];
    static const uint16_t TEXTPASSWORD_SIZE = 22;
    touchgfx::Unicode::UnicodeChar textPasswordBuffer[TEXTPASSWORD_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<SetupWifiScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // SETUPWIFISCREENVIEWBASE_HPP
