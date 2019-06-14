# Nabto 5 Beta2 Release
This repo captures artifacts, documentation and issues for the Nabto5 Beta1 release.

## Deliverables in the seconds beta release

* Embedded SDK for building device applications. Delivered as precompiled libraries for Linux and macOS in this release; full source will be provided later.
* Client SDK for building client apps. Delivered as precompiled libraries now and in the future. Currently libs for iOS, Android, macOS and Linux are provided (see known limitations below).
* Cloud Console for solution management.
* Basestation with full distributed backend infrastructure, ie no stub functionality as in Alpha.
* Embedded device CLI demo (full source code).
* Client app CLI demo (full source code).
* iOS app (full source and project).

Note the iOS static lib is delivered as a bzip2 archive due to its size, so unpack in `nabto-client-sdk/lib` before use.

## Improvements over Beta 1 release

* Error Codes in the client is implemented
* Logging in the client is implemented
* Local discovery functionality is addded to the client.
* Documented local discovery functionality.
* Several fixes of stuff in various parts.

## Limitations and known issues in the second beta release

* The device is unstable.
* No association between console users and domains - all console users have access to all Nabto5 entities created in the system. So this means that you should not add any potentially business sensitive information as e.g. product names.
* API error codes in the device is not fully implemented. For now, just check for `NABTO_DEVICE_OK` when checking for success, detailed error codes when status is not OK cannot yet be fully trusted (but will be different than the OK codes in case of error).
* Documentation is limited to annotation in header files.
* Memory leak in iOS client library when establishing a new connection.
* Device id and product id for embedded devices is not validated in the basestation during attach (against the values entered in the console), only the device's public key is validated.
* Timeouts in some situations where an explicit immediate error should be reported instead, for instance if connecting to an offline device.

Note on iOS clients: Your project must include a file with extension .mm to trigger Objective C++ builds, in turn to trigger the C++ runtime to be linked into the applications, needed by the Nabto static client library. For future releases, a higher level wrapper than what is currently provided will ensure this. The clang's libc++ runtime must be chosen (normally the default), not GCC's (libstdc++).

## Getting started

To get started, the following path is suggested (outlined in detail in the demo READMEs):

1. Obtain access to the Nabto Cloud Console. Described below.
2. Setup a solution in Nabto Cloud. Described below.
3. Build and run an embedded device demo. Described in the [embedded SDK demo README](nabto-embedded-sdk/demo/README.md)
4. build and run a client demo app towards the device. Described in the client SDK [CLI demo README](nabto-client-sdk/cli-demo/README.md) and the [iOS app demo README](nabto-client-sdk/ios-demo/README.md)

The SDK header files are annotated with documentation, useful for understanding the API interaction in the demos:

* [Embedded SDK header file](nabto-embedded-sdk/include/nabto/nabto_device.h)
* [Client SDK header file](nabto-client-sdk/include/nabto/nabto_client.h)

# Console Access

To use the Nabto Cloud console in its current incarnation (ie, without a signup feature), write to ug@nabto.com to get an account prepared. The account will be created on the development cloud system. Ideally, for confidientiality, credentials should be exchanged using secure communication, e.g. GPG encrypted mail or e.g. Signal IM. Given that the accounts will not be used for production, it is acceptable with some less strict exchange of credentials, though.

Access to the console requires the Google Authenticator app installed on a mobile device.

Note that all functionality in the Console application will be made available through a REST API for integration in your own device management solution.

Note the important isolation limitation from the limitations section in this document.

## Products

Creating a new product in the console is the first step towards running a device and client. Just click "Products" in the menu on the left and then "New Product" and save.


## Devices

Devices must be added through the console to obtain a device id and register a public key fingerprint, in turn allowing the device to later attach (register with the Nabto basestation). In the product overview, a new device can be created by clicking the "Devices" link for a given product and then clicking "Create New Device".

In the "Fingerprint" field of the new device, enter the public key fingerprint for the keypair to use with the device in question. See [the embedded demo](nabto-embedded-sdk/demo/README.md) for info on how to create this keypair and obtain the fingerprint.

## Client apps

Client apps need an api key. For a given product, multiple client apps can be created. Click the "Apps" link in the product overview and click the "New App" button.
