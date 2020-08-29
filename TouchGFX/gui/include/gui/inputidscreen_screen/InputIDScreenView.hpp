#ifndef INPUTIDSCREENVIEW_HPP
#define INPUTIDSCREENVIEW_HPP

#include <gui_generated/inputidscreen_screen/InputIDScreenViewBase.hpp>
#include <gui/inputidscreen_screen/InputIDScreenPresenter.hpp>

class InputIDScreenView : public InputIDScreenViewBase
{
public:
    InputIDScreenView();
    virtual ~InputIDScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();



protected:
};

#endif // INPUTIDSCREENVIEW_HPP
