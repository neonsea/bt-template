# Bluetooth Template

Arbitrary Bluetooth read-write template application for BipOS.

## Description

This sample application prints to screen what it receives over Bluetooth (it should be ASCII-encoded data).

There is no app for sending data (yet), but you can still do so manually using an appropriate tool, such as [nRF Connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp). Select the bonded Amazfit Bip device, select the appropriate service (in my case, it is the service with UUID `0x3802`), and write data to the characteristic with `Notify, Read, Write` set. Writing the byte array `41424344` should write `ABCD` on the Bip's display.

## Requirements

### Building

* [alipay-bt library](https://github.com/neonsea/alipay-bt) with the correct offsets for your FW specified
* [libbip library](https://github.com/neonsea/libbip)

### Running

* BipOS with [appropriate FW patches for BT functionality](https://github.com/neonsea/bip-fw-patch)