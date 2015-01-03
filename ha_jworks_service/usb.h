#ifndef USB_H
#define USB_H

#include <memory>
#include <set>
#include <cstdint>
#include <QString>

struct libusb_context;
struct libusb_device;
struct libusb_device_handle;

struct USB {
	USB();
	~USB();

	struct DeviceList {
		std::set<libusb_device *> devices;

		DeviceList(
			USB &,
			uint32_t vendor,
			uint32_t id
		);

		~DeviceList();
	};

private:
	struct libusb_context *context;
};

struct Device {
	Device(libusb_device *);
	~Device();

	QString manufacturer, product, serialNumber;

	libusb_device_handle *get() const { return handle; }

protected:
	libusb_device_handle *handle;

	QString get_string(size_t maxLen, uint8_t index) const;
};

#endif
