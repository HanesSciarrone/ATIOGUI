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
protected:
};

#endif // INPUTSALESCREENVIEW_HPP
