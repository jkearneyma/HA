#ifndef HA_COMMON
#define HA_COMMON

#define HA_TLD "com"
#define HA_DOMAIN "jkearney"
#define HA_APP "HA"

#define DBUS_PFX HA_TLD "." HA_DOMAIN "." HA_APP "."
#define INTERFACE "/" HA_TLD "/" HA_DOMAIN "/" HA_APP "/" DBUS_ID

template <typename APP>
inline void setAppDefaults(APP &app) {
    app.setOrganizationName(HA_DOMAIN "." HA_TLD);
    app.setApplicationName(HA_APP);
}

#endif // HA_COMMON

