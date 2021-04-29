
// TODO: Add WEB server for configuration.
// TODO: Add WEB API for remote controlling.
// TODO: Add IR remote controll.
// TODO: Add IR training button for the separate commands.

#pragma region Headers

#include "ApplicationConfiguration.h"

#include "DebugPort.h"

#include "GeneralHelper.h"

#include "DeviceConfiguration.h"

#include "DefaultCredentials.h"

#include "VolumeAction.h"

#include "NetworkConfiguration.h"


#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#include <pt2257.h>

#include <Button.h>


#include <Wire.h>


#include <WiFi.h>

#include <WiFiAP.h>


#include "FS.h"

#include "SPIFFS.h"

#include <ArduinoOTA.h>


#include <IRremote.h>

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
uint8_t ChannelIndex_g;

/**
 * @brief Last state of the output channel.
 * 
 */
uint8_t LastChannelIndex_g;

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

/**
 * @brief Station mode SSID.
 * 
 */
#ifdef DEFAULT_SSID
const char* SSID_STA_g = DEFAULT_SSID;
#else
const char* SSID_STA_g = "WuoyrWiFiSSID";
#endif

/**
 * @brief Station mode password.
 * 
 */
#ifdef DEFAULT_STA_PASSWORD
const char* PASS_STA_g = DEFAULT_STA_PASSWORD;
#else
const char* PASS_STA_g = "YourWiFiPassword";
#endif

/**
 * @brief Access point mode SSID.
 * 
 */
String SSID_AP_g = "TEST_AP_NAME";

/**
 * @brief Access point mode password.
 * 
 */
String PASS_AP_g = "12345678";

bool LearnIRCommandFlag_g;

bool ReceiveIRCommand_g;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Configure the file system.
 * 
 */
void setup_fs();

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
 * @brief Setup the WiFi.
 * 
 */
void setup_wifi();

/**
 * @brief Setup the Arduino OTA.
 * 
 */
void setup_arduino_ota();


/**
 * @brief Update the button status.
 * 
 */
void update_buttons();

/**
 * @brief Update IR receiver.
 * 
 */
void update_ir_receiver();

/**
 * @brief Update learn procedure.
 * 
 */
void update_learn_procedure();

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
 * @brief Update display.
 * 
 */
void update_display();

/**
 * @brief Update the status LED.
 * 
 */
void update_status_led();

#pragma endregion

void setup()
{
    setup_debug_port();

    setup_fs();

	// Try to load configuration from file system. Load defaults if any error.
	if (!load_network_configuration(&SPIFFS, CONFIG_NET))
	{
		save_network_configuration(&SPIFFS, CONFIG_NET);
	}

	// Load HTTP authorization. Load defaults if any error.
	if (!load_device_config(&SPIFFS, CONFIG_DEVICE))
	{
		save_device_config(&SPIFFS, CONFIG_DEVICE);
	}

    setup_variables();

    setup_io();
    
    setup_pot();

    setup_display();

    setup_wifi();

    setup_arduino_ota();
}

void loop()
{
    update_buttons();

    update_ir_receiver();

    update_learn_procedure();

    update_channel();

    update_volume();

    update_display();

    ArduinoOTA.handle();

    update_status_led();
}

#pragma region Functions

/**
 * @brief Configure the file system.
 * 
 */
void setup_fs() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

	if (!SPIFFS.begin())
	{
		DEBUGLOG("Can not load file system.\r\n");

		for (;;) {
#ifdef ESP32
// ESP32

#elif defined(ESP8266)
// ESP8266
			ESP.wdtFeed();
#endif
		}
	}

#ifdef DEBUGLOG
#ifdef ESP32
// ESP32
	File root = SPIFFS.open("/");
	File file = root.openNextFile();
	while(file)
	{
		DEBUGLOG("File: %s, size: %s\r\n", file.name(), formatBytes(file.size()).c_str()); 
		file = root.openNextFile();
	}

#elif defined(ESP8266)
// ESP8266
	// List files
	Dir dir = SPIFFS.openDir("/");
	while (dir.next())
	{
		String fileName = dir.fileName();
		size_t fileSize = dir.fileSize();
		DEBUGLOG("File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
	}

#endif

	DEBUGLOG("\r\n");
#endif // DEBUGLOG
}

/**
 * @brief Setup the variables objects.
 * 
 */
void setup_variables() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

    // Status LED.
    StatusLEDState_g = LOW;
    
    // Update loop timing.
    PreviousTime_g = 0;
    CurrentTime_g = 0;
    
    // User IO.
    LearnIRCommandFlag_g = false;
    ReceiveIRCommand_g = false;
    ChannelIndex_g = DeviceConfiguration.ChannelIndex;
    LastChannelIndex_g = 255;
    LastVolAct_g = VolumeAction_t::Nothing;
    VolAct_g = VolumeAction_t::Nothing;
    VolumeValue_g = DeviceConfiguration.Volume;
    DisplayFlag_g = false;
    // TextChannel_g = "";
    TextVolume_g = "";

    // Wi-Fi setup.    
    SSID_STA_g = NetworkConfiguration.SSID.c_str();
    PASS_STA_g = NetworkConfiguration.Password.c_str();

    //

}

/**
 * @brief Set the GPIOs.
 * 
 */
void setup_io() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

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

    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    // Start the receiver, enable feedback LED,
    // take LED feedback pin from the internal boards definition.
    IrReceiver.begin(PIN_IR, ENABLE_LED_FEEDBACK);
}

/**
 * @brief Setup the potentiometer.
 * 
 */
void setup_pot() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

    Pot_g.init();
    set_volume(VolumeValue_g);
}

/**
 * @brief Setup the display.
 * 
 */
void setup_display() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES
    
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!Display_g.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        DEBUGLOG("SSD1306 allocation failed");
        for(;;); // Don't proceed, loop forever
    }
    
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    Display_g.display();
    
    // Clear the buffer
    Display_g.clearDisplay();
}

/**
 * @brief Setup the WiFi.
 * 
 */
void setup_wifi() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

    int NetworksCountL = 0;
    String SSIDL = "";
    wifi_mode_t WiFiL = WIFI_AP;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Get the count of al found networks.
    NetworksCountL = WiFi.scanNetworks();

    // If no network found move to AP mode.
    if (NetworksCountL > 0)
    {
        for (int i = 0; i < NetworksCountL; ++i)
        {
            SSIDL = WiFi.SSID(i);
            if ((const String)SSID_STA_g == SSIDL)
            {
                WiFiL = WIFI_STA;
            }
        }
    }

    // STA mode.
    if (WiFiL == WIFI_STA)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID_STA_g, PASS_STA_g);
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            WiFiL = WIFI_AP;
        }
    }

    // AP mode.
    else if (WiFiL == WIFI_AP)
    {
        WiFi.softAP(SSID_AP_g.c_str(), PASS_AP_g.c_str());
    }
}

/**
 * @brief Setup the Arduino OTA.
 * 
 */
void setup_arduino_ota() {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

	// Port defaults to 3232
	// ArduinoOTA.setPort(3232);

	// Hostname defaults to esp3232-[MAC]
	// ArduinoOTA.setHostname("myesp32");

	// No authentication by default
	// ArduinoOTA.setPassword("admin");

	// Password can be set with it's md5 value as well
	// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
	// ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

	ArduinoOTA
		.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
				type = "sketch";
			else // U_SPIFFS
				type = "filesystem";

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			DEBUGLOG("Start updating " + type + "\r\n");
		})
		.onEnd([]() {
			DEBUGLOG("\r\nEnd");
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			DEBUGLOG("Progress: %u%%\r\n", (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			DEBUGLOG("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) DEBUGLOG("Auth Failed\r\n");
			else if (error == OTA_BEGIN_ERROR) DEBUGLOG("Begin Failed\r\n");
			else if (error == OTA_CONNECT_ERROR) DEBUGLOG("Connect Failed\r\n");
			else if (error == OTA_RECEIVE_ERROR) DEBUGLOG("Receive Failed\r\n");
			else if (error == OTA_END_ERROR) DEBUGLOG("End Failed\r\n");
		});

	ArduinoOTA.begin();
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
        ChannelIndex_g = 1;
    }
    else if (BtnIn2_g.pressed())
    {
        // Set the chanel.
        ChannelIndex_g = 2;
    }
    else if (BtnIn3_g.pressed())
    {
        // Set the chanel.
        ChannelIndex_g = 3;
    }
    else if (BtnIn4_g.pressed())
    {
        // Set the chanel.
        ChannelIndex_g = 4;
    }
    else if (BtnLearn_g.pressed())
    {
        if (LearnIRCommandFlag_g == false)
        {
            LearnIRCommandFlag_g = true;
        }
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
        (ChannelIndex_g != LastChannelIndex_g);
}

/**
 * @brief Update IR receiver.
 * 
 */
void update_ir_receiver()
{
    if (IrReceiver.decode())
    {
        // Grab an IR code
        // Check if the buffer overflowed
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW)
        {
            Serial.println("IR code too long. Edit IRremoteInt.h and increase RAW_BUFFER_LENGTH");
        }
        else
        {
            Serial.println();                               // 2 blank lines between entries
            Serial.println();
            IrReceiver.printIRResultShort(&Serial);
            Serial.println();
            Serial.println(F("Raw result in internal ticks (50 us) - with leading gap"));
            IrReceiver.printIRResultRawFormatted(&Serial, false); // Output the results in RAW format
            Serial.println(F("Raw result in microseconds - with leading gap"));
            IrReceiver.printIRResultRawFormatted(&Serial, true);  // Output the results in RAW format
            Serial.println();                               // blank line between entries
            Serial.print(F("Result as internal ticks (50 us) array - compensated with MARK_EXCESS_MICROS="));
            Serial.println(MARK_EXCESS_MICROS);
            IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, false); // Output the results as uint8_t source code array of ticks
            Serial.print(F("Result as microseconds array - compensated with MARK_EXCESS_MICROS="));
            Serial.println(MARK_EXCESS_MICROS);
            IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, true); // Output the results as uint16_t source code array of micros
            IrReceiver.printIRResultAsCVariables(&Serial);  // Output address and data as source code variables

            IrReceiver.compensateAndPrintIRResultAsPronto(&Serial);

            Serial.print(F("Decoded protocol: "));
            Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
            Serial.print(F("Decoded raw data: "));
            Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
            Serial.print(F(", decoded address: "));
            Serial.print(IrReceiver.decodedIRData.address, HEX);
            Serial.print(F(", decoded command: "));
            Serial.println(IrReceiver.decodedIRData.command, HEX);

            // TODO: Store last received IR data. 
            // - If no learning button is pressed if it is known do action.
            // - If learning button is pressed see which button is pressed and store IR data for the given action.

            if (ReceiveIRCommand_g == false)
            {
                ReceiveIRCommand_g = true;
            }

        }
        IrReceiver.resume();                            // Prepare for the next value
    }
}

/**
 * @brief Update learn procedure.
 * 
 */
void update_learn_procedure()
{
    if ((LearnIRCommandFlag_g == true) && (ReceiveIRCommand_g == true))
    {
        // Clear the learn flag.
        if (LearnIRCommandFlag_g == true)
        {
            LearnIRCommandFlag_g = false;
        }

        // Clear the received flag.
        if (ReceiveIRCommand_g == true)
        {
            ReceiveIRCommand_g = false;
        }

        if (ChannelIndex_g == 1)
        {
            // Save IR command for chanel index 1.
        }
        else if (ChannelIndex_g == 2)
        {
            // Save IR command for chanel index 2.
        }
        else if (ChannelIndex_g == 3)
        {
            // Save IR command for chanel index 3.
        }
        else if (ChannelIndex_g == 4)
        {
            // Save IR command for chanel index 4.
        }

        // Bypass button chanel action.
        ChannelIndex_g = LastChannelIndex_g;
    }
}

/**
 * @brief Check for chanel changes.
 * 
 */
void update_channel()
{
    if (ChannelIndex_g != LastChannelIndex_g)
    {
        set_channel(ChannelIndex_g);
        LastChannelIndex_g = ChannelIndex_g;
        DeviceConfiguration.ChannelIndex = ChannelIndex_g;
        save_device_config(&SPIFFS, CONFIG_DEVICE);
    }
}

/**
 * @brief Set the channel.
 * 
 * @param channel Chanel index.
 */
void set_channel(uint8_t channel) {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

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
        
        DeviceConfiguration.Volume = VolumeValue_g;
        save_device_config(&SPIFFS, CONFIG_DEVICE);
    }
}

/**
 * @brief Set the volume.
 * 
 * @param volume Volume index.
 */
void set_volume(uint8_t  volume) {
#ifdef SHOW_FUNC_NAMES
	DEBUGLOG("\r\n");
	DEBUGLOG(__PRETTY_FUNCTION__);
	DEBUGLOG("\r\n");
#endif // SHOW_FUNC_NAMES

    // Scale the pot value.
    uint8_t PotValL = (uint8_t)map(volume, MIN_VOL_VAL, MAX_VOL_VAL, MIN_POT_VAL, MAX_POT_VAL);

    // Set the volume.
    Pot_g.set_volume(PotValL);
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

    // TextChannel_g = String(ChannelIndex_g);
    TextVolume_g = String(VolumeValue_g);

    Display_g.clearDisplay();
    Display_g.setTextSize(1);      // Normal 1:1 pixel scale
    Display_g.setTextColor(SSD1306_WHITE); // Draw white text
    Display_g.cp437(true);         // Use full 256 char 'Code Page 437' font

    // Chanel animation.
    // Display_g.setCursor(65, 2)
    // Display_g.print(TextChannel_g);

    if (ChannelIndex_g == 1)
    {
        Display_g.fillRect(RECT_X_CH1, RECT_Y_CH1, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else 
    {
        Display_g.drawRect(RECT_X_CH1, RECT_Y_CH1, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }
    
    if (ChannelIndex_g == 2)
    {
        Display_g.fillRect(RECT_X_CH2, RECT_Y_CH2, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else
    {
        Display_g.drawRect(RECT_X_CH2, RECT_Y_CH2, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }

    if (ChannelIndex_g == 3)
    {
        Display_g.fillRect(RECT_X_CH3, RECT_Y_CH3, RECT_W_CH, RECT_H_CH, SSD1306_INVERSE);
    }
    else
    {
        Display_g.drawRect(RECT_X_CH3, RECT_Y_CH3, RECT_W_CH, RECT_H_CH, SSD1306_WHITE);
    }

    if (ChannelIndex_g == 4)
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

#pragma endregion
