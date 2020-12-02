#ifndef PADNUMERIC_HPP
#define PADNUMERIC_HPP

#include <gui_generated/containers/PadNumericBase.hpp>

class PadNumeric : public PadNumericBase
{
public:
    PadNumeric();
    virtual ~PadNumeric() {}

    virtual void initialize();


    virtual void Btn1Click();

    virtual void Btn2Click();

    virtual void Btn3Click();

    virtual void Btn4Click();

    virtual void Btn5Click();

    virtual void Btn6Click();

    virtual void Btn7Click();

    virtual void Btn8Click();

    virtual void Btn9Click();

    virtual void Btn0Click();

    virtual void BtnDecimalClick();

    virtual void BtnDeleteClick();

    int get_buffer_length();
    void get_buffer(uint8_t *buffer);

protected:

     uint8_t position;
};

#endif // PADNUMERIC_HPP
