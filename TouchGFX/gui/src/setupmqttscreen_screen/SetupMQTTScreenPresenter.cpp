#include <gui/setupmqttscreen_screen/SetupMQTTScreenView.hpp>
#include <gui/setupmqttscreen_screen/SetupMQTTScreenPresenter.hpp>

SetupMQTTScreenPresenter::SetupMQTTScreenPresenter(SetupMQTTScreenView& v)
    : view(v)
{

}

void SetupMQTTScreenPresenter::activate()
{

}

void SetupMQTTScreenPresenter::deactivate()
{

}

void SetupMQTTScreenPresenter::configure_parameters_mqtt(struct parameters_mqtt_s param)
{
	model->configure_parameters_mqtt(param);
}
