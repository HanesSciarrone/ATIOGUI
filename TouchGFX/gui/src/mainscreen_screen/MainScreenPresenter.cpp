#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

MainScreenPresenter::MainScreenPresenter(MainScreenView& v)
    : view(v)
{

}

void MainScreenPresenter::activate()
{

}

void MainScreenPresenter::deactivate()
{

}

void MainScreenPresenter::ShowProgreessBar()
{
	view.ShowProgreessBar();
}

void MainScreenPresenter::show_status_credential(uint8_t result)
{
	view.show_status_credential(result);
}
