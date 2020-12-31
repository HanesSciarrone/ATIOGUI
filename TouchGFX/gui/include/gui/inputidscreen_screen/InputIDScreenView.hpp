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

    /** Send credential getted with NFC module */
    void send_card_id();

    /** Active card reader */
    void active_reader();

    /** Select mode manual or card to identify user */
    void select_mode_id();

    /** Function that show result of sent credential to broker */
    virtual void show_status_credential(uint8_t result);

    /** Shows progress bar */
    virtual void ShowProgreessBar();

    /**
     * @brief Show you data got of card.
     *
     * @param[in]	id			Buffer when data on string format.
     * @param[in]	length_id	Length of data got.
     */
    void getting_data_read_card(uint8_t *id, uint8_t length_id);

private:

    uint16_t tickCounter;
    int16_t boxProgressMax;
    int16_t boxProgressMin;
    uint16_t timeout_counter;
    bool card_manual;


    /** Update value of progress bar */
    void update_progress(uint16_t count);

    /** Update clock to reduce timeout */
    void update_clock(uint16_t timeout_counter);

    /** Call function to update progress bar for each tick time */
    virtual void handleTickEvent();

protected:

};


#endif // INPUTIDSCREENVIEW_HPP
