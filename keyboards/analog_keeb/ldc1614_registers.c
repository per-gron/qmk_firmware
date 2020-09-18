#include "ldc1614_registers.h"

#include "common/print.h"

void prettyprint_status_register(const Ldc1614Status* status) {
    xprintf("unreadconv3: %d\n", status->unreadconv3);
    xprintf("unreadconv2: %d\n", status->unreadconv2);
    xprintf("unreadconv1: %d\n", status->unreadconv1);
    xprintf("unreadconv0: %d\n", status->unreadconv0);
    xprintf("reserved_0: %d\n", status->reserved_0);
    xprintf("drdy: %d\n", status->drdy);
    xprintf("reserved_1: %d\n", status->reserved_1);
    xprintf("err_zc: %d\n", status->err_zc);
    xprintf("err_ale: %d\n", status->err_ale);
    xprintf("err_ahe: %d\n", status->err_ahe);
    xprintf("err_wd: %d\n", status->err_wd);
    xprintf("err_or: %d\n", status->err_or);
    xprintf("err_ur: %d\n", status->err_ur);
    xprintf("err_chan: %d\n", status->err_chan);
}

void prettyprint_clock_dividers_register(const Ldc1614ClockDividers* clock_dividers) {
    xprintf("fref_divider: %d\n", clock_dividers->fref_divider);
    xprintf("reserved: %d\n", clock_dividers->reserved);
    xprintf("fin_divider: %d\n", clock_dividers->fin_divider);
}

void prettyprint_drive_current_register(const Ldc1614DriveCurrent* drive_current) {
    xprintf("reserved: %d\n", drive_current->reserved);
    xprintf("init_drive: %d\n", drive_current->init_drive);
    xprintf("idrive: %d\n", drive_current->idrive);
}

void prettyprint_config_register(const Ldc1614Config* config) {
    xprintf("active_chan: %d\n", config->active_chan);
    xprintf("sleep_mode_en: %d\n", config->sleep_mode_en);
    xprintf("rp_override_en: %d\n", config->rp_override_en);
    xprintf("sensor_activate_sel: %d\n", config->sensor_activate_sel);
    xprintf("auto_amp_dis: %d\n", config->auto_amp_dis);
    xprintf("ref_clk_src: %d\n", config->ref_clk_src);
    xprintf("reserved_0: %d\n", config->reserved_0);
    xprintf("intb_dis: %d\n", config->intb_dis);
    xprintf("high_current_drv: %d\n", config->high_current_drv);
    xprintf("reserved_1: %d\n", config->reserved_1);
}

void prettyprint_mux_config_register(const Ldc1614MuxConfig* mux_config) {
    xprintf("autoscan_en: %d\n", mux_config->autoscan_en);
    xprintf("rr_sequence: %d\n", mux_config->rr_sequence);
    xprintf("reserved: %d\n", mux_config->reserved);
    xprintf("deglitch: %d\n", mux_config->deglitch);
}
