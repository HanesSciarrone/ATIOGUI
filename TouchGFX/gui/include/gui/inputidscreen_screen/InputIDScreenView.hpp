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

    /** Function when you press button accept */
    virtual void send_credential();

    /** Function that show result of sent credential to broker */
    virtual void show_status_credential(uint8_t result);

    /** Shows progress bar */
    virtual void ShowProgreessBar();

private:

    uint16_t tickCounter;
    int16_t boxProgressMax;
    int16_t boxProgressMin;

    /** Update value of progress bar */
    void update_progress(uint16_t count);

    /** Call function to update progress bar for each tick time */
    virtual void handleTickEvent();

protected:

};


#endif // INPUTIDSCREENVIEW_HPP
