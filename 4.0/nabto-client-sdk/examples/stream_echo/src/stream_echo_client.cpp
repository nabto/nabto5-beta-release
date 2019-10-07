#include "nabto_client.hpp"

#include <cxxopts.hpp>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <thread>

static void reader(std::shared_ptr<nabto::client::Stream> stream);
static void run_stream_echo_client(const std::string& logLevel, const std::string& productId, const std::string& deviceId, const std::string& server, const std::string& serverKey);


int main(int argc, char** argv)
{
    cxxopts::Options options("Stream echo client", "Nabto stream echo client example.");


    options.add_options("Options")
        ("h,help", "Show help")
        ("log-level", "Log level (error|info|trace)", cxxopts::value<std::string>()->default_value(""))
        ("p,product", "Product id", cxxopts::value<std::string>())
        ("d,device", "Device id", cxxopts::value<std::string>())
        ("s,server", "Server url of basestation", cxxopts::value<std::string>())
        ("k,server-key", "Key to use with the server", cxxopts::value<std::string>())
        ;


    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    try {
        run_stream_echo_client(result["log-level"].as<std::string>(),
                               result["product"].as<std::string>(),
                               result["device"].as<std::string>(),
                               result["server"].as<std::string>(),
                               result["server-key"].as<std::string>());
    } catch(...) {
        std::cout << options.help() << std::endl;
        exit(1);
    }
}

class MyLogger : public nabto::client::Logger
{
 public:
    void log(nabto::client::LogMessage message) {
        std::cout << message.getMessage() << std::endl;
    }
};

void run_stream_echo_client(const std::string& logLevel, const std::string& productId, const std::string& deviceId, const std::string& server, const std::string& serverKey)
{
    auto ctx = nabto::client::Context::create();
    if (!logLevel.empty()) {
        ctx->setLogger(std::make_shared<MyLogger>());
        ctx->setLogLevel(logLevel);
    }

    auto connection = ctx->createConnection();
    connection->setProductId(productId);
    connection->setDeviceId(deviceId);
    connection->setServerUrl(server);
    connection->setServerKey(serverKey);
    std::string privateKey = ctx->createPrivateKey();
    connection->setPrivateKey(privateKey);

    try {
        connection->connect()->waitForResult();
    } catch (std::exception& e) {
        std::cout << "Connect failed" << e.what() << std::endl;
        exit(1);
    }

    auto stream = connection->createStream();
    stream->open(42)->waitForResult();

    std::thread t(reader, stream);

    for (;;) {
        char input;
        try {
            std::cin >> input;
        } catch (...) {
            // TODO clean exit
            stream->close()->waitForResult();
            exit(1);
        }
        auto buffer = std::make_shared<nabto::client::BufferImpl>(reinterpret_cast<const unsigned char*>(&input), 1);
        stream->write(buffer)->waitForResult();
    }

    t.join();

}


void reader(std::shared_ptr<nabto::client::Stream> stream)
{
    for (;;) {
        std::shared_ptr<nabto::client::Buffer> buffer = stream->readSome(1024)->waitForResult();
        std::cout << std::string(reinterpret_cast<const char*>(buffer->data()), buffer->size()) << std::endl;
    }
}
