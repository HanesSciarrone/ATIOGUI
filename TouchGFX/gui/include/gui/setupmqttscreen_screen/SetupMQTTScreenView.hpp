#ifndef SETUPMQTTSCREENVIEW_HPP
#define SETUPMQTTSCREENVIEW_HPP

#include <gui_generated/setupmqttscreen_screen/SetupMQTTScreenViewBase.hpp>
#include <gui/setupmqttscreen_screen/SetupMQTTScreenPresenter.hpp>

/* Include added for user */
#include "gui/common/CustomKeyboard.hpp"

class SetupMQTTScreenView : public SetupMQTTScreenViewBase
{
public:
    SetupMQTTScreenView();
    virtual ~SetupMQTTScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    /**
     * @brief Declaring callback handler for TextArea
     */
    void callback_event_click_listener(const TextAreaWithOneWildcard& object, const ClickEvent& event);

    /**
     * @brief Declaring callback handler for TextArea
     */
    void callback_publish_topic_event_click_listener(const TextAreaWithOneWildcard& object, const ClickEvent& event);

    /**
     * @brief Declaring callback handler for TextArea
     */
    void callback_suscribe_topic_event_click_listener(const TextAreaWithOneWildcard& object, const ClickEvent& event);

    /// Increase second of keep alive
    void Increase_keep_alive_mqtt();

    /// Decrease second of keep alive
    void Decrease_keep_alive_mqtt();

    /// Set up version of MQTT used
    void set_version_mqtt();

    /// Set QoS of MQTT
    void set_qos_mqtt();

    /// Fill label with different parameters used on MQTT
    void fill_text_area_buffer();

protected:

    static const uint8_t BUFFER_SIZE = 26;

    CustomKeyboard keyboard;


    // Declaring callback type of box and clickEvent
    Callback<SetupMQTTScreenView, const TextAreaWithOneWildcard&, const ClickEvent&> text_area_event_click;
    Callback<SetupMQTTScreenView, const TextAreaWithOneWildcard&, const ClickEvent&> text_area_publish_topic;
    Callback<SetupMQTTScreenView, const TextAreaWithOneWildcard&, const ClickEvent&> text_area_suscribe_topic;


    uint8_t text_area_selected;

    uint32_t keepalive;
    uint8_t client_id[BUFFER_SIZE];
    uint8_t publish_topic[BUFFER_SIZE];
    uint8_t suscribe_topic[BUFFER_SIZE];
    uint8_t version;
    uint8_t qos;
};

#endif // SETUPMQTTSCREENVIEW_HPP
