#ifndef INPUTIDSCREENPRESENTER_HPP
#define INPUTIDSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class InputIDScreenView;

class InputIDScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    InputIDScreenPresenter(InputIDScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~InputIDScreenPresenter() {};

private:
    InputIDScreenPresenter();

    InputIDScreenView& view;
};

#endif // INPUTIDSCREENPRESENTER_HPP
