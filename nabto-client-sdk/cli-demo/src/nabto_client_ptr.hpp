#pragma once

#include <memory>
#include <nabto/nabto_client.h>
#include <nabto/nabto_client_experimental.h>

struct NabtoClientContextFree {
    void operator ()(NabtoClientContext* context) { nabto_client_context_free(context); }
};
typedef std::unique_ptr<NabtoClientContext, NabtoClientContextFree> NabtoClientContextPtr;

struct NabtoClientConnectionFree {
    void operator ()(NabtoClientConnection* connection) { nabto_client_connection_free(connection); } 
};
typedef std::unique_ptr<NabtoClientConnection, NabtoClientConnectionFree> NabtoClientConnectionPtr;

struct NabtoClientStreamFree {
    void operator ()(NabtoClientStream* stream) { nabto_client_stream_free(stream); }
};
typedef std::unique_ptr<NabtoClientStream, NabtoClientStreamFree> NabtoClientStreamPtr;


struct NabtoClientFutureFree {
    void operator ()(NabtoClientFuture* future) { nabto_client_future_free(future); }
};
typedef std::unique_ptr<NabtoClientFuture, NabtoClientFutureFree> NabtoClientFuturePtr;

struct NabtoClientCoapRequestFree {
    void operator ()(NabtoClientCoapRequest* request) { nabto_client_experimental_coap_request_free(request); }
};
typedef std::unique_ptr<NabtoClientCoapRequest, NabtoClientCoapRequestFree> NabtoClientCoapRequestPtr;

struct NabtoClientBufferFree {
    void operator ()(NabtoClientBuffer* buffer) { nabto_client_experimental_buffer_free(buffer); }
};
typedef std::unique_ptr<NabtoClientBuffer, NabtoClientBufferFree> NabtoClientBufferPtr;
