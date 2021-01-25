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
    void dispache_fuel_action();
    void stop_dispache_action();

protected:

    float dispache_liters;
    float liters_dispensed;
    bool pump_dispaching;
};

#endif // OPERATIONPUMPSCREENVIEW_HPP
