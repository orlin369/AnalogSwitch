
// TODO: Add OTA.
// TODO: Add settings in FS.
// TODO: Add WEB server for configuration.
// TODO: Add WEB API for remote controlling.
// TODO: Add IR remote controll.
// TODO: Add IR training button for the separate commands.

#pragma region Headers

#include "ApplicationConfiguration.h"

#include "DefaultCredentials.h"

#include "VolumeAction.h"

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#include "pt2257.h"

#include <Button.h>

#include <WiFi.h>

#include <ESPmDNS.h>

#include <WiFiUdp.h>

#include <ArduinoOTA.h>

#pragma endregion

#pragma region Variables

/**
 * @brief Used to set the LED.
 * 
 */
int StatusLEDState_g;

/**
 * @brief Will store last time LED was updated.
 * 
 */
unsigned long PreviousTime_g;

/**
 * @brief Will store current time LED was updated.
 * 
 */
unsigned long CurrentTime_g;

/**
 * @brief Output chanel.
 * 
 */
uint8_t OutputChannel_g;

/**
 * @brief Last state of the output channel.
 * 
 */
uint8_t LastOutputChannel_g;

/**
 * @brief Button for input 1.
 * 
 * @return Button 
 */
Button BtnIn1_g(PIN_BTN_IN1);

/**
 * @brief Button for input 2.
 * 
 * @return Button 
 */
Button BtnIn2_g(PIN_BTN_IN2);

/**
 * @brief Button for input 3.
 * 
 * @return Button 
 */
Button BtnIn3_g(PIN_BTN_IN3);

/**
 * @brief Button for input 4.
 * 
 * @return Button 
 */
Button BtnIn4_g(PIN_BTN_IN4);

/**
 * @brief Button for learning IR codes.
 * 
 * @return Button 
 */
Button BtnLearn_g(PIN_BTN_LEARN);

/**
 * @brief Button for increesing the volume.
 * 
 * @return Button 
 */
Button BtnVolDn_g(PIN_BTN_VOL_DN);

/**
 * @brief Button for decreesing the volume.
 * 
 * @return Button 
 */
Button BtnVolUp_g(PIN_BTN_VOL_UP);

/**
 * @brief Digital potentiometer PT2257.
 * 
 */
PT2257 Pot_g;

/**
 * @brief Volume action.
 * 
 */
VolumeAction_t VolAct_g;

/**
 * @brief Last volume action.
 * 
 */
VolumeAction_t LastVolAct_g;

/**
 * @brief Volume value.
 * 
 */
uint8_t VolumeValue_g;

/**
 * @brief OLED SSD1306.
 * 
 * @return Adafruit_SSD1306 
 */
Adafruit_SSD1306 Display_g(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
 * @brief Display flag.
 * 
 */
bool DisplayFlag_g;

// String TextChannel_g;

/**
 * @brief Volume text in percentage.
 * 
 */
String TextVolume_g;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Setup the variables objects.
 * 
 */
void setup_variables();

/**
 * @brief Set the GPIOs.
 * 
 */
void setup_io();

/**
 * @brief Setup the potentiometer.
 * 
 */
void setup_pot();

/**
 * @brief Setup the display.
 * 
 */
void setup_display();

/**
 * @brief Update the button status.
 * 
 */
void update_buttons();

/**
 * @brief Check for chanel changes.
 * 
 */
void update_channel();

/**
 * @brief Set the channel.
 * 
 * @param channel Chanel index.
 */
void set_channel(uint8_t channel);

/**
 * @brief Check for volume changes.
 * 
 */
void update_volume();

/**
 * @brief Set the volume.
 * 
 * @param volume Volume index.
 */
void set_volume(uint8_t  volume);

/**
 * @brief Update the status LED.
 * 
 */
void update_status_led();

/**
 * @brief Update display.
 * 
 */
void update_display();

#pragma endregion

void setup()
{
    setup_variables();

    setup_io();
    
    setup_pot();

    setup_display();
}

void loop()
{
    update_buttons();

    update_channel();

    update_volume();

    update_status_led();

    update_display();
}

#pragma region Functions

/**
 * @brief Setup the variables objects.
 * 
 */
void setup_variables()
{
    StatusLEDState_g = LOW;
    PreviousTime_g = 0;
    CurrentTime_g = 0;
    OutputChannel_g = 0;
    LastOutputChannel_g = 0;
    LastVolAct_g = VolumeAction_t::Nothing;
    VolAct_g = VolumeAction_t::Nothing;
    VolumeValue_g = 50;
    DisplayFlag_g = false;
    // TextChannel_g = "";
    TextVolume_g = "";
}

/**
 * @brief Set the GPIOs.
 * 
 */
void setup_io()
{
    pinMode(PIN_REL_IN1, OUTPUT);
    pinMode(PIN_REL_IN2, OUTPUT);
    pinMode(PIN_REL_IN3, OUTPUT);
    pinMode(PIN_REL_IN4, OUTPUT);

    digitalWrite(PIN_REL_IN1, LOW);
    digitalWrite(PIN_REL_IN2, LOW);
    digitalWrite(PIN_REL_IN3, LOW);
    digitalWrite(PIN_REL_IN4, LOW);

    pinMode(PIN_LED_STATUS, OUTPUT);

    BtnIn1_g.begin();
    BtnIn2_g.begin();
    BtnIn3_g.begin();
    BtnIn4_g.begin();
    BtnLearn_g.begin();
    BtnVolDn_g.begin();
    BtnVolUp_g.begin();

    Serial.begin(9600);
    Serial.println("\n\nAnalog switch.");
}

/**
 * @brief Setup the potentiometer.
 * 
 */
void setup_pot()
{
    Pot_g.init();
    set_volume(VolumeValue_g);
}

/**
 * @brief Setup the display.
 * 
 */
void setup_display()
{ 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!Display_g.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  Display_g.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  Display_g.clearDisplay();
}

/**
 * @brief Update the button status.
 * 
 */
void update_buttons()
{
    if (BtnIn1_g.pressed())
    {
        // Set the chanel.
        OutputChannel_g = 1;
    }
    else if (BtnIn2_g.pressed())
    {
        // Set the chanel.
        OutputChannel_g = 2;
    }
    else if (BtnIn3_g.pressed())
    {
        // Set the chanel.
        OutputChannel_g = 3;
    }
    else if (BtnIn4_g.pressed())
    {
        // Set the chanel.
        OutputChannel_g = 4;
    }
    else if (BtnLearn_g.pressed())
    {
        Serial.println("Learn clicked");
    }
    else if (BtnVolDn_g.pressed())
    {
        VolAct_g = VolumeAction_t::Down;
    }
    else if (BtnVolUp_g.pressed())
    {
        VolAct_g = VolumeAction_t::Up;
    }

    if (BtnVolDn_g.toggled())
    {
        if (BtnVolDn_g.read() == Button::PRESSED)
        {
            VolAct_g = VolumeAction_t::Down;
        }
    }

    if (BtnVolUp_g.toggled())
    {
        if (BtnVolUp_g.read() == Button::PRESSED)
        {
            VolAct_g = VolumeAction_t::Down;
        }
    }

    // Display update action.
    DisplayFlag_g = 
        (VolAct_g != VolumeAction_t::Nothing) ||
        (OutputChannel_g != LastOutputChannel_g);
}

/**
 * @brief Check for chanel changes.
 * 
 */
void update_channel()
{
    if (OutputChannel_g != LastOutputChannel_g)
    {
        set_channel(OutputChannel_g);
        LastOutputChannel_g = OutputChannel_g;
    }
}

/**
 * @brief Set the channel.
 * 
 * @param channel Chanel index.
 */
void set_channel(uint8_t channel)
{
    // 1. Set the volume to 0.
    set_volume(MIN_VOL_VAL);

    // 2. Turn all relays to 0.
    digitalWrite(PIN_REL_IN1, LOW);
    digitalWrite(PIN_REL_IN2, LOW);
    digitalWrite(PIN_REL_IN3, LOW);
    digitalWrite(PIN_REL_IN4, LOW);

    // 3. Set relay chanel.
    if (channel == 1)
    {
        digitalWrite(PIN_REL_IN1, HIGH);
    }
    else if (channel == 2)
    {
        digitalWrite(PIN_REL_IN2, HIGH);
    }
    else if (channel == 3)
    {
        digitalWrite(PIN_REL_IN3, HIGH);
    }
    else if (channel == 4)
    {
        digitalWrite(PIN_REL_IN4, HIGH);
    }

    // 4. Put the volume back.
    set_volume(VolumeValue_g);
}

/**
 * @brief Check for volume changes.
 * 
 */
void update_volume()
{
    if (VolAct_g != LastVolAct_g)
    {
        // Increse volume.
        if (VolAct_g == VolumeAction_t::Up)
        {
            VolumeValue_g++;
            if (VolumeValue_g > MAX_VOL_VAL)
            {
                VolumeValue_g = MAX_VOL_VAL;
            }
        }

        // Decrece volume.
        else if (VolAct_g == VolumeAction_t::Down)
        {
            VolumeValue_g--;
            if (VolumeValue_g < MIN_VOL_VAL)
            {
                VolumeValue_g = MIN_VOL_VAL;
            }
        }

        set_volume(VolumeValue_g);

        LastVolAct_g = VolAct_g;
        VolAct_g = VolumeAction_t::Nothing;
    }
}

/**
 * @brief Set the volume.
 * 
 * @param volume Volume index.
 */
void set_volume(uint8_t  volume)
{
    // Scale the pot value.
    uint8_t PotValL = (uint8_t)map(volume, MIN_VOL_VAL, MAX_VOL_VAL, MIN_POT_VAL, MAX_POT_VAL);

    // Set the volume.
    Pot_g.set_volume(PotValL);
}

/**
 * @brief Update the status LED.
 * 
 */
void update_status_led()
{
    CurrentTime_g = millis();
    
    if (CurrentTime_g - PreviousTime_g >= BLINK_INTERVAL)
    {
        // Save the last time you blinked the LED.
        PreviousTime_g = CurrentTime_g;
        
        // If the LED is off turn it on and vice-versa:
        if (StatusLEDState_g == LOW)
        {
            StatusLEDState_g = HIGH;
        }
        else
        {
            StatusLEDState_g = LOW;
        }
        
        // Set the LED with the StatusLEDState_g of the variable:
        digitalWrite(PIN_LED_STATUS, StatusLEDState_g);
  }
}

/**
 * @brief Update display.
 * 
 */
void update_display()
{
    if(DisplayFlag_g == false)
    {
        return;
    }
    DisplayFlag_g = false;

    // TextChannel_g = String(OutputChannel_g);
    TextVolume_g = String(VolumeValue_g);

    Display_g.clearDisplay();
    Display_g.setTextSize(1);      // Normal 1:1 pixel scale
    Display_g.setTextColor(SSD1306_WHITE); // Draw white text
    Display_g.cp437(true);         // Use full 256 char 'Code Page 437' font

    // Chanel animation.
    // Display_g.setCursor(65, 2)
    // Display_g.print(TextChannel_g);

    if (OutputChannel_g == 1)
    {
        Display_g.fillRect(RECT_X_CH1, RECT_Y_CH1, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else 
    {
        Display_g.drawRect(RECT_X_CH1, RECT_Y_CH1, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }
    
    if (OutputChannel_g == 2)
    {
        Display_g.fillRect(RECT_X_CH2, RECT_Y_CH2, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else
    {
        Display_g.drawRect(RECT_X_CH2, RECT_Y_CH2, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }

    if (OutputChannel_g == 3)
    {
        Display_g.fillRect(RECT_X_CH3, RECT_Y_CH3, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else
    {
        Display_g.drawRect(RECT_X_CH3, RECT_Y_CH3, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }

    if (OutputChannel_g == 4)
    {
        Display_g.fillRect(RECT_X_CH4, RECT_Y_CH4, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else
    {
        Display_g.drawRect(RECT_X_CH4, RECT_Y_CH4, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }

    // Volume animation.
    Display_g.setCursor(TX_X_VOL, TX_Y_VOL);
    Display_g.print(TextVolume_g);
    Display_g.fillRect(RECT_X_VOL, RECT_Y_VOL, VolumeValue_g, RECT_H_VOL, SSD1306_INVERSE);
    Display_g.drawRect(RECT_X_VOL, RECT_Y_VOL, MAX_VOL_VAL,   RECT_H_VOL, SSD1306_WHITE);

    Display_g.display();
}

#pragma endregion
