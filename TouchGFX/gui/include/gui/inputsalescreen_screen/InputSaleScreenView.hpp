#ifndef INPUTSALESCREENVIEW_HPP
#define INPUTSALESCREENVIEW_HPP

#include <gui_generated/inputsalescreen_screen/InputSaleScreenViewBase.hpp>
#include <gui/inputsalescreen_screen/InputSaleScreenPresenter.hpp>

class InputSaleScreenView : public InputSaleScreenViewBase
{
public:
    InputSaleScreenView();
    virtual ~InputSaleScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void send_dispatch_operation();

protected:

    uint32_t liters;
};

#endif // INPUTSALESCREENVIEW_HPP
