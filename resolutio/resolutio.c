#include <windows.h>
#include <stdio.h>

struct DisplaySettings
{
    struct _devicemodeW DEVICE_MODEW;
    int is_success;
};

struct DisplaySettings get_display_settings()
{
    struct _DISPLAY_DEVICEW DISPLAY_DEVICEW;
    struct _devicemodeW DEVICE_MODEW;

    DISPLAY_DEVICEW.cb = sizeof(DISPLAY_DEVICEW);
    DEVICE_MODEW.dmSize = sizeof(DEVICE_MODEW);

    WINBOOL is_d_success = EnumDisplayDevicesW(
        NULL, 0, &DISPLAY_DEVICEW, 0x00000001);
    WINBOOL is_s_success = EnumDisplaySettingsW(
        DISPLAY_DEVICEW.DeviceName, ENUM_CURRENT_SETTINGS, &DEVICE_MODEW);
    int is_success = is_d_success && is_s_success;

    struct DisplaySettings display_settings;
    display_settings.DEVICE_MODEW = DEVICE_MODEW;
    display_settings.is_success = is_success;

    return display_settings;
}

int change_display_resolution(struct DisplaySettings display_settings)
{
    struct _devicemodeW DEVICE_MODEW = display_settings.DEVICE_MODEW;

    int width = DEVICE_MODEW.dmPelsWidth,
        height = DEVICE_MODEW.dmPelsHeight;

    int uhd_width = 3840, uhd_height = 2160,
        hd_width = 1920, hd_height = 1080;

    if (width == uhd_width && height == uhd_height)
    {
        DEVICE_MODEW.dmPelsWidth = hd_width;
        DEVICE_MODEW.dmPelsHeight = hd_height;
    }
    else
    {
        DEVICE_MODEW.dmPelsWidth = uhd_width;
        DEVICE_MODEW.dmPelsHeight = uhd_height;
    }

    long status_code = ChangeDisplaySettingsW(&DEVICE_MODEW, 0);
    int is_success = status_code == DISP_CHANGE_SUCCESSFUL;

    return is_success;
}

void show_message_box(char text[], long flags)
{
    MessageBox(0, text, "Toggle Resolution", flags);
}

int main()
{
    struct DisplaySettings display_settings = get_display_settings();
    int is_success = display_settings.is_success;

    if (is_success)
    {
        int is_success = change_display_resolution(display_settings);

        if (is_success)
        {
            show_message_box("Your screen resolution has changed.",
                             0x00080040L);
        }
        else
        {
            show_message_box("Couldn't change the display settings...",
                             0x00080010L);
        }
    }
    else
    {
        show_message_box("Couldn't find the display...",
                         0x00080010L);
    }

    return 0;
}