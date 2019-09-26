#ifndef NABTO_CLIENT_EXPERIMENTAL_H
#define NABTO_CLIENT_EXPERIMENTAL_H

#include "nabto_client.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Experimental
 *
 * Create a private key and return the privat ekey as a pem encoded
 * string. The returned pointer should be freed with
 * nabto_client_experimental_free
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_create_private_key(NabtoClientContext* context, char** privateKey);

/**
 * free a simple type of data allocated by the api.
 */
NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_string_free(char* str);

/**
 * Experimental: Wait for the p2p connection to be established
 *
 * The future resolves as soon a p2p connection has been established
 * or the p2p connection failed.
 *
 * Future error codes
 *  NABTO_CLIENT_OK if a p2p connection was established
 *  NABTO_CLIENT_CONNECTION_NO_P2P if no p2p connection could be made.
 *
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientFuture* NABTO_CLIENT_API
nabto_client_experimental_connection_async_p2p(NabtoClientConnection* connection, const char* hostname, uint16_t port);

/*****************
 * mDNS API
 ******************/
typedef struct NabtoClientMdnsResolver_ NabtoClientMdnsResolver;
typedef struct NabtoClientMdnsResult_ NabtoClientMdnsResult;

/**
 * Experimental: create an mdns resolver
 * @return A new mdns resolver or NULL.
 */

NABTO_CLIENT_DECL_PREFIX NabtoClientMdnsResolver* NABTO_CLIENT_API
nabto_client_experimental_mdns_resolver_new(NabtoClientContext* context);

/**
 * Experimental: free an mdns resolver, any outstanding get calls will
 * be resolved before freeing.
 */
NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_experimental_mdns_resolver_free(NabtoClientMdnsResolver* resolver);

/**
 * Experimental: wait for a result from the mdns resolver.
 *
 * The future resolves when a new mdns result is available or when the
 * resolver is freed.
 *
 * Future error codes
 * NABTO_CLIENT_OK if a new result is available
 * NABTO_CLIENT_ERROR_STOPPED
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientFuture* NABTO_CLIENT_API
nabto_client_experimental_mdns_resolver_get_result(NabtoClientMdnsResolver* resolver, NabtoClientMdnsResult* result);

/**
 * Experimental: allocate new result object
 * @return A new mdns result or NULL
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientMdnsResult* NABTO_CLIENT_API
nabto_client_experimental_mdns_result_new(NabtoClientContext* context);

/**
 * Experimental: free result object
 */
NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_experimental_mdns_result_free(NabtoClientMdnsResult* result);

/**
 * Experimental: get IP address of from result object
 * @return String representation of IP address or NULL
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_mdns_result_get_address(NabtoClientMdnsResult* result, const char** address);

/**
 * Experimental: get port of from result object
 * @return port number or 0
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_mdns_result_get_port(NabtoClientMdnsResult* result, uint16_t* port);

/**
 * Experimental: get device ID of from result object
 * @return the device ID or NULL
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_mdns_result_get_device_id(NabtoClientMdnsResult* result, const char** deviceId);

/**
 * Experimental: get product ID of from result object
 * @return the product ID or NULL
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_mdns_result_get_product_id(NabtoClientMdnsResult* result, const char** productId);

/*****************
 * Connection API
 ******************/
/**
 * Get connection metadata as json object:
 *
 * {
 *     "connection_type":    "P2P",     // P2P|RELAY
 *     "connection_state":   "OPEN",    // OPEN|CLOSED|CLOSED_BY_PEER|TIMEOUT
 *     "duration_millis":    9810,
 *     "current_rtt_millis": 23,
 *     "avg_rtt_millis":     44,
 *     "sent_bytes":         1242,
 *     "received_bytes":     113290
 * }
 *
 * @param connection The connection.
 * @param json The metadata returned as a JSON string.
 * @return NABTO_CLIENT_OK if the connection is connected, json output is set and must be freed by caller.
 *         NABTO_CLIENT_CONNECTION_CLOSED if the connection is closed or not opened yet.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_connection_get_metadata(NabtoClientConnection* connection, char** json);

/************
 * Tunnel   *
 ************/

/**
 * Nabto TCP tunnel handle.
 */
typedef struct NabtoClientTcpTunnel_ NabtoClientTcpTunnel;

/**
 * Create a tunnel (TODO - new+open not necessarily needed for tunnels, for now just following
 * stream pattern)
 *
 * @param connection  The connection to make the tunnel on, the connection needs
 * to be kept alive until the tunnel has been closed.
 * @return  Tunnel handle if the tunnel could be created, NULL otherwise.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientTcpTunnel* NABTO_CLIENT_API
nabto_client_tcp_tunnel_new(NabtoClientConnection* connection);

/**
 * Free a tunnel.
 *
 * @param tunnel, the tunnel to free
 */
NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_tcp_tunnel_free(NabtoClientTcpTunnel* tunnel);

enum NabtoClientTcpTunnelListenMode {
    LISTEN_MODE_LOCALHOST,
    LISTEN_MODE_ANY
};

/**
 * Set the listen mode for the tcp listener. Default is to only listen
 * on localhost / loopback such that only applications on the local
 * machine can connect to the tcp listener. Anyone on the local system
 * can connect to the tcp listener. Some form of application layer
 * authentication needs to be present on the tcp connection if the
 * system is multi tenant or not completely trusted or if the
 * application is not run in isolation.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_tcp_tunnel_listen_mode(NabtoClientTcpTunnel* tunnel,
                                    enum NabtoClientTcpTunnelListenMode listenMode);

/**
 * Opens a TCP tunnel to a remote TCP server through a Nabto enabled device connected to earlier. Device and remote server
 * are often the same.
 *
 * @param tunnel      Tunnel handle crated with nabto_client_tcp_tunnel_new
 * @param localPort   The local TCP port to listen on. If the localPort
 *                    number is 0 the api will choose the port number.
 * @param remoteHost  The host the remote endpoint establishes a TCP
 *                    connection to.
 * @param remotePort  The TCP port to connect to on remoteHost.
 * @return a future, when resolved the tunnel is either established or failed. If established, TCP clients can connect to the endpoint and metadata can be retrieved using nabto_client_tcp_tunnel_get_metadata.
 *
 * Future status:
 *   NABTO_CLIENT_OK if opening went ok.
 *
 *      +--------+          +-----------+               +--------+
 *      | nabto  |   nabto  |   nabto   |   tcp/ip      | remote |
 *   |--+ client +----~~~---+   device  +----~~~-----|--+ server |
 * port | API    |          |           |          port |        |
 *      +--------+           +----------+               +--------+
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientFuture* NABTO_CLIENT_API
nabto_client_tcp_tunnel_open(NabtoClientTcpTunnel* tunnel, uint16_t localPort, const char* remoteHost, uint16_t remotePort);

/**
 * Close a TCP tunnel. Detailed semantics TBD:
 * - TCP listener closed
 * - existing TCP client connections closed?
 * - what about pending stream data?
 *
 * @param tunnel, the tunnel to close.
 *
 * Future status:
 *   TBD
 */

NABTO_CLIENT_DECL_PREFIX NabtoClientFuture* NABTO_CLIENT_API
nabto_client_tcp_tunnel_close(NabtoClientTcpTunnel* tunnel);

/**
 * Get TCP tunnel metadata as json object:
 *
 * {
 *     "listener_port":      53281,
 *     "TBD":                "TBD"
 * }
 *
 * @param tunnel The tunnel to retrieve meta data about
 * @param json The metadata returned as a JSON string.
 * @return NABTO_CLIENT_OK if the connection is connected, json output is set and must be freed by caller.
 *         NABTO_CLIENT_CONNECTION_CLOSED if the connection is closed or not opened yet.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_tcp_tunnel_get_metadata(NabtoClientTcpTunnel* tunnel, char** json);

/************
 * CoAP     *
 ************/

/***
 * Client API Http Server
 *
 * NOT Implemented.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_http_server_start(NabtoClientContext* context, int port);

NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_http_server_stop(NabtoClientContext* context, int port);

NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_http_server_get_port(NabtoClientContext* context, int port);

#ifdef __cplusplus
} // extern C
#endif

#endif
