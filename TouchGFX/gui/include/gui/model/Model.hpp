#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

class ModelListener;

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

    uint32_t get_liters_fuel_available(void);

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
