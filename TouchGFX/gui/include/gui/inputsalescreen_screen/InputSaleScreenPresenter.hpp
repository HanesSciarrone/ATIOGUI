#ifndef INPUTSALESCREENPRESENTER_HPP
#define INPUTSALESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class InputSaleScreenView;

class InputSaleScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    InputSaleScreenPresenter(InputSaleScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~InputSaleScreenPresenter() {};
    virtual float get_liters_fuel_available(void);
    virtual void set_liters_to_dispache(uint8_t *liters_selected);

private:
    InputSaleScreenPresenter();

    InputSaleScreenView& view;
};

#endif // INPUTSALESCREENPRESENTER_HPP
