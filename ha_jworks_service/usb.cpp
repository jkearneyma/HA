#include "usb.h"

#include <libusb-1.0/libusb.h>

#include <stdexcept>

using namespace std;

USB::USB() : context(nullptr) {
	//libusb_set_debug(2);
	libusb_init(&context);
}

USB::~USB() {
	libusb_exit(context);
}

USB::DeviceList::DeviceList(
	USB &usb,
	uint32_t vendor,
	uint32_t product
) {
	libusb_device **list = nullptr;
	ssize_t count = libusb_get_device_list(usb.context, &list);
	for (ssize_t i = 0; i < count; ++i) {
		libusb_device_descriptor desc;
		libusb_get_device_descriptor(list[i], &desc);
		if (((vendor == 0) || (vendor == desc.idVendor))
			&& ((product == 0) || (product == desc.idProduct))) {
			libusb_ref_device(list[i]);
			devices.insert(list[i]);
		}
	}
	libusb_free_device_list(list, 1);
}

USB::DeviceList::~DeviceList() {
	for (auto device : devices) {
		libusb_unref_device(device);
	}
}

Device::Device(
	libusb_device *device
) :
	handle(nullptr)
{
	int err = libusb_open(device, &handle);
	if (!handle) throw std::runtime_error(libusb_error_name(err));
	libusb_device_descriptor desc;
	libusb_get_device_descriptor(device, &desc);
	manufacturer = get_string(desc.bLength, desc.iManufacturer);
	product = get_string(desc.bLength, desc.iProduct);
	serialNumber = get_string(desc.bLength, desc.iSerialNumber);

}

QString Device::get_string(
	size_t maxLen,
	uint8_t index
) const {
	unsigned char buffer[maxLen + 1];
	int len = libusb_get_string_descriptor_ascii(handle, index, &buffer[0], maxLen + 1);
	if (len < 0) throw std::runtime_error(libusb_error_name(len));
	return QString::fromUtf8(reinterpret_cast<const char *>(buffer));
}

Device::~Device() {
	libusb_close(handle);
}

