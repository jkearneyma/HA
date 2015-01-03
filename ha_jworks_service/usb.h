#include <memory>
#include <set>
#include <string>
#include <cstdint>

struct libusb_context;
struct libusb_device;
struct libusb_device_handle;

struct USB {
	USB();
	~USB();

	struct DeviceList {
		std::set<struct libusb_device *> devices;

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
	Device(struct libusb_device *);
	~Device();
	std::string manufacturer, product, serialNumber;

protected:
	libusb_device_handle *handle;

	std::string get_string(size_t maxLen, uint8_t index) const;
};

