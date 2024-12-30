//
// Created by yelv1 on 24-12-29.
//

#include "fwdgt.h"

/**
 * @brief  Initialize the watchdog
 * @param  None
 * @retval None
 */
void watchdog_init(void) {
    /* Enable the LSI clock */
    rcu_osci_on(RCU_IRC40K);
    rcu_osci_stab_wait(RCU_IRC40K);

    /* Configure FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(625, FWDGT_PSC_DIV64); // Set timeout to 1 seconds (625 / 0.625 KHz)

    /* Enable FWDGT */
    fwdgt_enable();
}

/**
 * @brief  Reset the MCU
 * @param  None
 * @retval None
 */
void fwdgt_reset_mcu(void) {
    /* Enable the write access to the FWDGT_CTL register */
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

    /* Configure FWDGT to trigger a system reset */
    fwdgt_config(50, FWDGT_PSC_DIV4);

    /* Reload the counter to trigger the reset */
    fwdgt_counter_reload();
}

/**
 * @brief  Reload the watchdog counter
 * @param  None
 * @retval None
 */
void watchdog_reload(void) {
    fwdgt_counter_reload();
}