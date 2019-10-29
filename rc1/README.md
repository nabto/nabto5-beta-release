# Nabto 5 Release Candidate 1

## Breaking changes
### Embedded SDK
* Most error codes have been renamed
* `nabto_device_free()` is split into 2 so `nabto_device_stop()`
  must now be called before free. Now free simply frees the
  resources.
* `nabto_device_experimental_util_create_private_key()` renamed to
  `nabto_device_create_private_key()`
* `nabto_device_stream_listen()` removed. Now use
  `nabto_device_stream_init_listener()`, then
  `nabto_device_listener_new_stream()` when ready to wait for a
  new stream.
* Asynchronous functions no longer return a future, instead a
  future is pre-allocated by the user with
  `nabto_device_future_new()` and passed as an argument to the
  asynchronous function. This allows a failing asynchronous
  functions to always fail by resolving the future providing more
  streamlined error handling.
* `nabto_device_coap_add_resource()` removed. COAP is now used in
  the same way as streams where you initialize a listener and use
  it to wait for incoming requests. With this pattern, the
  `NabtoDeviceListener` replaces the `NabtoDeviceCoapResource`.
* `NabtoDeviceCoapResponse` is removed and is now allocated with
  the request. This removes the need for
  `nabto_device_coap_create_response()`. This also means that all
  functions like `nabto_device_coap_response_*` now takes the
  request instead of the now removed response structure.
* Incoming COAP requests must now be freed by the user with
  `nabto_device_coap_request_free()`.
* Functions for resolving future now returns the error code of the
  future when it resolves. Affected functions are
  `nabto_device_future_wait()`,
  `nabto_device_future_timed_wait()`,
  `nabto_device_future_ready()`.
* `nabto_device_future_set_callback()` cannot fail. Therefore, it
  now returns void.
* `nabto_device_log_set_*` functions renamed to `nabto_device_set_log_*`

### Client SDK
* Most error codes have been stream lined.
* `nabto_client_free()` is split into stop and free.
* Futures have been slightly changed in the way they are used.
* Some function have changed name slightly
* Fingerprints from a connection are returned differently.

## Deliverables in this release
* JWT support in the basestation and in the client.
* Listener API for recurring events like new streams or coap requests.
* Listen for connection events and device events using new listener API.


## Improvements over last release
* Many issues with properly closing the software was fixed
* The APIs have been streamlined

## Limitations and known issues in this release
### General
* Documentation is limited to annotation in header files.
* The headers should be considered stable, we will still work on
      several improvements in the code, before the final release.

### Embedded SDK
* When the embedded SDK is not closed nicely, the flow should be
  to call `nabto_device_stop()` which will block untill the core
  is aborted, then `nabto_device_free()` should be
  called. Currently, `nabto_device_stop()` will hang
  indefinately if `nabto_device_close()` is not called
  first. This means the device currently only supports being
  closed nicely.
* Under certain circumstances, embedded SDK streams can leak
  stream segments. (This cannot happen with tunnel streams.)
* Some corner case errors are not handled in the embedded SDK,
  mostly these are out-of-memory errors.
* Device events can be used to determine when a device becomes
  attached/detached from the basestation. However, it is not
  final which other events the device should emit. Therefore,
  this functionallity remains in the experimental header.
* Changes in this release means it is no longer necessary for the
  IAM module to be embedded as deeply into the core as it
  is. Therefore, the IAM module remains in the experimental
  header.
* The creation of TCP tunnels is restricted through the IAM
  module. This lacks documentation and the IAM module is subject
  to change. Therefore, the TCP tunnelling remains in the
  experimental header.

### Client SDK
* The client api does not yet have a stable api for detecting closed connections.
* TCP tunnels is still in experimental.

### Examples
    * The heatpump and tunnel client examples does not validate device fingerprints on reconnect.


## Getting started

To get started using this release, first build both SDKs, then try
each example.

### building embedded SDK
```
mkdir nabto-embedded-sdk/build
cd nabto-embedded-sdk/build
cmake ..
make -j
```

### building client SDK
```
mkdir nabto-client-sdk/build
cd nabto-client-sdk/build
cmake ..
make -j
```

### Note on local connections
When running the examples, an App Server Key is required on the client,
and the device fingerprint must be registered on the basestation. If
the App Server Key is invalid, the client will not be allowed access
to the basestation, similarly, if the device fingerprint is invalid
the device will not be allowed to attach to the basestation. In this
scenario, the client would still be able to discover and connect to
the device if they are on the same local network using mDNS.

### Running the TCP tunnel example
Go to the Nabto cloud console and retrieve a Product ID, a Device ID,
and an App Server Key. Through this example, whereever it says
`<productId>`, `<deviceId>`, `<serverKey>` insert the values you
retrieved.

From the device build directory, initialize the device:

```
./examples/tcptunnel/tcptunnel_device --init -p <productId> -d <deviceId> -s a.devices.dev.nabto.net
```

This creates a new keypair for your tunnel and prints the
fingerprint of the public key. This fingerprint must be configured for the device in the
Nabto Cloud Console. The `init` command also creates a file called
`tcptunnel_device.json` with the configuration of your new device. If
you already have a keypair with the public key fingerprint configured in the
Nabto Cloud Console, you can replace the created device keypair in
the JSON file.

Once your device is configured, it can be started with:

```
./examples/tcptunnel/tcptunnel_device
```

The device should now print the line:

```
16:57:08:319 ...sdk/src/core/nc_device.c(093)[_INFO] Device is now attached
```

You should now be able to connect to your device using the example
client. First the client must be paired with the device. From the
client build directory run:

```
./examples/tcptunnel/tcptunnel_client --password-pairing --password secret123 -k <serverKey> -p <productId> -d <deviceId> -s https://a.clients.dev.nabto.net
```

For now, the password is set to `secret123` in the device, which
should obviously only be used for testing purposes. Once your client
is paired with the device, a TCP tunnel can be opened between the
two. For this example we tunnel port 80 from the device host to port
4242 on the client host. Allowing you to access a webserver on the
device by connecting to `127.0.0.1:4242` on the client host.

```
./examples/tcptunnel/tcptunnel_client --tcptunnel --local-port 4242 --remote-port 80 --remote-host 127.0.0.1
```

### Running the Heat pump example
Similarly to the TCP tunnel example, you first need a Product ID, a
Device ID and an App Server Key from the Nabto Cloud Console. Also
like the TCP tunnel, the device must first be initialized to generate
the `heat_pump_device.json` configuration file and a keypair:

```
./examples/heat_pump/heat_pump_device --init -d <deviceId> -p <productId> -s a.devices.dev.nabto.net
```

The device fingerprint must be configured in the Nabto Cloud Console,
after wich the device can be started:

```
./examples/heat_pump/heat_pump_device
```

Again your client must be paired with your new device before it can be
accessed. From the client build directory run:

```
./examples/heat_pump/heat_pump_client --pair -p <productId> -d <deviceId> -k <serverKey> -s https://a.clients.dev.nabto.net
```

This example shows a different pairing approach. Firstly, the client
promts you to accept the fingerprint of the device is correct before
pairing. To accept press `y` followed by enter. Once the device
fingerprint is accepted, the client will attempt to pair with the
device. This causes the device to make a similar prompt for the user
to accept the fingerprint of the client. To accept press `y` followed
by enter. This pairing pattern emulates pressing a physical button on
your embedded device to ensure only someone with physical access to
the device can be allowed to pair.

Once your client is paired with the device, several COAP requests can
be made using the client. A full list of queries can be found in the
with the command line option `--help`. For example, the simulated
heatpump can be turned on by the following command:

```
./examples/heat_pump/heat_pump_client --set-power ON
```

A few other examples could be:

```
./examples/heat_pump/heat_pump_client --set-target 24
./examples/heat_pump/heat_pump_client --get
./examples/heat_pump/heat_pump_client --users-list
./examples/heat_pump/heat_pump_client --users-get --user User-0
```


The heat pump example also shows how the internal MDNS client can be
used to scan for devices on the local network. This can be done using
the below command. This does not require the client to be paired.

```
./examples/heat_pump/heat_pump_client --scan
```
