# Local Discovery

Local discovery is done using the mDNS protocol.

The device implements an mDNS server which advertises the nabto
service on a port number provided by the Nabto core.

The mDNS server must answer mDNS PTR queries for `_nabto._udp.local`

The response must have a PTR record to a unique domain name eg:
`xyzabc._nabto._udp.local` 

The response must also have a TXT record containing the device ID and
product ID: `deviceid=<deviceId>` and `productid=<productId>` as per
RFC1464, the attribute name is case-insentitive.

The response must also have a SRV record for the device name.

In total the response should contain at least 3 Resource records:
 * PTR record which resolves `_nabto._udp.local` to
   `<UUID>._nabto._udp.local`
 * TXT record with the device ID and product ID
 * SRV record which resolves `<UUID>._nabto._udp.local` to the port
   number provided by the Nabto core and the device name

Additionally, the response record can contain an A and/or AAAA record
resolving the device name to the IPs of the device.

Since the domain name of the service must be unique, we will simplify
the mDNS implementation to not verify this at run time (though the RFC
states that we should).

The device should use a TTL of at least 3600s (1hour).
 
The client implements an mDNS client capable of performing the mDNS
PTR queiries for `_nabto._udp.local`. It should then decode the
response, and if te A and/or AAAA records are not in the response, it
must be able to resolve the device name to its IP addresses. The list
of addresses, the port number, device ID, and product ID should then
be passed on to the Nabto client SDK.

Given the Nabto usecase and the device TTL of at least 1hour, we will
make the simplifying assumption that chache maintainance is
irrelevant.
