#ifndef OPERATIONPUMPSCREENPRESENTER_HPP
#define OPERATIONPUMPSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class OperationPumpScreenView;

class OperationPumpScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    OperationPumpScreenPresenter(OperationPumpScreenView& v);

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

    virtual ~OperationPumpScreenPresenter() {};

    uint8_t *get_liters_to_dispahe();

    /**
     * @brief Sent command to dispache fuel.
     *
     * @param[in]	pump Pump number used.
     * @param[in]	type Type fuel used.
     */
    void dispatch_fuel_action(uint8_t *pump, uint8_t *type);

    /**
     * @brief Sent command to stop dispatch.
     *
     * @param[in]	pump Pump number to stop.
     */
    void stop_dispatch_action(uint8_t *pump);

private:
    OperationPumpScreenPresenter();

    OperationPumpScreenView& view;
};

#endif // OPERATIONPUMPSCREENPRESENTER_HPP
