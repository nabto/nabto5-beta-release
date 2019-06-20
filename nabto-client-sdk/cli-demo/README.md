# Nabto client SDK CLI demo
Nabto client SDK demo for Linux and MacOS command lines. The
`src/test_client.c` file implements all the Nabto Client
functionallities. The `src/nabto_client_ptr.hpp` file is used to wrap
the c pointers of the client SDK to c++ smart pointers, while
`src/cxxopts.hpp` is used to parse command line arguments. The Nabto
client SDK itself is provided through libraries located in the `lib`
folder. The accompanying header files are located in `include/nabto/`.

## Building the demo application
The demo application can be build using cmake as shown here:

```
mkdir build
cd build
cmake ../src
make -j
```

This produces an binary called `test_client`. It is likely necessary to set the dynamic libray load path to find the Nabto client SDK libs before running. On macOS this is done as follows:

```
# assumes libnabto_client.dylib is located here
export DYLD_LIBRARY_PATH=../../cli-demo/lib
```

on Linux:

```
# assumes libnabto_client.so is located here
export LD_LIBRARY_PATH=../../cli-demo/lib
```

This must be done before running the demo executable.


## Running the demo application
Before running the Nabto client demo, ensure you have a Nabto embedded
SDK device running and attached to the Nabto basestation as described
in `nabto-embedded-sdk` folder of this repository.

Running the CLI demo client requires six arguments: a product ID, a
device ID, a server api key, a host name, a path to a file containing
a private key, and a test argument. The product ID, device ID, and
server api key must be obtained through the Nabto cloud console
(https://console.cloud.dev.nabto.com), and must match the device it is
desired to connect to. The host name should be
`https://a.clients.dev.nabto.net`. The private key file can be
generated using openssl with the following command:

```
openssl ecparam -genkey -name prime256v1 -out key.pem
```

The final parameter determines which functionallity to
showcase. Currently, four values are accepted:

 * `stream-echo`: Opens a stream to the device on which any keyboard
   inputs are send. The device will echo any input back to the client.
 * `stream-ping`: Opens a stream to the device and sends a ping
   message to the device every second which the device echoes back to
   the client.
 * `coap-get`: Sends a CoAP GET request to the device which responds
   with the `helloWorld` string.
 * `coap-post`: Sends a CoAP POST request to the device which responds
   with the `helloWorld` string.

An example command to start the client is shown below, be sure to
replace the Product ID, Device ID, and server api key with those
obtained through the Nabto cloud console.

```
./test_client -H https://a.clients.dev.nabto.net -d de-tsxiqkt9 -p pr-3tayzujn -s sk-e918d713537077c409450a723cc06da7 -k key.pem -t stream-echo
```
