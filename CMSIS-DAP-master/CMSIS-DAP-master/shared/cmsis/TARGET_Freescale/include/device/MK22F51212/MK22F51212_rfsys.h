/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL FREESCALE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */
/*
 * WARNING! DO NOT EDIT THIS FILE DIRECTLY!
 *
 * This file was generated automatically and any changes may be lost.
 */
#ifndef __HW_RFSYS_REGISTERS_H__
#define __HW_RFSYS_REGISTERS_H__

#include "MK22F51212.h"
#include "fsl_bitband.h"

/*
 * MK22F51212 RFSYS
 *
 * System register file
 *
 * Registers defined in this header file:
 * - HW_RFSYS_REGn - Register file register
 *
 * - hw_rfsys_t - Struct containing all module registers.
 */

#define HW_RFSYS_INSTANCE_COUNT (1U) /*!< Number of instances of the RFSYS module. */

/*******************************************************************************
 * HW_RFSYS_REGn - Register file register
 ******************************************************************************/

/*!
 * @brief HW_RFSYS_REGn - Register file register (RW)
 *
 * Reset value: 0x00000000U
 *
 * Each register can be accessed as 8-, 16-, or 32-bits.
 */
typedef union _hw_rfsys_regn
{
    uint32_t U;
    struct _hw_rfsys_regn_bitfields
    {
        uint32_t LL : 8;               /*!< [7:0]  */
        uint32_t LH : 8;               /*!< [15:8]  */
        uint32_t HL : 8;               /*!< [23:16]  */
        uint32_t HH : 8;               /*!< [31:24]  */
    } B;
} hw_rfsys_regn_t;

/*!
 * @name Constants and macros for entire RFSYS_REGn register
 */
/*@{*/
#define HW_RFSYS_REGn_COUNT (8U)

#define HW_RFSYS_REGn_ADDR(x, n) ((x) + 0x0U + (0x4U * (n)))

#define HW_RFSYS_REGn(x, n)      (*(__IO hw_rfsys_regn_t *) HW_RFSYS_REGn_ADDR(x, n))
#define HW_RFSYS_REGn_RD(x, n)   (HW_RFSYS_REGn(x, n).U)
#define HW_RFSYS_REGn_WR(x, n, v) (HW_RFSYS_REGn(x, n).U = (v))
#define HW_RFSYS_REGn_SET(x, n, v) (HW_RFSYS_REGn_WR(x, n, HW_RFSYS_REGn_RD(x, n) |  (v)))
#define HW_RFSYS_REGn_CLR(x, n, v) (HW_RFSYS_REGn_WR(x, n, HW_RFSYS_REGn_RD(x, n) & ~(v)))
#define HW_RFSYS_REGn_TOG(x, n, v) (HW_RFSYS_REGn_WR(x, n, HW_RFSYS_REGn_RD(x, n) ^  (v)))
/*@}*/

/*
 * Constants & macros for individual RFSYS_REGn bitfields
 */

/*!
 * @name Register RFSYS_REGn, field LL[7:0] (RW)
 *
 * Low lower byte
 */
/*@{*/
#define BP_RFSYS_REGn_LL     (0U)          /*!< Bit position for RFSYS_REGn_LL. */
#define BM_RFSYS_REGn_LL     (0x000000FFU) /*!< Bit mask for RFSYS_REGn_LL. */
#define BS_RFSYS_REGn_LL     (8U)          /*!< Bit field size in bits for RFSYS_REGn_LL. */

/*! @brief Read current value of the RFSYS_REGn_LL field. */
#define BR_RFSYS_REGn_LL(x, n) (HW_RFSYS_REGn(x, n).B.LL)

/*! @brief Format value for bitfield RFSYS_REGn_LL. */
#define BF_RFSYS_REGn_LL(v)  ((uint32_t)((uint32_t)(v) << BP_RFSYS_REGn_LL) & BM_RFSYS_REGn_LL)

/*! @brief Set the LL field to a new value. */
#define BW_RFSYS_REGn_LL(x, n, v) (HW_RFSYS_REGn_WR(x, n, (HW_RFSYS_REGn_RD(x, n) & ~BM_RFSYS_REGn_LL) | BF_RFSYS_REGn_LL(v)))
/*@}*/

/*!
 * @name Register RFSYS_REGn, field LH[15:8] (RW)
 *
 * Low higher byte
 */
/*@{*/
#define BP_RFSYS_REGn_LH     (8U)          /*!< Bit position for RFSYS_REGn_LH. */
#define BM_RFSYS_REGn_LH     (0x0000FF00U) /*!< Bit mask for RFSYS_REGn_LH. */
#define BS_RFSYS_REGn_LH     (8U)          /*!< Bit field size in bits for RFSYS_REGn_LH. */

/*! @brief Read current value of the RFSYS_REGn_LH field. */
#define BR_RFSYS_REGn_LH(x, n) (HW_RFSYS_REGn(x, n).B.LH)

/*! @brief Format value for bitfield RFSYS_REGn_LH. */
#define BF_RFSYS_REGn_LH(v)  ((uint32_t)((uint32_t)(v) << BP_RFSYS_REGn_LH) & BM_RFSYS_REGn_LH)

/*! @brief Set the LH field to a new value. */
#define BW_RFSYS_REGn_LH(x, n, v) (HW_RFSYS_REGn_WR(x, n, (HW_RFSYS_REGn_RD(x, n) & ~BM_RFSYS_REGn_LH) | BF_RFSYS_REGn_LH(v)))
/*@}*/

/*!
 * @name Register RFSYS_REGn, field HL[23:16] (RW)
 *
 * High lower byte
 */
/*@{*/
#define BP_RFSYS_REGn_HL     (16U)         /*!< Bit position for RFSYS_REGn_HL. */
#define BM_RFSYS_REGn_HL     (0x00FF0000U) /*!< Bit mask for RFSYS_REGn_HL. */
#define BS_RFSYS_REGn_HL     (8U)          /*!< Bit field size in bits for RFSYS_REGn_HL. */

/*! @brief Read current value of the RFSYS_REGn_HL field. */
#define BR_RFSYS_REGn_HL(x, n) (HW_RFSYS_REGn(x, n).B.HL)

/*! @brief Format value for bitfield RFSYS_REGn_HL. */
#define BF_RFSYS_REGn_HL(v)  ((uint32_t)((uint32_t)(v) << BP_RFSYS_REGn_HL) & BM_RFSYS_REGn_HL)

/*! @brief Set the HL field to a new value. */
#define BW_RFSYS_REGn_HL(x, n, v) (HW_RFSYS_REGn_WR(x, n, (HW_RFSYS_REGn_RD(x, n) & ~BM_RFSYS_REGn_HL) | BF_RFSYS_REGn_HL(v)))
/*@}*/

/*!
 * @name Register RFSYS_REGn, field HH[31:24] (RW)
 *
 * High higher byte
 */
/*@{*/
#define BP_RFSYS_REGn_HH     (24U)         /*!< Bit position for RFSYS_REGn_HH. */
#define BM_RFSYS_REGn_HH     (0xFF000000U) /*!< Bit mask for RFSYS_REGn_HH. */
#define BS_RFSYS_REGn_HH     (8U)          /*!< Bit field size in bits for RFSYS_REGn_HH. */

/*! @brief Read current value of the RFSYS_REGn_HH field. */
#define BR_RFSYS_REGn_HH(x, n) (HW_RFSYS_REGn(x, n).B.HH)

/*! @brief Format value for bitfield RFSYS_REGn_HH. */
#define BF_RFSYS_REGn_HH(v)  ((uint32_t)((uint32_t)(v) << BP_RFSYS_REGn_HH) & BM_RFSYS_REGn_HH)

/*! @brief Set the HH field to a new value. */
#define BW_RFSYS_REGn_HH(x, n, v) (HW_RFSYS_REGn_WR(x, n, (HW_RFSYS_REGn_RD(x, n) & ~BM_RFSYS_REGn_HH) | BF_RFSYS_REGn_HH(v)))
/*@}*/

/*******************************************************************************
 * hw_rfsys_t - module struct
 ******************************************************************************/
/*!
 * @brief All RFSYS module registers.
 */
#pragma pack(1)
typedef struct _hw_rfsys
{
    __IO hw_rfsys_regn_t REGn[8];          /*!< [0x0] Register file register */
} hw_rfsys_t;
#pragma pack()

/*! @brief Macro to access all RFSYS registers. */
/*! @param x RFSYS module instance base address. */
/*! @return Reference (not a pointer) to the registers struct. To get a pointer to the struct,
 *     use the '&' operator, like <code>&HW_RFSYS(RFSYS_BASE)</code>. */
#define HW_RFSYS(x)    (*(hw_rfsys_t *)(x))

#endif /* __HW_RFSYS_REGISTERS_H__ */
/* v33/140401/2.1.0 */
/* EOF */
