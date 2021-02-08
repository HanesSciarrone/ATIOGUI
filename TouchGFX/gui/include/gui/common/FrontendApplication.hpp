#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

    void gotoInputSaleScreenScreenCoverTransitionSouth();
    void gotoInputSaleScreenScreenCoverTransitionSouthImpl();
    void gotoOperationPumpScreenCoverTransitionSouth();

private:

    touchgfx::Callback<FrontendApplication> transitionCallback_custom;
    FrontendHeap& frontendHeap_custom;
    Model& model_custom;

    void gotoOperationPumpScreenCoverTransitionSouthImpl();
};

#endif // FRONTENDAPPLICATION_HPP
