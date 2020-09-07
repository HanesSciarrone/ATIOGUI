#ifndef MODEL_HPP
#define MODEL_HPP

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

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
