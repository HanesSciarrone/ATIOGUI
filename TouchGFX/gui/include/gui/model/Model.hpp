#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

#define GUI_MAX_COUNT_LIST	30

class ModelListener;

struct parameters_mqtt_s {
	uint32_t keep_alive;
	uint8_t version;
	uint8_t qos;
	uint8_t client_id[30];
	uint8_t publish_topic[30];
	uint8_t suscribe_topic[30];
};

/// Type of variable that data's network
typedef struct
{
	uint8_t ssid[22];	/*!< Name of network */
	int8_t rssi;		/*!< Signal strength */
}gui_network_list_t;

typedef struct
{
	gui_network_list_t listNetwork[GUI_MAX_COUNT_LIST];
}gui_network_t;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void MsgScanNetwork();
    void MsgConnectNetwork(char *ssid, char *password);

    void sent_credential_to_IoT(uint8_t *buffer, uint16_t length);

    /**
     * @brief Getting liters available of back end.
     *
     * @return Return liters available.
     */
    uint32_t get_liters_fuel_available(void);

    /**
     * @brief Configure MQTT parameters sending parameters of GUI to
     * Wi-Fi task.
     *
     * @param[in]	param	Parameters of MQTT.
     */
    void configure_parameters_mqtt(struct parameters_mqtt_s param);

    /**
     * @brief Activa reader card NFC.
     */
    void active_reader(void);

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
