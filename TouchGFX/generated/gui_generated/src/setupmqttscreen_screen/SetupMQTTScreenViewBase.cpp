/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/setupmqttscreen_screen/SetupMQTTScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

SetupMQTTScreenViewBase::SetupMQTTScreenViewBase() :
    buttonCallback(this, &SetupMQTTScreenViewBase::buttonCallbackHandler),
    radioButtonSelectedCallback(this, &SetupMQTTScreenViewBase::radioButtonSelectedCallbackHandler)
{

    Background.setPosition(0, 0, 800, 480);
    Background.setColor(touchgfx::Color::getColorFrom24BitRGB(41, 73, 105));

    ToolBar.setPosition(0, 0, 800, 62);
    ToolBar.setColor(touchgfx::Color::getColorFrom24BitRGB(145, 145, 145));

    logo.setBitmap(touchgfx::Bitmap(BITMAP_CONTROLGAS_ID));
    logo.setPosition(0, 1, 153, 60);
    logo.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    BtnVersionMQTT.setXY(98, 148);
    BtnVersionMQTT.setBitmaps(touchgfx::Bitmap(BITMAP_SWITCH_OFF_ID), touchgfx::Bitmap(BITMAP_SWITCH_ON_ID));
    BtnVersionMQTT.setAction(buttonCallback);

    label_version_mqtt.setXY(42, 111);
    label_version_mqtt.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    label_version_mqtt.setLinespacing(0);
    label_version_mqtt.setTypedText(touchgfx::TypedText(T_SINGLEUSEID42));

    BtnQoS_0.setXY(147, 211);
    BtnQoS_0.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID));
    BtnQoS_0.setSelected(true);
    BtnQoS_0.setDeselectionEnabled(false);

    BtnQoS_1.setXY(147, 285);
    BtnQoS_1.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID));
    BtnQoS_1.setSelected(false);
    BtnQoS_1.setDeselectionEnabled(true);

    BtnQoS_2.setXY(147, 359);
    BtnQoS_2.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_INACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_ACTIVE_ID), touchgfx::Bitmap(BITMAP_BLUE_CHECK_BUTTONS_CHECK_ROUND_MARK_NORMAL_ID));
    BtnQoS_2.setSelected(false);
    BtnQoS_2.setDeselectionEnabled(true);

    TxtQoS_0.setXY(28, 221);
    TxtQoS_0.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    TxtQoS_0.setLinespacing(0);
    TxtQoS_0.setTypedText(touchgfx::TypedText(T_SINGLEUSEID43));

    TxtQoS_1.setXY(28, 295);
    TxtQoS_1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    TxtQoS_1.setLinespacing(0);
    TxtQoS_1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID44));

    TxtQoS_2.setXY(28, 369);
    TxtQoS_2.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    TxtQoS_2.setLinespacing(0);
    TxtQoS_2.setTypedText(touchgfx::TypedText(T_SINGLEUSEID45));

    Title_Client_id.setXY(329, 112);
    Title_Client_id.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Title_Client_id.setLinespacing(0);
    Title_Client_id.setTypedText(touchgfx::TypedText(T_SINGLEUSEID48));

    Background_Client_id.setPosition(422, 110, 351, 28);
    Background_Client_id.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Background_Client_id.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Background_Client_id.setBorderSize(3);

    Title_publish_topic.setXY(293, 179);
    Title_publish_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Title_publish_topic.setLinespacing(0);
    Title_publish_topic.setTypedText(touchgfx::TypedText(T_SINGLEUSEID50));

    Background_publish_topic.setPosition(422, 177, 351, 28);
    Background_publish_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Background_publish_topic.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Background_publish_topic.setBorderSize(3);

    Client_id.setPosition(426, 114, 343, 20);
    Client_id.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Client_id.setLinespacing(0);
    Client_idBuffer[0] = 0;
    Client_id.setWildcard(Client_idBuffer);
    Client_id.setTypedText(touchgfx::TypedText(T_SINGLEUSEID49));

    Title_suscribe_topic.setXY(293, 246);
    Title_suscribe_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Title_suscribe_topic.setLinespacing(0);
    Title_suscribe_topic.setTypedText(touchgfx::TypedText(T_SINGLEUSEID52));

    Background_suscribe_topic.setPosition(422, 244, 351, 28);
    Background_suscribe_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Background_suscribe_topic.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Background_suscribe_topic.setBorderSize(3);

    Publish_topic.setPosition(426, 181, 343, 20);
    Publish_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Publish_topic.setLinespacing(0);
    Publish_topicBuffer[0] = 0;
    Publish_topic.setWildcard(Publish_topicBuffer);
    Publish_topic.setTypedText(touchgfx::TypedText(T_SINGLEUSEID51));

    Btn_OK.setXY(739, 1);
    Btn_OK.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    Btn_OK.setLabelText(touchgfx::TypedText(T_SINGLEUSEID54));
    Btn_OK.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Btn_OK.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Btn_OK.setAction(buttonCallback);

    Backgrount_keepalive.setXY(578, 304);
    Backgrount_keepalive.setBitmap(touchgfx::Bitmap(BITMAP_COUNTER_BOX_ID));

    Suscribe_topic.setPosition(426, 248, 343, 20);
    Suscribe_topic.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Suscribe_topic.setLinespacing(0);
    Suscribe_topicBuffer[0] = 0;
    Suscribe_topic.setWildcard(Suscribe_topicBuffer);
    Suscribe_topic.setTypedText(touchgfx::TypedText(T_SINGLEUSEID53));

    Keep_alive.setPosition(584, 361, 140, 40);
    Keep_alive.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    Keep_alive.setLinespacing(7);
    Unicode::snprintf(Keep_aliveBuffer, KEEP_ALIVE_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID61).getText());
    Keep_alive.setWildcard(Keep_aliveBuffer);
    Keep_alive.setTypedText(touchgfx::TypedText(T_SINGLEUSEID55));

    textArea1.setXY(369, 356);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID56));

    Btn_Increase_keepalive.setXY(501, 309);
    Btn_Increase_keepalive.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID));
    Btn_Increase_keepalive.setIconXY(7, 17);
    Btn_Increase_keepalive.setAction(buttonCallback);

    Pop_up.setBackground(touchgfx::BitmapId(BITMAP_BACKGROUND_QWERTY_ID), 150, 105);
    Pop_up.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    Pop_up.setShadeAlpha(200);
    Pop_up.hide();

    btn_pop_up_ok.setXY(430, 206);
    btn_pop_up_ok.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    btn_pop_up_ok.setLabelText(touchgfx::TypedText(T_SINGLEUSEID59));
    btn_pop_up_ok.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pop_up_ok.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pop_up_ok.setAction(buttonCallback);
    Pop_up.add(btn_pop_up_ok);

    btn_pop_up_cancel.setXY(11, 206);
    btn_pop_up_cancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    btn_pop_up_cancel.setLabelText(touchgfx::TypedText(T_SINGLEUSEID60));
    btn_pop_up_cancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pop_up_cancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    btn_pop_up_cancel.setAction(buttonCallback);
    Pop_up.add(btn_pop_up_cancel);

    Btn_Decrease_keepalive.setXY(501, 394);
    Btn_Decrease_keepalive.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID));
    Btn_Decrease_keepalive.setIconXY(7, 17);
    Btn_Decrease_keepalive.setAction(buttonCallback);

    add(Background);
    add(ToolBar);
    add(logo);
    add(BtnVersionMQTT);
    add(label_version_mqtt);
    add(BtnQoS_0);
    add(BtnQoS_1);
    add(BtnQoS_2);
    add(TxtQoS_0);
    add(TxtQoS_1);
    add(TxtQoS_2);
    add(Title_Client_id);
    add(Background_Client_id);
    add(Title_publish_topic);
    add(Background_publish_topic);
    add(Client_id);
    add(Title_suscribe_topic);
    add(Background_suscribe_topic);
    add(Publish_topic);
    add(Btn_OK);
    add(Backgrount_keepalive);
    add(Suscribe_topic);
    add(Keep_alive);
    add(textArea1);
    add(Btn_Increase_keepalive);
    add(Pop_up);
    add(Btn_Decrease_keepalive);
    radioButtonGroup1.add(BtnQoS_0);
    radioButtonGroup1.add(BtnQoS_1);
    radioButtonGroup1.add(BtnQoS_2);
    radioButtonGroup1.setRadioButtonSelectedHandler(radioButtonSelectedCallback);
}

void SetupMQTTScreenViewBase::setupScreen()
{

}

void SetupMQTTScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &BtnVersionMQTT)
    {
        //Version_MQTT
        //When BtnVersionMQTT clicked call virtual function
        //Call set_version_mqtt
        set_version_mqtt();
    }
    else if (&src == &Btn_OK)
    {
        //Configure_MQTT
        //When Btn_OK clicked change screen to MainScreen
        //Go to MainScreen with screen transition towards West
        application().gotoMainScreenScreenCoverTransitionWest();

        //Parameters_MQTT
        //When Configure_MQTT completed call virtual function
        //Call configure_parameters_mqtt
        configure_parameters_mqtt();
    }
    else if (&src == &Btn_Increase_keepalive)
    {
        //Increase_second
        //When Btn_Increase_keepalive clicked call virtual function
        //Call Increase_keep_alive_mqtt
        Increase_keep_alive_mqtt();
    }
    else if (&src == &btn_pop_up_ok)
    {
        //Fill_text
        //When btn_pop_up_ok clicked call virtual function
        //Call fill_text_area_buffer
        fill_text_area_buffer();
    }
    else if (&src == &btn_pop_up_cancel)
    {
        //Hide_qwerty
        //When btn_pop_up_cancel clicked hide Pop_up
        //Hide Pop_up
        Pop_up.setVisible(false);
        Pop_up.invalidate();
    }
    else if (&src == &Btn_Decrease_keepalive)
    {
        //Decrease_second
        //When Btn_Decrease_keepalive clicked call virtual function
        //Call Decrease_keep_alive_mqtt
        Decrease_keep_alive_mqtt();
    }
}

void SetupMQTTScreenViewBase::radioButtonSelectedCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &BtnQoS_0)
    {
        //Set_QoS_0
        //When BtnQoS_0 selected call virtual function
        //Call set_qos_mqtt
        set_qos_mqtt();
    }
    else if (&src == &BtnQoS_1)
    {
        //Set_QoS_1
        //When BtnQoS_1 selected call virtual function
        //Call set_qos_mqtt
        set_qos_mqtt();
    }
    else if (&src == &BtnQoS_2)
    {
        //Set_QoS_2
        //When BtnQoS_2 selected call virtual function
        //Call set_qos_mqtt
        set_qos_mqtt();
    }
}
