#include "usb.hpp"
#include <usb.h>

#include <iostream>

using namespace std;

USB::USB() {
	//usb_set_debug(2);
	usb_init();
	usb_find_busses();
	usb_find_devices();
	list = usb_get_busses();
	if (!list) throw usb_strerror();
}

USB::~USB() {
}

map<struct usb_device_descriptor *, struct usb_device *>
USB::find(uint32_t vendor, uint32_t id) const {
	map<struct usb_device_descriptor *, struct usb_device *> ret;
	for (auto bus = list; bus; bus = bus->next) {
		for (auto dev = bus->devices; dev; dev = dev->next) {
			auto desc = &dev->descriptor;
			if (((vendor == 0) || (desc->idVendor == vendor)) &&
				((id == 0) || (desc->idProduct == id))) {
				ret[desc] = dev;
			}
		}
	}
	return ret;
}

Device::Device(struct usb_device_descriptor *desc, struct usb_device *dev) {
	handle = usb_open(dev);
	if (!handle) throw "failed to open USB device";
	if (usb_set_configuration(handle, 1)) throw usb_strerror();
	if (usb_claim_interface(handle, 0)) throw usb_strerror();
	manufacturer = get_string(desc->bLength, desc->iManufacturer);
	product = get_string(desc->bLength, desc->iProduct);
	serialNumber = get_string(desc->bLength, desc->iSerialNumber);
}

Device::~Device() {
	usb_release_interface(handle, 0);
	usb_close(handle);
}

string Device::get_string(size_t maxLen, uint8_t index) const {
	char buffer[maxLen];
	int len = usb_get_string_simple(handle, index, &buffer[0], maxLen);
	if (len < 0) throw usb_strerror();
	return string(&buffer[0], &buffer[0] + len);
}

