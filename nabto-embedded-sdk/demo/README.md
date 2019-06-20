# Nabto embedded SDK demo
The Nabto embedded SDK demo device demonstrates how to start the Nabto
embedded SDK and use it to stream data between it and a client. The
demo also shows how to invoke RPC using CoAP. The demo is fully
implemented in the `demo/test_device.c` file. The files located in
`demo/gopt/` are only used to parse command line arguments. The Nabto
embedded SDK itself is provided through libraries located in
`lib`. Libraries are provided for Linux and MacOs. The accompanying
header file is located in `include/nabto/`

## Building the demo device
The demo device can be build using cmake as shown here:

```
mkdir build
cd build
cmake ../src
make -j
```

This produces an binary called `test_device`

## Running the demo device
Running the nabto demo device requires four arguments, a product ID, a
device ID, a host name, and a private key. The product ID and device
ID must be generated through the nabto cloud console
(https://console.cloud.dev.nabto.com). As the host name
`a.devices.dev.nabto.net` should be used. The private key can be
generated using openssl with the following command:

```
openssl ecparam -genkey -name prime256v1 -out key.pem
```

Once the key is generated, the device can be started using the
following command, replacing the product ID and device ID with the
ones generated through the nabto cloud console.

```
./test_device -p pr-3tayzujn -d de-tsxiqkt9 --hostname a.devices.dev.nabto.net -k key.pem
```

When the device starts, the first line printed shows the fingerprint
assosiated with the provided key. This fingerprint must be registered
for the device in the Nabto cloud console. Once the fingerprint is
registered, restart the device using the same command again. The
device should now attach successfully to the Nabto basestation.

Once the device has successfully attached, the Nabto client SDK demo
from this repository can be used to connect to the device.
