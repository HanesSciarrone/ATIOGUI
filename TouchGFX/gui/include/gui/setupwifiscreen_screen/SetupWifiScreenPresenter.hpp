#ifndef SETUPWIFISCREENPRESENTER_HPP
#define SETUPWIFISCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SetupWifiScreenView;

class SetupWifiScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SetupWifiScreenPresenter(SetupWifiScreenView& v);

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

    virtual ~SetupWifiScreenPresenter() {};

private:
    SetupWifiScreenPresenter();

    SetupWifiScreenView& view;
};

#endif // SETUPWIFISCREENPRESENTER_HPP