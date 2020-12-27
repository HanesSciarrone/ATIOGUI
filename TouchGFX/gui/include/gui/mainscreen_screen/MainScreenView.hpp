#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

class MainScreenView : public MainScreenViewBase
{
public:
    MainScreenView();
    virtual ~MainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void ShowProgreessBar();
    void show_status_credential(uint8_t result);

protected:

    uint16_t tickCounter;
    int16_t boxProgressMax;
    int16_t boxProgressMin;

    /** Update value of progress bar */
    void update_progress(uint16_t count);

    /** Call function to update progress bar for each tick time */
    virtual void handleTickEvent();
};

#endif // MAINSCREENVIEW_HPP
