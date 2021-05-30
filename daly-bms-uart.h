

#ifndef DALY_BMS_UART_H
#define DALY_BMS_UART_H

// Uncomment the below #define to enable debugging print statements.
// NOTE: You must call Serial.begin(<baud rate>) in your setup() for this to work
// #define DALY_BMS_DEBUG

#define XFER_BUFFER_LENGTH 13

class Daly_BMS_UART
{
public:
    // These command names are adapted/translated from the protocol description (see /docs/)
    // Note: Not all are currently supported with public functions
    enum COMMAND
    {
        VOUT_IOUT_SOC = 0x90,
        MIN_MAX_CELL_VOLTAGE = 0x91,
        MIN_MAX_TEMPERATURE = 0x92,
        DISCHARGE_CHARGE_MOS_STATUS = 0x93,
        STATUS_INFO = 0x94,
        CELL_VOLTAGES = 0x95,
        FAILURE_CODES = 0x98
    };

    Daly_BMS_UART(HardwareSerial &serialIntf);

    /**
     * @brief Initializes this driver
     * @details Configures the serial peripheral and pre-loads the transmit buffer with command-independent bytes
     */
    bool Init();

    /**
     * @brief Gets Voltage, Current, and SOC measurements from the BMS
     * @param voltage returns voltage in deci-volts (ex. 57.3V = 573)
     * @param current returns current out of the battery in deci-amps (ex. 3.9A = 39)
     * @param SOC returns soc in tenths of percent (ex 49.3% = 493)
     * @return True on successful aquisition, false otherwise
     */
    bool getPackMeasurements(uint16_t &voltage, int16_t &current, uint16_t &SOC);

    /**
     * @brief Gets the pack temperature in degrees celcius
     * @details This function uses the MIN_MAX_TEMPERATURE command, and averages the 
     * min and max temperatures to get the returned value
     * @return True on successful aquisition, false otherwise
     */
    bool getPackTemp(int8_t &temp);

    /**
     * @brief
     */
    bool getMinMaxCellVoltage(float &minCellV, uint8_t &minCellVNum, float &maxCellV, uint8_t &maxCellVNum);

private:
    HardwareSerial *serialIntf;

    /**
     * @brief Sends a complete packet with the specified command
     * @details calculates the checksum and sends the command over the specified serial connection
     */
    void sendCommand(COMMAND cmdID);

    /**
     * @brief 
     * @details
     * @return True on success, false on failure
     */
    bool receiveBytes(void);

    /**
     * @brief Validates the checksum in the RX Buffer
     * @return true if checksum matches, false otherwise
     */
    bool validateChecksum();

    /**
     * @brief Prints out the contense of the RX buffer
     * @details Useful for debugging
     */
    void barfRXBuffer();

    /**
     * @brief Buffer used to transmit data to the BMS
     * @details Populated primarily in the "Init()" function, see the readme for more info
     */
    uint8_t my_txBuffer[XFER_BUFFER_LENGTH];

    /**
     * @brief Buffer filled with data from the BMS
     */
    uint8_t my_rxBuffer[XFER_BUFFER_LENGTH];
};

#endif // DALY_BMS_UART_H