#include <math.h>
#include "sensor.h"
#include "iom361_r4.h"

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc;
    uint32_t raw_temp;
    uint32_t raw_rh;
    reading_t result;

    if (r == NULL) {
        return 1;
    }

    raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    raw_rh = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    raw_temp &= 0x000FFFFFu;
    raw_rh &= 0x000FFFFFu;

    double temp = raw_temp / 1048576.0 * 200.0 - 50.0;
    double rh = raw_rh / 1048576.0 * 100.0;

    result.t = now;
    result.temp_c = (float)(round(temp * 10.0) / 10.0);
    result.rh_pct = (float)(round(rh * 10.0) / 10.0);

    *r = result;
    return 0;
}
