## Nabto5 beta 3 release

This is the beta 3 release of nabto5. This release does primarily contain an IAM system.

## New Features, since beta 2 release:

  * An IAM system for embedded devices based policies and attributes.
  * A nearly complete heat pump demo which shows an iot application, built using nabto5.
  * A mdns server such that it's trivial to build local discovery for devices on systems without a mdns server.
  * A preliminary experimental mdns client, this will need another iteration to be really usefull.
  * Organizations in our cloud console.


## Improvements over Beta 2 release

  * Several memory leaks has been fixed. But we do still have some in the streaming implementation.
  * Updated android wrappers.

## Limitations and known issues in the second beta release

  * API error codes in the device is not fully implemented. For now, just check for `NABTO_DEVICE_OK` when checking for success, detailed error codes when status is not OK cannot yet be fully trusted (but will be different than the OK codes in case of error).
  * Documentation is limited to annotation in header files.
  * Device id and product id for embedded devices is not validated in the basestation during attach (against the values entered in the console), only the device's public key is validated.
  * There are still a few memory leaks.

## Getting started

TDB
