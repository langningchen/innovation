#include <network.h>

/**
 * @brief NETWORK constructor
 * @tparam CLIENT_MSG message sent from client
 * @tparam SERVER_MSG message responded from server
 * @param pinCE CE pin
 * @param pinCSN CSN pin
 * @param channel channel number, range is [0, 125]
 * @param dataRate data rate
 * @param powerLevel power level
 * @param retriesDelay delay between retries, in multiples of 250 us
 * @param retriesCount number of retries, range is [0, 15]
 * @param address address for the radio, 5 bytes
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
NETWORK<CLIENT_MSG, SERVER_MSG>::NETWORK(uint8_t pinCE, uint8_t pinCSN, uint8_t channel, rf24_datarate_e dataRate, rf24_pa_dbm_e powerLevel, uint8_t retriesDelay, uint8_t retriesCount, uint64_t address)
    : radio(pinCE, pinCSN),
      channel(channel), dataRate(dataRate), powerLevel(powerLevel),
      retriesDelay(retriesDelay), retriesCount(retriesCount),
      address(address) {}

/**
 * @brief Initialize the NETWORK
 * @return true if successful, false otherwise
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::begin()
{
    if (!radio.begin())
        return false;
    radio.setChannel(channel);
    if (!radio.setDataRate(dataRate))
        return false;
    radio.setPALevel(powerLevel);
    radio.setRetries(retriesDelay, retriesCount);
    radio.enableAckPayload();
    return true;
}

/**
 * @brief Set as the server mode
 * @param pipe pipe number, range is [0, 5]
 * @param serverCallback callback function to handle the client message and return the server message
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
void NETWORK<CLIENT_MSG, SERVER_MSG>::setServer(uint8_t pipe, std::function<SERVER_MSG(CLIENT_MSG)> serverCallback)
{
    radio.openReadingPipe(pipe, address), radio.startListening();
    isServer = true, this->pipe = pipe, this->serverCallback = serverCallback;
}

/**
 * @brief Set as the client mode
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
void NETWORK<CLIENT_MSG, SERVER_MSG>::setClient()
{
    radio.openWritingPipe(address), radio.stopListening();
    isServer = false;
}

/**
 * @brief Proceed the server mode
 * @param hasData true if there is data available, false otherwise
 * @return true if successful, false otherwise
 * @note setServer must be called before this function
 * @see NETWORK::setServer
 */
template <typename CLIENT_MSG, typename SERVER_MSG>
bool NETWORK<CLIENT_MSG, SERVER_MSG>::proceedServer(boolean &hasData)
{
    if (radio.available())
    {
        hasData = true;
        CLIENT_MSG clientMsg;
        memset(&clientMsg, 0, sizeof(clientMsg));
        radio.read(&clientMsg, sizeof(clientMsg));
        SERVER_MSG serverMsg = serverCallback(clientMsg);
        return radio.writeAckPayload(pipe, &serverMsg, sizeof(serverMsg));
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
    if (!radio.write(&clientMsg, sizeof(clientMsg)))
        return false;
    memset(&serverMsg, 0, sizeof(serverMsg));
    if (radio.isAckPayloadAvailable())
    {
        radio.read(&serverMsg, sizeof(serverMsg));
        return true;
    }
    return false;
}
