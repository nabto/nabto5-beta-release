#include "nabto_client.hpp"
#include "json_config.hpp"

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using json = nlohmann::json;

enum {
  COAP_CONTENT_FORMAT_APPLICATION_CBOR = 60
};

class MyLogger : public nabto::client::Logger
{
 public:
    void log(nabto::client::LogMessage message) {
        std::cout << message.getMessage() << std::endl;
    }
};

void my_handler(int s){
    printf("Caught signal %d\n",s);
}



std::shared_ptr<nabto::client::Connection> createConnection(const std::string& configFile)
{
    json config;
    if(!json_config_load(configFile, config)) {
        std::cerr << "Could not read config file" << std::endl;
        exit(1);
    }

    auto ctx = nabto::client::Context::create();
    ctx->setLogger(std::make_shared<MyLogger>());
//    ctx->setLogLevel("trace");
    auto connection = ctx->createConnection();
    connection->setProductId(config["ProductId"].get<std::string>());
    connection->setDeviceId(config["DeviceId"].get<std::string>());
    connection->setServerUrl(config["ServerUrl"].get<std::string>());
    connection->setServerKey(config["ServerKey"].get<std::string>());
    connection->setPrivateKey(config["PrivateKey"].get<std::string>());
    try {
        connection->connect()->waitForResult();
    } catch (std::exception& e) {
        std::cerr << "Connect failed" << e.what() << std::endl;
        exit(1);
    }

    return connection;
}

void tcptunnel(const std::string& configFile, uint16_t localPort, const std::string& remoteHost, uint16_t remotePort)
{
    std::cout << "Creating tunnel " << configFile << " local port " << localPort << " remote host " << remoteHost << " remote port " << remotePort << std::endl;

    auto connection = createConnection(configFile);

    auto tunnel = connection->createTcpTunnel();
    tunnel->open(localPort, remoteHost, remotePort)->waitForResult();
    std::cout << "tunnel is opened" << std::endl;

    // wait for ctrl c
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    pause();

    connection->close();
}

void tcptunnel_pairing(const std::string& configFile, const std::string& productId, const std::string& deviceId, const std::string& server, const std::string& serverKey, const std::string& password)
{
    json config;
    std::cout << "Pairing with tcp tunnel " << productId << "." << deviceId << std::endl;
    auto ctx = nabto::client::Context::create();
    auto connection = ctx->createConnection();
    connection->setProductId(productId);
    connection->setDeviceId(deviceId);
    connection->setServerUrl(server);
    connection->setServerKey(serverKey);

    std::string privateKey = ctx->createPrivateKey();
    connection->setPrivateKey(privateKey);

    config["ProductId"] = productId;
    config["DeviceId"] = deviceId;
    config["ServerUrl"] = server;
    config["ServerKey"] = serverKey;
    config["PrivateKey"] = privateKey;

    try {
        connection->connect()->waitForResult();
    } catch (std::exception& e) {
        std::cout << "Connect failed" << e.what() << std::endl;
        exit(1);
    }

    std::cout << "Connected to device with fingerprint " << connection->getDeviceFingerprintHex() << std::endl;
    std::cout << "Is the fingerprint valid [yn]" << std::endl;

    for (;;) {
        char input;
        std::cin >> input;
        if (input == 'y') {
            break;
        }
        if (input == 'n') {
            std::cout << "Fingerprint not accepted, quitting" << std::endl;
            exit(1);
        }
        std::cout << "only y or n is accepted as answers" << std::endl;
    }

    auto coap = connection->createCoap("POST", "/pairing/password");

    json pwd = password;
    auto cbor = std::make_shared<nabto::client::BufferImpl>(json::to_cbor(pwd));
    coap->setRequestPayload(COAP_CONTENT_FORMAT_APPLICATION_CBOR, cbor);

    coap->execute()->waitForResult();

    if (coap->getResponseStatusCode() == 205)
    {
        std::cout << "Paired with the device, writing configuration to disk" << std::endl;
        if (!json_config_save(configFile, config)) {
            std::cerr << "Failed to write config to " << configFile << std::endl;
        }
        exit(0);
    } else {
        std::string reason;
        auto buffer = coap->getResponsePayload();
        reason = std::string(reinterpret_cast<char*>(buffer->data()), buffer->size());
        std::cout << "Could not pair with the device status: " << coap->getResponseStatusCode() << " " << reason << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv)
{
    cxxopts::Options options("Tunnel client", "Nabto tunnel client example.");

    options.add_options("General")
        ("h,help", "Show help")
        ("password-pairing", "Do a pairing with the device using a password")
        ("tcptunnel", "Create a tcp tunnel with the device.")
        ("c,config", "Configutation File", cxxopts::value<std::string>()->default_value("tcptunnel_client.json"))
        ;

    options.add_options("Pairing")
        ("p,product", "Product id", cxxopts::value<std::string>())
        ("d,device", "Device id", cxxopts::value<std::string>())
        ("s,server", "Server url of basestation", cxxopts::value<std::string>())
        ("k,server-key", "Key to use with the server", cxxopts::value<std::string>())
        ("password", "Password to use in the pairing with the device", cxxopts::value<std::string>())
        ;

    options.add_options("TCPTunnelling")
        ("local-port", "Local port to bind tcp listener to", cxxopts::value<uint16_t>()->default_value("0"))
        ("remote-host", "Remote ip to connect to", cxxopts::value<std::string>()->default_value(""))
        ("remote-port", "Remote port to connect to", cxxopts::value<uint16_t>()->default_value("0"));

    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("password-pairing")) {
        try {
            tcptunnel_pairing(result["config"].as<std::string>(),
                              result["product"].as<std::string>(),
                              result["device"].as<std::string>(),
                              result["server"].as<std::string>(),
                              result["server-key"].as<std::string>(),
                              result["password"].as<std::string>());
        } catch(...) {
            std::cout << "Missing required option" << std::endl;
            std::cout << options.help() << std::endl;
            exit(1);
        }
    } else if (result.count("tcptunnel")) {
        try {
            tcptunnel(result["config"].as<std::string>(),
                      result["local-port"].as<uint16_t>(),
                      result["remote-host"].as<std::string>(),
                      result["remote-port"].as<uint16_t>());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            exit(1);
        }
    } else {
        std::cout << "Missing mode option" << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }
}
