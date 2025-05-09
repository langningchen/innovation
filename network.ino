#include <network.hpp>

/**
 * @brief NETWORK constructor
 * @tparam CLIENT_MSG message sent from client
 * @tparam SERVER_MSG message responded from server
 * @param pinCS CS pin
 * @param pinRESET RESET pin
 * @param pinIRQ IRQ pin
 * @param pinBUSY BUSY pin
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
NETWORK<CLIENT_MSG, SERVER_MSG>::NETWORK(uint8_t pinCS, uint8_t pinRESET, uint8_t pinIRQ, uint8_t pinBUSY)
    : radio(pinCS, pinRESET, pinIRQ, pinBUSY) {}

/**
 * @brief Initialize the NETWORK
 * @return true if successful, false otherwise
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::begin()
{
    if (radio.begin() != RADIOLIB_ERR_NONE)
        return false;
    radio.setPacketReceivedAction([this]() -> void
                                  { packageReceived = true; });
    return true;
}

/**
 * @brief Set as the server mode
 * @param serverCallback callback function to handle the client message and return the server message
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::setServer(std::function<SERVER_MSG(CLIENT_MSG)> serverCallback)
{
    if (!radio.startReceive())
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
 * @return true if successful, false otherwise
 * @note setServer must be called before this function
 * @see NETWORK::setServer
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::proceedServer(bool &hasData)
{
    hasData = packageReceived;
    if (packageReceived)
    {
        packageReceived = false;
        uint16_t irqStatus = radio.getIrqStatus();
        if (irqStatus & RADIOLIB_SX128X_IRQ_RX_DONE)
        {
            CLIENT_MSG clientMsg;
            if (!radio.readData((uint8_t *)&clientMsg, sizeof(clientMsg)))
                return false;
            SERVER_MSG serverMsg = serverCallback(clientMsg);
            if (!radio.transmit((uint8_t *)&serverMsg, sizeof(serverMsg)))
                return false;
            radio.startReceive();
        }
        else
            return false;
    }
    return true;
}

/**
 * @brief Proceed the client mode
 * @param clientMsg message to send to the server
 * @param serverMsg message received from the server
 * @return true if successful, false otherwise
 * @note setClient must be called before this function
 * @see NETWORK::setClient
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::proceedClient(CLIENT_MSG clientMsg, SERVER_MSG &serverMsg)
{
    if (!radio.transmit(&clientMsg, sizeof(clientMsg)))
        return false;
    memset(&serverMsg, 0, sizeof(serverMsg));
    if (!radio.receive(&serverMsg, sizeof(serverMsg)))
        return false;
    return true;
}
