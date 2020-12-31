#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    virtual void ResultScandNetwork(gui_network_t *networks) {}

    virtual void ShowStatusConnectionNetwork(uint8_t value) {}

    virtual void show_status_credential(uint8_t result) {}

    virtual void getting_data_read_card(uint8_t *id, uint8_t length_id) {}

    virtual void ShowProgreessBar() {}

    void bind(Model* m)
    {
        model = m;
    }



protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
