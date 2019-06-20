#include <nabto/nabto_client.h>
#include <nabto/nabto_client_experimental.h>

#include "nabto_client_ptr.hpp"

#include "cxxopts.hpp"

#include <string>
#include <iostream>

#ifdef WIN32
#include "windows.h"
#else
#include <unistd.h>
#endif

#define MAX_KEY_PEM_SIZE 1024

void die(std::string msg, cxxopts::Options options) {
    std::cout << msg << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    exit(1);
}

void close_connection(NabtoClientConnectionPtr connection) {
    NabtoClientFuturePtr closeFuture(nabto_client_connection_close(connection.get()));
    nabto_client_future_wait(closeFuture.get());

    NabtoClientError ec = nabto_client_future_error_code(closeFuture.get());
    if (ec != NABTO_CLIENT_OK) {
        std::cerr << "could not close connection " << nabto_client_error_get_message(ec) << std::endl;
    }
    std::cout << "freed connection" << std::endl;
}

void stream_echo(NabtoClientConnectionPtr connection) {
    NabtoClientStreamPtr stream(nabto_client_stream_new(connection.get()));

    NabtoClientFuturePtr streamOpenFuture(nabto_client_stream_open(stream.get(), 4242));
    nabto_client_future_wait(streamOpenFuture.get());

    std::vector<uint8_t> output(2048);
    size_t transferred;
    std::cout << "Echo client, input data to get it echoed" << std::endl;
    std::cout << "Typing quit, exit, or q exits the client" << std::endl;

    while (true) {
        std::string input;
        std::cin >> input;

        if (input == "quit" || input == "exit" || input == "q") {
            break;
        } else {
            transferred = 0;
            NabtoClientFuturePtr streamWriteFuture(nabto_client_stream_write(stream.get(),
                                                                             input.data(),
                                                                             input.size()+1));
            NabtoClientFuturePtr streamReadFuture(nabto_client_stream_read_all(stream.get(),
                                                                               output.data(),
                                                                               input.size()+1,
                                                                               &transferred));
            nabto_client_future_wait(streamWriteFuture.get());
            nabto_client_future_wait(streamReadFuture.get());

            if (transferred > 0) {
                std::cout << std::string(reinterpret_cast<const char*>(output.data()), transferred) << std::endl;
            }
        }
    }
    NabtoClientFuturePtr streamCloseFuture(nabto_client_stream_close(stream.get()));
    nabto_client_future_wait(streamCloseFuture.get());

    std::cout << "Stream closed" << std::endl;
    close_connection(std::move(connection));
}

void stream_ping(NabtoClientConnectionPtr connection) {
    NabtoClientStreamPtr stream(nabto_client_stream_new(connection.get()));

    NabtoClientFuturePtr streamOpenFuture(nabto_client_stream_open(stream.get(), 4242));
    nabto_client_future_wait(streamOpenFuture.get());

    std::vector<uint8_t> output(2048);
    size_t transferred;
    std::string input = "ping";
    std::cout << "pinging" << std::endl;
    while (true) {
        transferred = 0;
        NabtoClientFuturePtr streamWriteFuture(nabto_client_stream_write(stream.get(),
                                                                         input.data(),
                                                                         input.size()+1));
        NabtoClientFuturePtr streamReadFuture(nabto_client_stream_read_all(stream.get(),
                                                                           output.data(),
                                                                           input.size()+1,
                                                                           &transferred));
        nabto_client_future_wait(streamWriteFuture.get());
        nabto_client_future_wait(streamReadFuture.get());

        if (transferred > 0) {
            std::cout << std::string(reinterpret_cast<const char*>(output.data()), transferred) << std::endl;
        }
#ifdef WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }
    NabtoClientFuturePtr streamCloseFuture(nabto_client_stream_close(stream.get()));
    nabto_client_future_wait(streamCloseFuture.get());

    std::cout << "Stream closed" << std::endl;
    close_connection(std::move(connection));
}

void coap_get(NabtoClientConnectionPtr connection) {
    std::cout << "Sending CoAP GET request" << std::endl;
    NabtoClientCoapRequestPtr request(nabto_client_experimental_coap_request_new(connection.get(), NABTO_CLIENT_COAP_GET, "/test/get"));

    NabtoClientFuturePtr responseFuture(nabto_client_experimental_coap_execute(request.get()));
    nabto_client_future_wait(responseFuture.get());
    uint16_t statusCode;
    nabto_client_experimental_coap_response_get_status_code(request.get(), &statusCode);
    if (statusCode != 205) {
        std::cout << "coap error: " << statusCode << std::endl;
        close_connection(std::move(connection));
        return;
    }
    NabtoClientBufferPtr buffer(nabto_client_experimental_coap_response_get_payload(request.get()));
    std::string responseData((const char*)nabto_client_experimental_buffer_data(buffer.get()), nabto_client_experimental_buffer_size(buffer.get()));
    std::cout << "Received CoAP response data: " << responseData << std::endl;

    close_connection(std::move(connection));
}

void coap_post(NabtoClientConnectionPtr connection) {
    std::string coapPostData = "Example post data";
    std::cout << "Sending CoAP POST request" << std::endl;
    NabtoClientCoapRequestPtr request(nabto_client_experimental_coap_request_new(connection.get(), NABTO_CLIENT_COAP_POST, "/test/post"));

    nabto_client_experimental_coap_request_set_payload(request.get(), NABTO_CLIENT_COAP_CONTENT_FORMAT_TEXT_PLAIN_UTF8, (void*)coapPostData.c_str(), coapPostData.length());

    nabto_client_experimental_coap_request_set_accept(request.get(), NABTO_CLIENT_COAP_CONTENT_FORMAT_TEXT_PLAIN_UTF8);
    NabtoClientFuturePtr responseFuture(nabto_client_experimental_coap_execute(request.get()));
    nabto_client_future_wait(responseFuture.get());
    uint16_t statusCode;
    nabto_client_experimental_coap_response_get_status_code(request.get(), &statusCode);
    if (statusCode != 205) {
        std::cout << "coap error: " << statusCode << std::endl;
        close_connection(std::move(connection));
        return;
    }
    NabtoClientBufferPtr buffer(nabto_client_experimental_coap_response_get_payload(request.get()));
    std::string responseData((const char*)nabto_client_experimental_buffer_data(buffer.get()), nabto_client_experimental_buffer_size(buffer.get()));
    std::cout << "Received CoAP response data: " << responseData << std::endl;

    close_connection(std::move(connection));

}

int main(int argc, char** argv) {
    std::string host;
    std::string deviceId;
    std::string productId;
    std::string serverKey;
    std::string privateKey(MAX_KEY_PEM_SIZE, '\0');
    std::string testType;

    try
    {
        cxxopts::Options options(argv[0], "Nabto 5 test client");
        options.add_options()
            ("H,hostname", "URL for the Nabto basestation", cxxopts::value<std::string>())
            ("d,deviceid", "Device ID to connect to", cxxopts::value<std::string>())
            ("p,productid", "Product ID to use", cxxopts::value<std::string>())
            ("s,serverkey", "Server key of the app", cxxopts::value<std::string>())
            ("k,keyfile", "Path to file containing a private key", cxxopts::value<std::string>())
            ("t,testtype", "test to run (values: stream-echo, stream-ping, coap-get, coap-post", cxxopts::value<std::string>())
            ("h,help", "Shows this help text");
        options.parse(argc, argv);

        if (options.count("help")) {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(0);
        }

        if(options.count("hostname")) {
            host = options["hostname"].as<std::string>();
        } else {
            die("no hostname provided", options);
        }

        if(options.count("deviceid")) {
           deviceId = options["deviceid"].as<std::string>();
        } else {
            die("no device ID provided", options);
        }

        if(options.count("productid")) {
            productId = options["productid"].as<std::string>();
        } else {
            die("no product ID provided", options);
        }

        if(options.count("serverkey")) {
            serverKey = options["serverkey"].as<std::string>();
        } else {
            die("no server key provided", options);
        }

        if(options.count("testtype")) {
            testType = options["testtype"].as<std::string>();
        } else {
            die("no test type provided", options);
        }

        if(options.count("keyfile")) {
            FILE* f;
            f = fopen(options["keyfile"].as<std::string>().c_str(), "r");
            if (f == NULL) {
                die("Key file not found", options);
            }
            fread(&privateKey[0], 1, MAX_KEY_PEM_SIZE, f);
            fclose(f);
        } else {
            die("no key file provided", options);
        }
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        exit(1);
    }

    std::cout << "connecting to " << productId << "." << deviceId << " using client_lb_host: " << host << std::endl;

    NabtoClientContextPtr context(nabto_client_context_new());

    NabtoClientConnectionPtr connection(nabto_client_connection_new(context.get()));
    std::cout << "created new connection" << std::endl;

    NabtoClientError ec;
    ec = nabto_client_connection_set_server_url(connection.get(), host.c_str());
    ec = nabto_client_connection_set_server_api_key(connection.get(), serverKey.c_str());

    ec = nabto_client_connection_set_product_id(connection.get(), productId.c_str());
    ec = nabto_client_connection_set_device_id(connection.get(), deviceId.c_str());

    ec = nabto_client_connection_set_private_key(connection.get(), privateKey.c_str());
    NabtoClientFuturePtr connectFuture(nabto_client_connection_connect(connection.get()));

    nabto_client_future_wait(connectFuture.get());

    ec = nabto_client_future_error_code(connectFuture.get());

    if (ec != NABTO_CLIENT_OK) {
        std::cerr << "could not connect to device " << nabto_client_error_get_message(ec) << std::endl;
    } else {
        char fingerprint[32];

        ec = nabto_client_connection_get_device_fingerprint_hex(connection.get(), fingerprint);
        if (ec != NABTO_CLIENT_OK) {
            std::cerr << "could not get remote peer fingerprint" << std::endl;
        } else {
            std::cout << "Connected to device with fingerprint: " << std::string(fingerprint, 32) << std::endl;
        }
    }

    if (testType == "stream-echo") {
        stream_echo(std::move(connection));
    } else if (testType == "stream-ping") {
        stream_ping(std::move(connection));
    } else if (testType == "coap-get") {
        coap_get(std::move(connection));
    } else if (testType == "coap-post") {
        coap_post(std::move(connection));
    } else {
        std::cerr << "Invalid test type provided" << std::endl;
        NabtoClientFuturePtr closeFuture(nabto_client_connection_close(connection.get()));
        nabto_client_future_wait(closeFuture.get());

        NabtoClientError ec = nabto_client_future_error_code(closeFuture.get());
        if (ec != NABTO_CLIENT_OK) {
            std::cerr << "could not close connection " << nabto_client_error_get_message(ec) << std::endl;
        }
        std::cout << "freed connection" << std::endl;
    }


}
