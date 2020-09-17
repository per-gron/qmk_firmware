/* Copyright 2020 Nick Brassel (tzarc)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include_next "board.h"

#undef STM32_HSE_BYPASS

/*
 * GPIOA setup:
 *
 * PA0  - BUTTON                    (input floating).
 * PA1  - PIN1                      (input floating).
 * PA2  - PIN2                      (input floating).
 * PA3  - PIN3                      (input floating).
 * PA4  - CS43L22_LRCK              (input floating).
 * PA5  - L3GD20_SCL                (input floating).
 * PA6  - L3GD20_SD0                (input floating).
 * PA7  - L3GD20_SDI                (input floating).
 * PA8  - PIN8                      (input floating).
 * PA9  - VBUS_FS                   (input floating).
 * PA10 - OTG_FS_ID                 (input floating).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - SWDIO                     (input floating).
 * PA14 - SWCLK                     (input floating).
 * PA15 - PIN15                     (input floating).
 */
#undef VAL_GPIOA_MODER
#undef VAL_GPIOA_OTYPER
#undef VAL_GPIOA_OSPEEDR
#undef VAL_GPIOA_PUPDR
#undef VAL_GPIOA_ODR
#undef VAL_GPIOA_AFRL
#undef VAL_GPIOA_AFRH
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON) |         \
                                     PIN_MODE_INPUT(GPIOA_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOA_CS43L22_LRCK) |\
                                     PIN_MODE_INPUT(GPIOA_L3GD20_SCL) | \
                                     PIN_MODE_INPUT(GPIOA_L3GD20_SD0) | \
                                     PIN_MODE_INPUT(GPIOA_L3GD20_SDI) | \
                                     PIN_MODE_INPUT(GPIOA_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOA_VBUS_FS) |        \
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_INPUT(GPIOA_SWDIO) |      \
                                     PIN_MODE_INPUT(GPIOA_SWCLK) |      \
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_BUTTON) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CS43L22_LRCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_L3GD20_SCL) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_L3GD20_SD0) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_L3GD20_SDI) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_VBUS_FS) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_BUTTON) |        \
                                     PIN_OSPEED_HIGH(GPIOA_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOA_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOA_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOA_CS43L22_LRCK) |  \
                                     PIN_OSPEED_HIGH(GPIOA_L3GD20_SCL) |    \
                                     PIN_OSPEED_HIGH(GPIOA_L3GD20_SD0) |    \
                                     PIN_OSPEED_HIGH(GPIOA_L3GD20_SDI) |    \
                                     PIN_OSPEED_HIGH(GPIOA_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOA_VBUS_FS) |       \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_ID) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) |         \
                                     PIN_OSPEED_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_BUTTON) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_CS43L22_LRCK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_L3GD20_SCL) | \
                                     PIN_PUPDR_FLOATING(GPIOA_L3GD20_SD0) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_L3GD20_SDI) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN8) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_VBUS_FS) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_ID) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_BUTTON) |           \
                                     PIN_ODR_HIGH(GPIOA_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOA_CS43L22_LRCK) |     \
                                     PIN_ODR_HIGH(GPIOA_L3GD20_SCL) |       \
                                     PIN_ODR_HIGH(GPIOA_L3GD20_SD0) |       \
                                     PIN_ODR_HIGH(GPIOA_L3GD20_SDI) |       \
                                     PIN_ODR_HIGH(GPIOA_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOA_VBUS_FS) |          \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_ID) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_BUTTON, 0U) |        \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_CS43L22_LRCK, 0U) |  \
                                     PIN_AFIO_AF(GPIOA_L3GD20_SCL, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_L3GD20_SD0, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_L3GD20_SDI, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_VBUS_FS, 0U) |       \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input floating).
 * PB1  - PIN1                      (input floating).
 * PB2  - PIN2                      (input floating).
 * PB3  - SWO                       (input floating).
 * PB4  - PIN4                      (input floating).
 * PB5  - PIN5                      (input floating).
 * PB6  - LSM303DLHC_SCL            (alternate 4 I2C1 SCL).
 * PB7  - PIN7                      (alternate 4 I2C1 SDA).
 * PB8  - PIN8                      (input floating).
 * PB9  - LSM303DLHC_SDA            (input floating).
 * PB10 - MP45DT02_CLK_IN           (input floating).
 * PB11 - PIN11                     (input floating).
 * PB12 - PIN12                     (input floating).
 * PB13 - PIN13                     (input floating).
 * PB14 - PIN14                     (input floating).
 * PB15 - PIN15                     (input floating).
 */
#undef VAL_GPIOB_MODER
#undef VAL_GPIOB_OTYPER
#undef VAL_GPIOB_OSPEEDR
#undef VAL_GPIOB_PUPDR
#undef VAL_GPIOB_ODR
#undef VAL_GPIOB_AFRL
#undef VAL_GPIOB_AFRH
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN1) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN2) |            \
                                     PIN_MODE_INPUT(GPIOB_SWO) |             \
                                     PIN_MODE_INPUT(GPIOB_PIN4) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |            \
                                     PIN_MODE_ALTERNATE(GPIOB_LSM303DLHC_SCL) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_PIN7) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN8) |            \
                                     PIN_MODE_INPUT(GPIOB_LSM303DLHC_SDA) |  \
                                     PIN_MODE_INPUT(GPIOB_MP45DT02_CLK_IN) | \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN14) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO) |             \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |            \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_LSM303DLHC_SCL) |  \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_PIN7) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8) |            \
                                     PIN_OTYPE_PUSHPULL(GPIOB_LSM303DLHC_SDA) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_MP45DT02_CLK_IN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |           \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |           \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |           \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |           \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_PIN0) |            \
                                     PIN_OSPEED_HIGH(GPIOB_PIN1) |            \
                                     PIN_OSPEED_HIGH(GPIOB_PIN2) |            \
                                     PIN_OSPEED_HIGH(GPIOB_SWO) |             \
                                     PIN_OSPEED_HIGH(GPIOB_PIN4) |            \
                                     PIN_OSPEED_HIGH(GPIOB_PIN5) |            \
                                     PIN_OSPEED_HIGH(GPIOB_LSM303DLHC_SCL) |  \
                                     PIN_OSPEED_HIGH(GPIOB_PIN7) |            \
                                     PIN_OSPEED_HIGH(GPIOB_PIN8) |            \
                                     PIN_OSPEED_HIGH(GPIOB_LSM303DLHC_SDA) |  \
                                     PIN_OSPEED_HIGH(GPIOB_MP45DT02_CLK_IN) | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN11) |           \
                                     PIN_OSPEED_HIGH(GPIOB_PIN12) |           \
                                     PIN_OSPEED_HIGH(GPIOB_PIN13) |           \
                                     PIN_OSPEED_HIGH(GPIOB_PIN14) |           \
                                     PIN_OSPEED_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_PIN0) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) |             \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN4) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_LSM303DLHC_SCL) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN8) |            \
                                     PIN_PUPDR_FLOATING(GPIOB_LSM303DLHC_SDA) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_MP45DT02_CLK_IN) | \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11) |           \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN12) |           \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN13) |           \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN14) |           \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PIN0) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN1) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |              \
                                     PIN_ODR_HIGH(GPIOB_SWO) |               \
                                     PIN_ODR_HIGH(GPIOB_PIN4) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |              \
                                     PIN_ODR_HIGH(GPIOB_LSM303DLHC_SCL) |    \
                                     PIN_ODR_HIGH(GPIOB_PIN7) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN8) |              \
                                     PIN_ODR_HIGH(GPIOB_LSM303DLHC_SDA) |    \
                                     PIN_ODR_HIGH(GPIOB_MP45DT02_CLK_IN) |   \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN14) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_SWO, 0U) |            \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_LSM303DLHC_SCL, 4U) | \
                                     PIN_AFIO_AF(GPIOB_PIN7, 4U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_LSM303DLHC_SDA, 0U) | \
                                     PIN_AFIO_AF(GPIOB_MP45DT02_CLK_IN, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0U))
