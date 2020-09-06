#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

/* Include added for user */
#include "ModuleWifi.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    virtual void ResultScandNetwork(WifiMessage_t *networks) {}

    void bind(Model* m)
    {
        model = m;
    }



protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
