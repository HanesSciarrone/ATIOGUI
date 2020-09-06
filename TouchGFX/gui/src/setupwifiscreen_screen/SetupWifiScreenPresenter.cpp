#include <gui/setupwifiscreen_screen/SetupWifiScreenView.hpp>
#include <gui/setupwifiscreen_screen/SetupWifiScreenPresenter.hpp>

SetupWifiScreenPresenter::SetupWifiScreenPresenter(SetupWifiScreenView& v)
    : view(v)
{

}

void SetupWifiScreenPresenter::activate()
{

}

void SetupWifiScreenPresenter::deactivate()
{

}

void SetupWifiScreenPresenter::MsgScanNetwork()
{
	model->MsgScanNetwork();
}

void SetupWifiScreenPresenter::ResultScandNetwork(WifiMessage_t *networks)
{
	view.FillOptionNetwork(networks);
}
