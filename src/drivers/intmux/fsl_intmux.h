/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_INTMUX_H_
#define _FSL_INTMUX_H_

#include "fsl_common.h"

/*!
 * @addtogroup intmux_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< Version 2.0.0. */
#define FSL_INTMUX_DRIVER_VERSION   (MAKE_VERSION(2, 0, 0))

/*! @brief INTMUX channel logic mode. */
typedef enum _intmux_channel_logic_mode
{
    kINTMUX_ChannelLogicOR = 0x0U, /*!< Logic OR all enabled interrupt inputs */
    kINTMUX_ChannelLogicAND,       /*!< Logic AND all enabled interrupt inputs */
} intmux_channel_logic_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @name Initialization and deinitialization */
/*@{*/

/*!
 * @brief Initializes INTMUX module.
 *
 * This function enables the clock gate for the specified INTMUX. It then resets all channels, so that no
 * interrupt sources are routed and the logic mode is set to the default of #kINTMUX_ChannelLogicOR.
 * Finally, the NVIC vectors for all of the INTMUX output channels are enabled.
 *
 * @param base INTMUX peripheral base address.
 */
void INTMUX_Init(INTMUX_Type *base);

/*!
 * @brief Deinitializes an INTMUX instance for operation.
 *
 * The clock gate for the specified INTMUX is disabled, and the NVIC vectors for all channels are disabled.
 *
 * @param base INTMUX peripheral base address.
 */
void INTMUX_Deinit(INTMUX_Type *base);

/*!
 * @brief Reset an INTMUX channel.
 *
 * Set all register values in the specified channel to their reset value. This will disable all interrupt
 * sources for the channel.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 */
static inline void INTMUX_ResetChannel(INTMUX_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);
    
    base->CHANNEL[channel].CHn_CSR |= INTMUX_CHn_CSR_RST_MASK;
}

/*!
 * @brief Set the logic mode for an INTMUX channel.
 *
 * INTMUX channels can be configured to use one of two logic modes that control how pending interrupt sources
 * on the channel trigger the output interrupt.
 * - #kINTMUX_ChannelLogicOR means any source pending will trigger the output interrupt.
 * - #kINTMUX_ChannelLogicAND means all selected sources on the channel must be pending before the channel
 *          output interrupt will trigger.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 * @param logic The INTMUX channel logic mode.
 */
static inline void INTMUX_SetChannelMode(INTMUX_Type *base, uint32_t channel, intmux_channel_logic_mode_t logic)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);
    
    base->CHANNEL[channel].CHn_CSR = INTMUX_CHn_CSR_AND(logic);
}

/*@}*/
/*! @name Sources */
/*@{*/

/*!
 * @brief Enable an interrupt source on an INTMUX channel.
 *
 * @param base INTMUX peripheral base address.
 * @param channel Index of the INTMUX channel on which the specified interrupt will be enabled.
 * @param irq Interrupt to route to the specified INTMUX channel. The interrupt must be an INTMUX source.
 */
static inline void INTMUX_EnableInterrupt(INTMUX_Type *base, uint32_t channel, IRQn_Type irq)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);
    assert(irq >= FSL_FEATURE_INTMUX_IRQ_START_INDEX);
    
    base->CHANNEL[channel].CHn_IER_31_0 |= (1U << ((uint32_t)irq - FSL_FEATURE_INTMUX_IRQ_START_INDEX));
}

/*!
 * @brief Disable an interrupt source on an INTMUX channel.
 *
 * @param base INTMUX peripheral base address.
 * @param channel Index of the INTMUX channel on which the specified interrupt will be disabled.
 * @param irq Interrupt number. The interrupt must be an INTMUX source.
 */
static inline void INTMUX_DisableInterrupt(INTMUX_Type *base, uint32_t channel, IRQn_Type irq)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);
    assert(irq >= FSL_FEATURE_INTMUX_IRQ_START_INDEX);
    
    base->CHANNEL[channel].CHn_IER_31_0 &= ~(1U << ((uint32_t)irq - FSL_FEATURE_INTMUX_IRQ_START_INDEX));
}

/*@}*/
/*! @name Status */
/*@{*/

/*!
 * @brief Get INTMUX pending interrupt sources for specific channel.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 * @return The mask of pending interrupt bits. Bit[n] set means intmux source n is pending.
 */
static inline uint32_t INTMUX_GetChannelPendingSources(INTMUX_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);

    return base->CHANNEL[channel].CHn_IPR_31_0;
}

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FSL_INTMUX_H_ */
