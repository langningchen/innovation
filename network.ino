#include <network.hpp>

template <typename CLIENT_MSG, typename SERVER_MSG>
NETWORK<CLIENT_MSG, SERVER_MSG> *NETWORK<CLIENT_MSG, SERVER_MSG>::currentInstance = nullptr;

template <typename CLIENT_MSG, typename SERVER_MSG>
inline void NETWORK<CLIENT_MSG, SERVER_MSG>::packetReceived()
{
    if (currentInstance != nullptr)
        currentInstance->packageReceived = true;
}

/**
 * @brief NETWORK constructor
 * @tparam CLIENT_MSG message sent from client
 * @tparam SERVER_MSG message responded from server
 * @param pinCS CS pin
 * @param pinIRQ IRQ pin
 * @param pinRESET RESET pin
 * @param pinBUSY BUSY pin
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
NETWORK<CLIENT_MSG, SERVER_MSG>::NETWORK(uint8_t pinCS, uint8_t pinIRQ, uint8_t pinRESET, uint8_t pinBUSY)
{
    radio = new SX1281(new Module(pinCS, pinIRQ, pinRESET, pinBUSY));
}

/**
 * @brief Initialize the NETWORK
 * @return true if successful, false otherwise
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::begin()
{
    if (radio->begin() != RADIOLIB_ERR_NONE)
        return false;
    currentInstance = this;
    radio->setPacketReceivedAction(NETWORK::packetReceived);
    return true;
}

/**
 * @brief Set as the server mode
 * @param serverCallback callback function to handle the client message and return the server message
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::setServer(std::function<SERVER_MSG(CLIENT_MSG)> serverCallback)
{
    if (radio->startReceive() != RADIOLIB_ERR_NONE)
        return false;
    isServer = true;
    this->serverCallback = serverCallback;
    return true;
}

/**
 * @brief Set as the client mode
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
void NETWORK<CLIENT_MSG, SERVER_MSG>::setClient() { isServer = false; }

/**
 * @brief Proceed the server mode
 * @param hasData true if there is data available, false otherwise
 * @return int16_t status code
 * @note setServer must be called before this function
 * @see NETWORK::setServer
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
int16_t NETWORK<CLIENT_MSG, SERVER_MSG>::proceedServer(bool &hasData)
{
    int16_t status = RADIOLIB_ERR_NONE;
    hasData = packageReceived;
    if (packageReceived)
    {
        packageReceived = false;
        uint16_t irqStatus = radio->getIrqStatus();
        if (irqStatus & RADIOLIB_SX128X_IRQ_RX_DONE)
        {
            CLIENT_MSG clientMsg;
            status = radio->readData((uint8_t *)&clientMsg, sizeof(clientMsg));
            if (status == RADIOLIB_ERR_NONE)
            {
                SERVER_MSG serverMsg = serverCallback(clientMsg);
                status = radio->transmit((uint8_t *)&serverMsg, sizeof(serverMsg));
            }
            std::ignore = radio->startReceive();
        }
    }
    return status;
}

/**
 * @brief Proceed the client mode
 * @param clientMsg message to send to the server
 * @param serverMsg message received from the server
 * @return int16_t status code
 * @note setClient must be called before this function
 * @see NETWORK::setClient
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
int16_t NETWORK<CLIENT_MSG, SERVER_MSG>::proceedClient(CLIENT_MSG clientMsg, SERVER_MSG &serverMsg)
{
    memset(&serverMsg, 0, sizeof(serverMsg));
    int16_t status = RADIOLIB_ERR_NONE;
    status = radio->transmit((uint8_t *)&clientMsg, sizeof(clientMsg));
    if (status == RADIOLIB_ERR_NONE)
        status = radio->receive((uint8_t *)&serverMsg, sizeof(serverMsg));
    return status;
}
