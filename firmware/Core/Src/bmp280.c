/* Simple driver for BMP280
    (c) Nick Lott 2024

*/
#include <stdint.h>

typedef BMP280_S32_t int32_t;
typedef BMP280_U32_t uint32_t;

// what is our clock freq? How many instructions is 20ns ?
// --> 20ns = 50Mhz so probably ok to use single NOP
// HAL_Delay() is in milliseconds so a bit too long for this.
#define BMP_WAIT_20ns   do {__asm__("nop")} while(0)

#define BMP_CSB_SETUP_TIME    WAIT_20ns;
#define BMP_CSB_HOLD_TIME     WAIT_20ns;

#define BMP_SELECT   do {HAL_GPIO_WritePin(BMP_CS_GPIO_Port, BMP_CS_Pin, GPIO_PIN_RESET);BMP_CSB_SETUP_TIME;} while(0)
#define BMP_RELEASE  do {BMP_CSB_HOLD_TIME;HAL_GPIO_WritePin(BMP_CS_GPIO_Port, BMP_CS_Pin, GPIO_PIN_SET);} while(0)

static void select(){
}

static uint8_t read( uint8_t out)
{
}

void
bmp280_init(void )
{
    // data sheet informs that chip uses i2c untill it sees CS go low first time
    // so lets select and unselect
    BMP_SELECT;
    BMP_RELEASE;



}

bmp280_update()
bmp280_get_temp()
bmp280_get_alt()


// START: from Bosch Sensortec BMP280 Data sheet : BST-BMP280-DS001-26 Revision_1.26_102021
// ------>8-------------------------------------------------------------------------
// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
BMP280_S32_t t_fine;
static BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
    BMP280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) – ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) – ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) – ((BMP280_S32_t)dig_T1))) >> 12) *
            ((BMP280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
static BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P)
{
    BMP280_S32_t var1, var2;
    BMP280_U32_t p;
    var1 = (((BMP280_S32_t)t_fine)>>1) – (BMP280_S32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
    var2 = var2 + ((var1*((BMP280_S32_t)dig_P5))<<1);
    var2 = (var2>>2)+(((BMP280_S32_t)dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1)>>1))>>18;
    var1 =((((32768+var1))*((BMP280_S32_t)dig_P1))>>15);
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else
    {
        p = (p / (BMP280_U32_t)var1) * 2;
    }
    var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)dig_P8))>>13;
    p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
    return p;
}
// ------>8-------------------------------------------------------------------------
// END: from Bosch Sensortec BMP280 Data sheet : BST-BMP280-DS001-26 Revision_1.26_102021
