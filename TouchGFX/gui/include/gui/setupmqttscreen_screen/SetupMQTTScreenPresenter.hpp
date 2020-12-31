#ifndef SETUPMQTTSCREENPRESENTER_HPP
#define SETUPMQTTSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SetupMQTTScreenView;

class SetupMQTTScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SetupMQTTScreenPresenter(SetupMQTTScreenView& v);

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

    virtual ~SetupMQTTScreenPresenter() {};

    void configure_parameters_mqtt(struct parameters_mqtt_s param);

private:
    SetupMQTTScreenPresenter();

    SetupMQTTScreenView& view;
};

#endif // SETUPMQTTSCREENPRESENTER_HPP
