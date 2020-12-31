#include <gui/setupmqttscreen_screen/SetupMQTTScreenView.hpp>

#include <string.h>

SetupMQTTScreenView::SetupMQTTScreenView()
: keyboard(),
  text_area_event_click(this, &SetupMQTTScreenView::callback_event_click_listener)
{
	keyboard.setPosition(90, 15, 320, 240);
	Pop_up.add(keyboard);
}

void SetupMQTTScreenView::setupScreen()
{
    SetupMQTTScreenViewBase::setupScreen();

    text_area_selected = 0;

	// Setup action of click listener
    Client_id.setClickAction(text_area_event_click);
    Publish_topic.setClickAction(text_area_event_click);
    Suscribe_topic.setClickAction(text_area_event_click);
}

void SetupMQTTScreenView::tearDownScreen()
{
    SetupMQTTScreenViewBase::tearDownScreen();
}

void SetupMQTTScreenView::callback_event_click_listener(const TextAreaWithOneWildcard& object, const ClickEvent& event)
{
	if (&object == &Client_id) {
		text_area_selected = 0;
		Pop_up.setVisible(true);
		Pop_up.invalidate();
		keyboard.clear_buffer();
	}

	if (&object == &Publish_topic) {
		text_area_selected = 1;
		Pop_up.setVisible(true);
		Pop_up.invalidate();
		keyboard.clear_buffer();
	}

	if (&object == &Suscribe_topic) {
		text_area_selected = 2;
		Pop_up.setVisible(true);
		Pop_up.invalidate();
		keyboard.clear_buffer();
	}
}

void SetupMQTTScreenView::set_version_mqtt()
{
	if (BtnVersionMQTT.getState()) {
		version = 4;
	}
	else {
		version = 3;
	}
}

void SetupMQTTScreenView::Increase_keep_alive_mqtt()
{
	this->keepalive = Unicode::atoi(Keep_aliveBuffer);

	if (this->keepalive >= 99999999) {
		this->keepalive = 0;
	}

	this->keepalive++;
	Unicode::snprintf(Keep_aliveBuffer, KEEP_ALIVE_SIZE, "%d", this->keepalive);
	Keep_alive.invalidate();
}

void SetupMQTTScreenView::Decrease_keep_alive_mqtt()
{
	this->keepalive = Unicode::atoi(Keep_aliveBuffer);

	if (this->keepalive > 0) {
		this->keepalive--;
	}
	Unicode::snprintf(Keep_aliveBuffer, KEEP_ALIVE_SIZE, "%d", this->keepalive);
	Keep_alive.invalidate();
}

void SetupMQTTScreenView::set_qos_mqtt()
{
	if (BtnQoS_0.getSelected()) {
		qos = 0;
	}
	else if (BtnQoS_1.getSelected()) {
		qos = 1;
	}
	else {
		qos = 2;
	}
}

void SetupMQTTScreenView::fill_text_area_buffer()
{
	uint8_t buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);
	Pop_up.setVisible(false);
	keyboard.GetBuffer(buffer);

	switch(text_area_selected){
	case 0: {

		Unicode::fromUTF8(buffer, Client_idBuffer, BUFFER_SIZE-1);
		Client_id.invalidate();
		Pop_up.invalidate();
		strncpy((char *)client_id, (char *)buffer, strlen((char *)buffer));
	}
	break;

	case 1: {
		Unicode::fromUTF8(buffer, Publish_topicBuffer, BUFFER_SIZE-1);
		Publish_topic.invalidate();
		Pop_up.invalidate();
		strncpy((char *)publish_topic, (char *)buffer, strlen((char *)buffer));
	}
	break;

	default:
		Unicode::fromUTF8(buffer, Suscribe_topicBuffer, BUFFER_SIZE-1);
		Suscribe_topic.invalidate();
		Pop_up.invalidate();
		strncpy((char *)suscribe_topic, (char *)buffer, strlen((char *)buffer));

	}
}

void SetupMQTTScreenView::configure_parameters_mqtt()
{
	struct parameters_mqtt_s param_mqtt;

	memset(&param_mqtt, 0, sizeof(struct parameters_mqtt_s));
	param_mqtt.keep_alive = keepalive;
	param_mqtt.version = version;
	param_mqtt.qos = qos;
	strncpy((char *)param_mqtt.client_id, (char *)client_id, strlen((char *)client_id));
	strncpy((char *)param_mqtt.publish_topic, (char *)publish_topic, strlen((char *)publish_topic));
	strncpy((char *)param_mqtt.suscribe_topic, (char *)suscribe_topic, strlen((char *)suscribe_topic));

	presenter->configure_parameters_mqtt(param_mqtt);
}
