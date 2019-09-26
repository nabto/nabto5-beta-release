# iOS Demo Nabto 5 Demo App

This app demonstrates interacting with the Nabto5 client api from iOS.

When pressing the + button, a new connection is made. A stream is opened towards a stream echo Nabto embedded server and a few bytes are written to the stream. Once the echo response is received, the latency is written to a table.

Very rudimentary implementation, e.g. with insufficient cleanup and resource deallocation - and all device parameters are just embedded in source code (in the `nabtoDemoConnect` function in MasterViewController.mm). So make sure to change the parameters to match your device. The Embedded SDK cli-demo parameters matching the parameters in the source code in the iOS app look as follows:

```
./test_device  -p pr-wrukmj9j -d de-jwmawg7z -k ug-device.pem --hostname nabto5.dev.nabto.net
```

Parameters are just shown for reference to compare with the iOS app source code, you will need to replace parameters with your own that are registered in the Nabto Cloud console with the fingerprint of your own public key.

Note that the 10 roundtrips measured often have quite high latencies: The "ping" requests are sent immediately after connect, meaning that a P2P connection has not yet been established and relay is used (this is the Nabto5 QuickConnect feature). If continuing to send requests or if waiting a bit, latencies will become lower (if P2P is possible to establish).
