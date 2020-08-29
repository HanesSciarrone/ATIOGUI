/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef PADNUMERICBASE_HPP
#define PADNUMERICBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class PadNumericBase : public touchgfx::Container
{
public:
    PadNumericBase();
    virtual ~PadNumericBase() {}
    virtual void initialize();

    /*
     * Virtual Action Handlers
     */
    virtual void Btn1Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn2Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn3Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn4Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn5Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn6Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn7Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn8Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn9Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void Btn0Click()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void BtnDecimalClick()
    {
        // Override and implement this function in PadNumeric
    }

    virtual void BtnDeleteClick()
    {
        // Override and implement this function in PadNumeric
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Container ContainerKeyboard;
    touchgfx::Box background;
    touchgfx::ButtonWithLabel button_N1;
    touchgfx::ButtonWithLabel button_N2;
    touchgfx::ButtonWithLabel button_N3;
    touchgfx::ButtonWithLabel button_N4;
    touchgfx::ButtonWithLabel button_N5;
    touchgfx::ButtonWithLabel button_N6;
    touchgfx::ButtonWithLabel button_N7;
    touchgfx::ButtonWithLabel button_N8;
    touchgfx::ButtonWithLabel button_N9;
    touchgfx::ButtonWithLabel button_N0;
    touchgfx::ButtonWithLabel button_decimal;
    touchgfx::ButtonWithIcon button_delete;
    touchgfx::BoxWithBorder bkgTextID;
    touchgfx::TextAreaWithOneWildcard textArea;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREA_SIZE = 20;
    touchgfx::Unicode::UnicodeChar textAreaBuffer[TEXTAREA_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<PadNumericBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // PADNUMERICBASE_HPP
