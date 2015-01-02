#include <memory>
#include <map>
#include <string>
#include <cstdint>

struct usb_dev_handle;
struct usb_device;


struct USB {
	USB();
	~USB();

	std::map<struct usb_device_descriptor *, struct usb_device *> find(uint32_t vendor, uint32_t id) const;

private:
	struct usb_bus *list;
};

struct Device {
	Device(struct usb_device_descriptor *, struct usb_device *);
	~Device();
	std::string manufacturer, product, serialNumber;

protected:
	usb_dev_handle *handle;

	std::string get_string(size_t maxLen, uint8_t index) const;
};

