#ifndef _NABTO_DEVICE_H_
#define _NABTO_DEVICE_H_

/**
 * Nabto Device High Level C Api.
 *
 * Nabto provides a platform for connecting applications with
 * devices. The platform consists of three major parts.
 *
 * Vocabulary:
 *
 * Client: Clients are often apps where the nabto_client library is
 * embedded inside. The clients can make connections to devices. Using
 * the servers.
 *
 * Device: Devices is often embedded devices running the Nabto
 * Embedded SDK, e.g. a heating control system or an ip camera. A
 * device instance can be created by this api.
 *
 * Server: Servers are hosted in datacenters and makes it possible to
 * create connections between the clients and devices.
 */

#if defined(_WIN32)
#define NABTO_DEVICE_API __stdcall
#if defined(NABTO_DEVICE_WIN32_API_STATIC)
#define NABTO_DEVICE_DECL_PREFIX extern
#elif defined(NABTO_DEVICE_CLIENT_API_EXPORTS)
#define NABTO_DEVICE_DECL_PREFIX __declspec(dllexport)
#else
#define NABTO_DEVICE_DECL_PREFIX __declspec(dllimport)
#endif
#else
#define NABTO_DEVICE_API
#define NABTO_DEVICE_DECL_PREFIX extern
#endif

//#include <nabto_types.h>

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The NabtoDevice is a the place which owns the device id,
 * sockets, etc.
 */
typedef struct NabtoDevice_ NabtoDevice;

/**
 * The NabtoDeviceConnection represents a connection between a client
 * and device.
 */
typedef struct NabtoDeviceConnection_ NabtoDeviceConnection;

/**
 * The NabtoDeviceStream represents a stream on top of a connection.
 */
typedef struct NabtoDeviceStream_ NabtoDeviceStream;

/**
 * The NabtoDeviceFuture is used to resolve asyncronous function calls
 */
typedef struct NabtoDeviceFuture_ NabtoDeviceFuture;

/**
 * The nabto_device_duration_t is a time abstraction used to wait for
 * futures.
 */
typedef uint32_t nabto_device_duration_t;

typedef int NabtoDeviceError;

/**
 * Connection reference, used to correlate requests on connections
 * with e.g. IAM systems.
 */
typedef uint64_t NabtoDeviceConnectionRef;

/**
 * The NabtoDeviceError represents error codes
 */
extern const NabtoDeviceError NABTO_DEVICE_EC_OK;
extern const NabtoDeviceError NABTO_DEVICE_EC_FAILED;
extern const NabtoDeviceError NABTO_DEVICE_EC_NOT_IMPLEMENTED;
extern const NabtoDeviceError NABTO_DEVICE_EC_INVALID_LOG_LEVEL;
extern const NabtoDeviceError NABTO_DEVICE_EC_OUT_OF_MEMORY;
extern const NabtoDeviceError NABTO_DEVICE_EC_STRING_TOO_LONG;
extern const NabtoDeviceError NABTO_DEVICE_EC_OPERATION_IN_PROGRESS;
extern const NabtoDeviceError NABTO_DEVICE_EC_API_FUTURE_NOT_READY;
extern const NabtoDeviceError NABTO_DEVICE_EC_ABORTED;
extern const NabtoDeviceError NABTO_DEVICE_EC_INVALID_STATE;

/**********************
 * Device Api *
 **********************/

/**
 * Create a new device instance.
 * @return the new device instance
 */
NABTO_DEVICE_DECL_PREFIX NabtoDevice* NABTO_DEVICE_API
nabto_device_new();

/**
 * Free a device instance
 *
 * @param device   The device instance to free
 */
NABTO_DEVICE_DECL_PREFIX void NABTO_DEVICE_API
nabto_device_free(NabtoDevice* device);

/**
 * Set the product id
 *
 * @param device    The device instance to perform action on
 * @param productId The product ID to set e.g. pr-abcdefg
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_OUT_OF_MEMORY if string could not be saved
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_product_id(NabtoDevice* device, const char* productId);

/**
 * Set the device id.
 *
 * @param device    The device instance to perform action on
 * @param deviceId  The device ID to set e.g. de-abcdefg
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_OUT_OF_MEMORY if string could not be saved
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_device_id(NabtoDevice* device, const char* deviceId);

/**
 * Set the server url.
 *
 * @param device    The device instance to perform action on
 * @param serverUrl The url of the basestation attach node to set e.g. foo.bar.baz
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_OUT_OF_MEMORY if string could not be saved
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_server_url(NabtoDevice* device, const char* serverUrl);

/**
 * Set the private key from the device.
 *
 * @param device    The device instance to perform action on
 * @param privKey   The private code to set
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_OUT_OF_MEMORY if string could not be saved
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_private_key(NabtoDevice* device, const char* privKey);

// TODO: distinguish different applications in bs / console / device
/**
 * Set the application name of the device.
 *
 * @param device    The device instance to perform action on
 * @param name      The application name to set
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_STRING_TOO_LOG if string length > 32
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_app_name(NabtoDevice* device, const char* name);

/**
 * Set the application version the device.
 *
 * @param device    The device instance to perform action on
 * @param version   The application version to set
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_STRING_TOO_LOG if string length > 32
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_app_version(NabtoDevice* device, const char* version);

/**
 * Set local port to use, if unset or 0 using ephemeral
 *
 * @param device    The device instance to perform action on
 * @param port      The port number to set
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_set_local_port(NabtoDevice* device, uint16_t port);

/**
 * Get the local port used by the device if empemeral port is used by
 * the device. If set_local_port was used, the port set will be
 * returned.
 *
 * @param device    The device instance to perform action on
 * @param port      Reference port to set
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_get_local_port(NabtoDevice* device, uint16_t* port);

/**
 * Start the context, attach to some servers if possible, wait for
 * client connections.
 *
 * @param device    The device instance to start
 * @return
 *  NABTO_DEVICE_EC_OK on success
 *  NABTO_DEVICE_EC_INVALID_STATE if device does not have public Key,
 *               private key, server URL, device ID, or Product ID.
 *  NABTO_DEVICE_EC_FAILED if device threads could not be started
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_start(NabtoDevice* device);


/**
 * Get the public key fingerprint of the device.
 *
 * @param fingerprint  the fingerprint is stored as hex in the
 * parameter. The fingerprint should be freed by calling
 * nabto_device_string_free afterwards.
 *
 * @return
 *  NABTO_DEVICE_EC_OK iff the fingerprint is available in the fingerprint output parameter.
 *  NABTO_DEVICE_EC_INVALID_STATE if the device provided did not contain a valid private key.
 *  NABTO_DEVICE_EC_FAILED on underlying DTLS module error
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_get_device_fingerprint_hex(NabtoDevice* device, char** fingerprint);

/**
 * Close a context.
 *
 * @param device   The device instance to close
 * @return a future, when resolved the device is closed and can be freed
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_close(NabtoDevice* device);

/**************
 * Connection *
 **************/

/* /\** */
/*  * Listen for new connections. */
/*  *\/ */
/* NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API */
/* nabto_device_connection_listen(NabtoDevice* device, NabtoDeviceConnection** connection); */

/* nabto_device_connection_free(NabtoDeviceConnection* connection); */

/* nabto_device_connection_close(NabtoDeviceConnection* connection); */

/* nabto_device_connection_get_client_fingerprint(NabtoDeviceConnection* connection); */

/*************
 * Streaming *
 *************/

/**
 * listen for a stream, the returned NabtoDeviceStream* should be
 * freed after use.
 *
 * @param device  device
 * @param port    A number describing the id/port of the stream to listen for.
 *                Think of it as a demultiplexing port number.
 * @return Future which resolves once a new stream is available, or on error
 *
 * Future status:
 *   NABTO_DEVICE_EC_OK if new stream is available
 *   NABTO_DEVICE_EC_ABORTED if device is closed
 *   NABTO_DEVICE_EC_FAILED on failure
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_listen(NabtoDevice* device, uint32_t port, NabtoDeviceStream** stream);

/**
 * Free a stream
 *
 * @param stream, the stream to free
 */
NABTO_DEVICE_DECL_PREFIX void NABTO_DEVICE_API
nabto_device_stream_free(NabtoDeviceStream* stream);

/**
 * Accept a stream. After a stream is returned from listen, if the
 * stream is accepted this function is called.
 *
 * @param stream, the stream to accept
 * @return a future when resolved the stream is either established or failed.
 *
 * Future status:
 *   NABTO_DEVICE_EC_OK if opening went ok.
 *   NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if other accept is in progress
 *   NABTO_DEVICE_EC_ABORTED if device is closed
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_accept(NabtoDeviceStream* stream);

/**
 * Get the id for the underlying connection
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceConnectionRef NABTO_DEVICE_API
nabto_device_stream_get_connection_ref(NabtoDeviceStream* stream);

/**
 * Read exactly n bytes from a stream
 *
 * if (readLength != bufferLength) the stream has reached a state
 * where no more bytes can be read.
 *
 * @param stream, the stream to read bytes from.
 * @param buffer, the buffer to put data into.
 * @param bufferLength, the length of the output buffer.
 * @param readLength, the actual number of bytes read.
 * @return  a future which resolves with ok or an error.
 *
 * Future status:
 *  NABTO_DEVICE_EC_OK   if all data was read.
 *  NABTO_DEVICE_EC_EOF  if only some data was read and the stream is eof.
 *  NABTO_DEVICE_EC_ABORTED if the stream is aborted.
 *  NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if stream is already being read
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_read_all(NabtoDeviceStream* stream, void* buffer, size_t bufferLength, size_t* readLength);

/**
 * Read some bytes from a stream.
 *
 * Read atleast 1 byte from the stream, unless an error occurs or the
 * stream is eof.
 *
 * @param stream        The stream to read bytes from
 * @param buffer        The buffer where bytes is copied to,
 * @param bufferLenght  The length of the output buffer
 * @param readLength    The actual number of read bytes.
 * @return  a future which resolves to ok or a stream error.
 *
 * Future status:
 *  NABTO_DEVICE_OK if some bytes was read.
 *  NABTO_DEVICE_EOF if stream is eof.
 *  NABTO_DEVICE_ABORTED if the stream is aborted.
 *  NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if stream is already being read
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_read_some(NabtoDeviceStream* stream, void* buffer, size_t bufferLength, size_t* readLength);

/**
 * Write bytes to a stream.
 *
 * When the future resolves the data is only written to the stream,
 * but not neccessary acked. This is why it does not make sense to
 * return a number of actual bytes written in case of error since it
 * says nothing about the number of acked bytes. To ensure that
 * written bytes have been acked, a succesful call to
 * nabto_device_stream_close is neccessary after last call to
 * nabto_device_stream_write.
 *
 * @param stream, the stream to write data to.
 * @param buffer, the input buffer with data to write to the stream.
 * @param bufferLenth, length of the input data.
 * @return a future when resolved the data is written to the stream.
 *
 * Future status:
 *  NABTO_DEVICE_OK if write was ok.
 *  NABTO_DEVICE_STREAM_CLOSED if the stream is closed for writing.
 *  NABTO_DEVICE_ABORTED if the stream is aborted.
 *  NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if stream is already being written to
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_write(NabtoDeviceStream* stream, const void* buffer, size_t bufferLength);

/**
 * Close a stream. When a stream has been closed no further data can
 * be written to the stream. Data can however still be read from the
 * stream until the other peer closes the stream.
 *
 * When close returns all written data has been acknowledged by the
 * other peer.
 *
 * @param stream, the stream to close.
 * @return future which resolves when stream is closed or on error
 *
 * Future status:
 *  NABTO_DEVICE_OK if the stream is closed for writing.
 *  NABTO_DEVICE_ABORTED if the stream is aborted.
 *  NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if stream is already being closed
 */

NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_stream_close(NabtoDeviceStream* stream);

/************
 * Coap API *
 ************/
/**
 * Represents the COAP method for requests and responses
 */
typedef enum {
    NABTO_DEVICE_COAP_GET,
    NABTO_DEVICE_COAP_POST,
    NABTO_DEVICE_COAP_PUT,
    NABTO_DEVICE_COAP_DELETE
} NabtoDeviceCoapMethod;

typedef enum  {
    NABTO_DEVICE_COAP_CONTENT_FORMAT_TEXT_PLAIN_UTF8 = 0,
    NABTO_DEVICE_COAP_CONTENT_FORMAT_APPLICATION_OCTET_STREAM = 42,
    NABTO_DEVICE_COAP_CONTENT_FORMAT_APPLICATION_JSON = 50,
    NABTO_DEVICE_COAP_CONTENT_FORMAT_APPLICATION_CBOR = 60
} nabto_device_coap_content_format;

/**
 * The COAP resource is used when notifying observers of a specefic resource
 */
typedef struct NabtoDeviceCoapResource_ NabtoDeviceCoapResource;

/**
 * Representing a COAP request received from the client
 */
typedef struct NabtoDeviceCoapRequest_ NabtoDeviceCoapRequest;

/**
 * Representing a COAP response for a specific request
 */
typedef struct NabtoDeviceCoapResponse_ NabtoDeviceCoapResponse;

/**
 * Resource handling callback invoked when a request is available for the resource
 */
typedef void (*NabtoDeviceCoapResourceHandler)(NabtoDeviceCoapRequest* request, void* userData);

/**
 * Add a COAP resource. Once a COAP resource is added, incoming
 * requests will invoke the handler. The returned resource is alive
 * for the life time of the COAP server
 *
 * @param method     The CoAP method for which to handle requests
 * @param pathSegments
 *
 * The CoAP path segments of the resource. The array of segments is a
 * NULL terminated array of null terminated strings. The familiar
 * notation for rest resources "/heatpump/state" becomes the array
 * {"heatpump", "state", NULL }
 *
 * @param resource  Resource where you can listen for COAP requests
 *
 * TODO more error handling on module errors
 * @return NABTO_DEVICE_EC_OK iff ok.
 *         NABTO_DEVICE_EC_OUT_OF_MEMORY if the resource could not be allocated
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_add_resource(NabtoDevice* device, NabtoDeviceCoapMethod method, const char** pathSegments, NabtoDeviceCoapResource** resource);

// TODO return recurring future
/**
 * Listen for a new coap request on the given resource.
 *
 * @param resource   Resource on which to listen
 * @param request    Where to reference an incoming request
 * @return Future which resolves when a new request is available or an error occurs
 *
 * Future status:
 *   NABTO_DEVICE_EC_OK if request is ready
 *   NABTO_DEVICE_EC_OPERATION_IN_PROGRESS if the resource already has an active listener
 *   NABTO_DEVICE_EC_ABORTED if device is being freed
 *   NABTO_DEVICE_EC_OUT_OF_MEMORY if request was received but the
 *                                structure could not be allocated.
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceFuture* NABTO_DEVICE_API
nabto_device_coap_resource_listen(NabtoDeviceCoapResource* resource, NabtoDeviceCoapRequest** request);

/**
 * Notify observers of a resource retreived by calling
 * nabto_device_coap_add_resource. Notifying observers will trigger an
 * invokation of the resource handler assosiated with the resource,
 * which should ensure a response is made ready.
 *
 * @param resource  The COAP resource to notify
 *
 * @return NABTO_DEVICE_EC_OK if notify was successful
 */
// TODO
//NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
//nabto_device_coap_notify_observers(NabtoDeviceCoapResource* resource);

/**
 * Send back an error.
 *
 * A coap error consists of a status code and an error description in
 * UTF8. If more complex errors needs to be returned they have to be
 * constructed using a response.
 *
 * @param request  The request for which to create a response
 * @param code     The status code for the response in standard HTTP 
 *                 status code format (eg. 200 for success)
 * @param message  zero terminated UTF8 string message
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_error_response(NabtoDeviceCoapRequest* request, uint16_t code, const char* message);


/**
 * Create a COAP response for a given request. This MUST only be
 * called once per resource handler invokation. That is, for
 * non-observable requests ONLY once per request, and for observable
 * requests ONLY once per notification.
 *
 * @param request  The COAP request assosiated with the response
 *
 * @return A representation of the created response. 
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceCoapResponse* NABTO_DEVICE_API
nabto_device_coap_create_response(NabtoDeviceCoapRequest* request);

/**
 * Set the response code of a given response. This code should follow
 * the standard HTTP status codes (eg. 200 for success).
 *
 * @param response  The response for which to set the code
 * @param code      The code to be set
 *
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_response_set_code(NabtoDeviceCoapResponse* response, uint16_t code);

/**
 * Set the payload of a given response.
 *
 * @param response  The response for which to set the payload
 * @param data      The payload to set
 * @param dataSize  The length of the payload in bytes
 *
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_response_set_payload(NabtoDeviceCoapResponse* response, const void* data, size_t dataSize);

/**
 * Set the content format of a given response. This should follow the
 * content format definitions defined by IANA (same as HTTP).
 *
 * @param response  The response for which to set the content format
 * @param format    The format to set
 *
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_response_set_content_format(NabtoDeviceCoapResponse* response, uint16_t format);

/**
 * Mark a response as ready. Once ready, the response will be sent to
 * the client. After this call, both the request and the response will
 * be cleaned up.
 *
 * @param response  The response to be sent
 *
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_response_ready(NabtoDeviceCoapResponse* response);

/**
 * Get the content format of a given request.
 *
 * @param request       The request for which to get the content format
 * @param contentFormat A reference to where to put the content format
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_FAILED on failure
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_request_get_content_format(NabtoDeviceCoapRequest* request, uint16_t* contentFormat);

/**
 * Get the payload of a given request.
 *
 * @param request       The request for which to get the payload
 * @param payload       A reference to where to put the payload reference
 * @param payloadLength A reference to where to put the length of the payload
 *
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_FAILED on failure
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_coap_request_get_payload(NabtoDeviceCoapRequest* request, void** payload, size_t* payloadLength);

/**
 * Get a reference to the underlying connection on which the request
 * was received.
 *
 * @param request  The request to get connection ref from
 * @return Reference to the connection on success
 *         0 on error
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceConnectionRef NABTO_DEVICE_API
nabto_device_coap_request_get_connection_ref(NabtoDeviceCoapRequest* request);

/**
 * Get a parameter from a coap requests. If the parameter does not
 * exist NULL is returned. The lifetime for the returned value is no
 * longer than the lifetime of the NabtoDeviceCoapRequest.
 *
 * @param request       The request to get parameter from
 * @param parameterName zero terminated UTF8 string name of parameter
 * @return reference to parameter value, NULL on errors
 */
NABTO_DEVICE_DECL_PREFIX const char* NABTO_DEVICE_API
nabto_device_coap_request_get_parameter(NabtoDeviceCoapRequest* request, const char* parameterName);

/**************
 * Future API *
 **************/

/**
 * We have made a future api such that it's easier to get all the
 * different async models from a simple standard api.
 *
 * We could have implemented all the future functions for each async
 * function but that would lead to a lot of specialized functions
 * doing almost the same thing.
 */

/**
 * Callback function for resolving futures.
 */
typedef void (*NabtoDeviceFutureCallback)(NabtoDeviceFuture* fut, NabtoDeviceError err, void* data);

/**
 * Free a future.
 */
NABTO_DEVICE_DECL_PREFIX void NABTO_DEVICE_API
nabto_device_future_free(NabtoDeviceFuture* future);

/**
 * Query if a future is ready.
 *
 * @param future, the future.
 * @return NABTO_DEVICE_EC_OK if the future is ready else NABTO_DEVICE_EC_API_FUTURE_NOT_READY
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_future_ready(NabtoDeviceFuture* future);

/**
 * Set a callback to be called when the future resolves
 *
 * @param future   The future instance to set callback on
 * @param callback The function to be called when the future resolves
 * @param data     Void pointer passed to the callback once invoked
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_future_set_callback(NabtoDeviceFuture* future,
                                 NabtoDeviceFutureCallback callback,
                                 void* data);
/**
 * Wait until a future is resolved.
 */
NABTO_DEVICE_DECL_PREFIX void NABTO_DEVICE_API
nabto_device_future_wait(NabtoDeviceFuture* future);

/**
 * Wait atmost duration milliseconds for the future to be resolved.
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_future_timed_wait(NabtoDeviceFuture* future, nabto_device_duration_t duration);

/**
 * Get the error code of resolved future
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_future_error_code(NabtoDeviceFuture* future);

/*************
 * Error API *
 *************/

/**
 * Get message assosiated with an error code
 */
NABTO_DEVICE_DECL_PREFIX const char* NABTO_DEVICE_API
nabto_device_error_get_message(NabtoDeviceError error);

/********
 * Misc *
 ********/

/**
 * Return the version of the nabto embedded library.
 */
NABTO_DEVICE_DECL_PREFIX const char* NABTO_DEVICE_API
nabto_device_version();

/**
 * Free a string allocated by the device.
 */
NABTO_DEVICE_DECL_PREFIX void NABTO_DEVICE_API
nabto_device_string_free(char* str);

/***********
 * Logging *
 ***********/

enum NabtoDeviceLogLevel_ {
    NABTO_DEVICE_LOG_FATAL = 0x00000001ul,
    NABTO_DEVICE_LOG_ERROR = 0x00000002ul,
    NABTO_DEVICE_LOG_WARN  = 0x00000004ul,
    NABTO_DEVICE_LOG_INFO  = 0x00000008ul,
    NABTO_DEVICE_LOG_TRACE = 0x00000010ul
};

typedef enum NabtoDeviceLogLevel_ NabtoDeviceLogLevel;

struct NabtoDeviceLogMessage_ {
    NabtoDeviceLogLevel severity;
    const char* file;
    int line;
    const char* message; /** the message null terminated utf-8 */
};

typedef struct NabtoDeviceLogMessage_ NabtoDeviceLogMessage;

/**
 * Log callback function definition. This function is invoked directly
 * by the core when a log message is to be printed. Since this is
 * called directly from the core, blocking or calling back into the
 * API from this function is not allowed.
 */
typedef void (*NabtoDeviceLogCallback)(NabtoDeviceLogMessage* msg, void* data);

/**
 * Set log callback if custom logging is desired
 *
 * @param device   The device instance to set callback for
 * @param cb       The function to be called on log event
 * @param data     Void pointer passed to the callback when invoked
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_log_set_callback(NabtoDevice* device, NabtoDeviceLogCallback cb, void* data);

/**
 * Set log level of device
 *
 * @param device   The device instance to set level on
 * @param level    The log level to set, available levels are:
 *                 error, warn, info, trace
 * @return NABTO_DEVICE_EC_OK on success
 *         NABTO_DEVICE_EC_INVALID_LOG_LEVEL on invalid level string
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_log_set_level(NabtoDevice* device, const char* level);

/**
 * Set log callback to write logging directly to std out
 *
 * @param device  The device instance to set log callback
 * @return NABTO_DEVICE_EC_OK on success
 */
NABTO_DEVICE_DECL_PREFIX NabtoDeviceError NABTO_DEVICE_API
nabto_device_log_set_std_out_callback(NabtoDevice* device);

/********
 * Util *
 ********/

#ifdef __cplusplus
} // extern c
#endif

#endif
