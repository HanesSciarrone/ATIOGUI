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

void SetupWifiScreenPresenter::MsgConnectNetwork(uint8_t *ssid, uint8_t *password)
{
	model->MsgConnectNetwork((char *)ssid, (char *)password);
}


void SetupWifiScreenPresenter::ResultScandNetwork(gui_network_t *networks)
{
	view.FillOptionNetwork(networks);
}

void SetupWifiScreenPresenter::ShowStatusConnectionNetwork(uint8_t value)
{
	view.ShowStatusConnection(value);
}

void SetupWifiScreenPresenter::hide_progreess_bar()
{
	view.hide_progreess_bar();
}
