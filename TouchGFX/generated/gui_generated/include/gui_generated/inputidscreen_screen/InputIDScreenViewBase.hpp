/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef INPUTIDSCREENVIEWBASE_HPP
#define INPUTIDSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/inputidscreen_screen/InputIDScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <gui/containers/PadNumeric.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/containers/progress_indicators/CircleProgress.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>

class InputIDScreenViewBase : public touchgfx::View<InputIDScreenPresenter>
{
public:
    InputIDScreenViewBase();
    virtual ~InputIDScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void send_credential()
    {
        // Override and implement this function in InputIDScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box Background;
    touchgfx::Box ToolBar;
    touchgfx::ScalableImage logo;
    PadNumeric PadNumericID;
    touchgfx::TextArea titleInputID;
    touchgfx::ButtonWithLabel BtnAccept;
    touchgfx::ButtonWithLabel BtnCancel;
    touchgfx::Box background_progress;
    touchgfx::ModalWindow pop_up;
    touchgfx::TextArea label1_pop_up;
    touchgfx::ButtonWithLabel button_ok_pop_up;
    touchgfx::TextArea label2_pop_up;
    touchgfx::CircleProgress progress_bar;
    touchgfx::PainterRGB888Bitmap progress_barPainter;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<InputIDScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // INPUTIDSCREENVIEWBASE_HPP
