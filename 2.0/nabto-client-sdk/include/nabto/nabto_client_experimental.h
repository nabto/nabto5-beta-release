#ifndef NABTO_CLIENT_EXPERIMENTAL_H
#define NABTO_CLIENT_EXPERIMENTAL_H

#include "nabto_client.h"

#ifdef __cplusplus
extern "C" {
#endif


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
 * Connection API
 ******************/

typedef char* NabtoClientString;

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
nabto_client_connection_get_metadata(NabtoClientConnection* connection, NabtoClientString* json);

/**
 * Free a string resource returned earlier by the API.
 * @param string The string resource to free
 */
NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_string_free(NabtoClientString* string);


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
nabto_client_tcp_tunnel_open(NabtoClientTcpTunnel* tunnel, int localPort, const char* remoteHost, int remotePort);

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
nabto_client_tcp_tunnel_get_metadata(NabtoClientTcpTunnel* tunnel, NabtoClientString* json);

/************
 * CoAP     *
 ************/

typedef enum {
    NABTO_CLIENT_COAP_GET = 1,
    NABTO_CLIENT_COAP_POST = 2,
    NABTO_CLIENT_COAP_PUT = 3,
    NABTO_CLIENT_COAP_DELETE = 4
} NabtoClientCoapMethod;

typedef enum {
    NABTO_CLIENT_COAP_CONTENT_FORMAT_TEXT_PLAIN_UTF8 = 0,
    NABTO_CLIENT_COAP_CONTENT_FORMAT_APPLICATION_LINK_FORMAT = 40,
    NABTO_CLIENT_COAP_CONTENT_FORMAT_XML = 41,
    NABTO_CLIENT_COAP_CONTENT_FORMAT_APPLICATION_OCTET_STREAM = 42,
    NABTO_CLIENT_COAP_CONTENT_FORMAT_APPLICATION_JSON = 50,
    NABTO_CLIENT_COAP_CONTENT_FORMAT_APPLICATION_CBOR = 60
} NabtoClientCoapContentFormat;

typedef struct NabtoClientCoapRequest_ NabtoClientCoapRequest;
typedef struct NabtoClientCoapResponse_ NabtoClientCoapResponse;

typedef struct NabtoClientBuffer_ NabtoClientBuffer;

/**
 * Create a new coap request on the given connection
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientCoapRequest* NABTO_CLIENT_API
nabto_client_experimental_coap_request_new(NabtoClientConnection* connection, NabtoClientCoapMethod method, const char* path);

NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_experimental_coap_request_free(NabtoClientCoapRequest* coap);

/**
 * Observe a resource.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_coap_request_set_observe(NabtoClientCoapRequest* coap);

/**
 * Set payload and content type for the payload
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_coap_request_set_payload(NabtoClientCoapRequest* coap, uint16_t contentFormat, void* payload, size_t payloadLength);

/**
 * Set accept content formats, this function can be called multiple
 * times for each content format the client accepts.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_coap_request_set_accept(NabtoClientCoapRequest* coap, uint16_t contentFormat);

/**
 * Execute a coap request.
 *
 * If the request is an observable the execute function can be called
 * multiple times on the same request. Each new call will wait for a
 * new observed value.
 *
 * If the request went well the response is set.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientFuture* NABTO_CLIENT_API
nabto_client_experimental_coap_execute(NabtoClientCoapRequest* request);

/**
 * Get response status. encoded as e.g. 404, 200, 203, 500
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_coap_response_get_status_code(NabtoClientCoapRequest* request, uint16_t* statusCode);

/**
 * Get content type of the payload if one exists.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientError NABTO_CLIENT_API
nabto_client_experimental_coap_response_get_content_type(NabtoClientCoapRequest* request, uint16_t* contentType);

/**
 * Get the response data.
 */
NABTO_CLIENT_DECL_PREFIX NabtoClientBuffer* NABTO_CLIENT_API
nabto_client_experimental_coap_response_get_payload(NabtoClientCoapRequest* request);

NABTO_CLIENT_DECL_PREFIX void NABTO_CLIENT_API
nabto_client_experimental_buffer_free(NabtoClientBuffer* buffer);

/**
 * Return a pointer to the buffer data.
 */
NABTO_CLIENT_DECL_PREFIX void* NABTO_CLIENT_API
nabto_client_experimental_buffer_data(NabtoClientBuffer* buffer);

/**
 * Return the length of the data in the buffer.
 */
NABTO_CLIENT_DECL_PREFIX size_t NABTO_CLIENT_API
nabto_client_experimental_buffer_size(NabtoClientBuffer* buffer);

/**
 * Coap example
 *
 * NabtoClientCoapRequest* request = nabto_client_experimental_coap_request_new(connection, NABTO_CLIENT_COAP_GET, "/temperature/living_room");
 * NabtoClientFuture* future = nabto_client_experimental_coap_execute(request);
 * nabto_client_future_wait(future);
 *
 * void* payload = NULL;
 * size_t payloadLength;
 * nabto_client_experimental_coap_response_get_payload(request, &payload, &payloadLength);
 * nabto_client_future_free(future);
 * nabto_client_experimental_buffer_free(payload);
 * nabto_client_experimental_coap_request_free(request);
 *
 * DO STUFF HERE
 *
 * free(payload);
 *
 */

/***
 * Client API Http Server
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
