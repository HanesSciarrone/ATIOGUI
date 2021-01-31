#ifndef OPERATIONPUMPSCREENVIEW_HPP
#define OPERATIONPUMPSCREENVIEW_HPP

#include <gui_generated/operationpumpscreen_screen/OperationPumpScreenViewBase.hpp>
#include <gui/operationpumpscreen_screen/OperationPumpScreenPresenter.hpp>

class OperationPumpScreenView : public OperationPumpScreenViewBase
{
public:
    OperationPumpScreenView();
    virtual ~OperationPumpScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void pay_sale_action();
    void cancel_sale_action();
    void dispatch_fuel_action();
    void stop_dispache_action();
    void set_pump_selected(int value);
    void select_types_fuel();

protected:

    uint8_t type_fuel[20];		// Type of fuel selected
    float dispache_liters;		// Liters selected to dispatch
    float liters_dispensed;		// Liters already dispensed.
    bool selector_enable;		// Selector of fuel is enable
};

#endif // OPERATIONPUMPSCREENVIEW_HPP
