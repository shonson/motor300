# Exporting Component Description of MC_System_sb_MSS to TCL
# Family: SmartFusion2
# Part Number: M2S010-1FG484

set mss_name {MC_System_sb_MSS}

# Creating the MSS component MC_System_sb_MSS
create_and_configure_core -core_vlnv Actel:SmartFusion2MSS:MSS:$MSSver -component_name ${mss_name} -params {}

# Enabling/Disabling the instance 'CAN' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {CAN}

# Configuring the instance 'CC' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {CC} -params {\
"CACHE_ENABLED:false" \
"CC_CACHE_REGION:128MB_0001" }
mss_save_instance_config -component_name ${mss_name} -instance_name {CC}

# Configuring the instance 'CCC' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {CCC} -params {\
"FAB_PLL_LOCK_EN:false" \
"FAB_PLL_LOCK_IS_USED:1" \
"FAB_PLL_LOCK_LOST_EN:false" \
"FACC_GLMUX_SEL_IS_USED:false" \
"FCLK_FREQ:100.00" \
"FCLK_PCLK0_DIVISOR:1" \
"FCLK_PCLK1_DIVISOR:1" \
"FIC32_0_DIVISOR:1" \
"FIC32_1_DIVISOR:1" \
"MDDR_CLK_FIC64_DIVISOR:1" \
"MDDR_FCLK_MULT:1" \
"MPLL_LOCK_COUNT:32" \
"MPLL_LOCK_EN:false" \
"MPLL_LOCK_IS_USED:false" \
"MPLL_LOCK_LOST_EN:false" \
"MPLL_LOCK_WINDOW:8000" \
"MPLL_SUPPLY_VOLTAGE:3_3" \
"REFCLK_FREQ:50.0" \
"REFCLK_SRC:CLK_BASE" }
mss_save_instance_config -component_name ${mss_name} -instance_name {CCC}

# Configuring the instance 'CFGM' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {CFGM} -params {\
"CFG_FABRIC:1" \
"CFG_MDDR:0" \
"MASTER:MSS" \
"MDDR_CONFIG_LOCAL:1" \
"PER2_PADDR_MSB:15" }
mss_save_instance_config -component_name ${mss_name} -instance_name {CFGM}

# Configuring the instance 'CM3' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {CM3} -params {\
"MPU_USED:true" \
"RX_EV_USED:false" \
"SLEEPDEEP_USED:false" \
"SLEEPHOLD_USED:false" \
"SLEEPING_USED:false" \
"STCALIB:0x2000000" \
"STCLK_DIVISOR:STCLK_DIVISOR_32" \
"TRACE_USED:false" \
"TRACECLK_DIV2_SEL:false" \
"TX_EV_USED:false" }
mss_save_instance_config -component_name ${mss_name} -instance_name {CM3}

# Configuring the instance 'DDRB' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {DDRB} -params {\
"DDRB_BUF_SZ:32B" \
"DDRB_DS_REN:true" \
"DDRB_DS_WEN:true" \
"DDRB_HPD_ENABLE:false" \
"DDRB_HPD_REN:true" \
"DDRB_HPD_WEN:true" \
"DDRB_IDC_REN:true" \
"DDRB_NB_ADDR:0xA000" \
"DDRB_NB_SZ:64KB" \
"DDRB_SW_REN:true" \
"DDRB_SW_WEN:true" \
"DDRB_TIMER:0x3FF" }
mss_save_instance_config -component_name ${mss_name} -instance_name {DDRB}

# Enabling/Disabling the instance 'DMA' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {DMA}

# Configuring the instance 'EDAC' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {EDAC} -params {\
"EDAC_ENABLE_CAN_EDAC_EN:false" \
"EDAC_ENABLE_CC_EDAC_EN:false" \
"EDAC_ENABLE_ESRAM0_EDAC_EN:false" \
"EDAC_ENABLE_ESRAM1_EDAC_EN:false" \
"EDAC_ENABLE_MAC_EDACRX_EN:false" \
"EDAC_ENABLE_MAC_EDACTX_EN:false" \
"EDAC_ENABLE_USB_EDAC_EN:false" \
"EDAC_INT_ENABLE_CAN:DISABLE_ALL" \
"EDAC_INT_ENABLE_CC:DISABLE_ALL" \
"EDAC_INT_ENABLE_ESRAM0:DISABLE_ALL" \
"EDAC_INT_ENABLE_ESRAM1:DISABLE_ALL" \
"EDAC_INT_ENABLE_MAC_EDACRX:DISABLE_ALL" \
"EDAC_INT_ENABLE_MAC_EDACTX:DISABLE_ALL" \
"EDAC_INT_ENABLE_MDDR_ECC_INT_EN:true" \
"EDAC_INT_ENABLE_USB:DISABLE_ALL" \
"EXPOSE_EDAC_ERROR:false" }
mss_save_instance_config -component_name ${mss_name} -instance_name {EDAC}

# Configuring the instance 'ENVM' in the MSS component
mss_configure_envm -component_name ${mss_name} -cfg_file "./ENVM.cfg" 

# Enabling/Disabling the instance 'FIC32_0' in the MSS component
mss_enable_instance -component_name ${mss_name} -instance_name {FIC32_0}

# Configuring the instance 'FIC32_0' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {FIC32_0} -params {\
"EXPOSE_MASTER_IDENTITY:false" \
"FAB_CLK_FREQUENCY:100.000" \
"INTERFACE_MASTER:true" \
"INTERFACE_SLAVE:false" \
"INTERFACE_TYPE:INTERFACE_APB" \
"MSS_CLK_FREQUENCY:100.000" \
"USE_BYPASS_MODE:false" \
"USE_FAB_REGION_0:true" \
"USE_FAB_REGION_1:true" \
"USE_FAB_REGION_2:false" \
"USE_FAB_REGION_3:false" \
"USE_FAB_REGION_4:false" \
"USE_FAB_REGION_5:false" }
mss_save_instance_config -component_name ${mss_name} -instance_name {FIC32_0}

# Enabling/Disabling the instance 'GPIO' in the MSS component
mss_enable_instance -component_name ${mss_name} -instance_name {GPIO}

# Configuring the instance 'GPIO' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {GPIO} -params {\
"ADVANCED_OPTION:false" \
"GPIO0_FAB_CAPT:false" \
"GPIO0_TYPE:CONNECTION_FABRICA" \
"GPIO0_USAGE:USAGE_OUTPUT" \
"GPIO1_FAB_CAPT:false" \
"GPIO1_TYPE:CONNECTION_IOA" \
"GPIO1_USAGE:NOT_USED" \
"GPIO2_FAB_CAPT:false" \
"GPIO2_TYPE:CONNECTION_IOA" \
"GPIO2_USAGE:NOT_USED" \
"GPIO3_FAB_CAPT:false" \
"GPIO3_TYPE:CONNECTION_IOA" \
"GPIO3_USAGE:NOT_USED" \
"GPIO4_FAB_CAPT:false" \
"GPIO4_TYPE:CONNECTION_IOA" \
"GPIO4_USAGE:NOT_USED" \
"GPIO5_FAB_CAPT:false" \
"GPIO5_TYPE:CONNECTION_IOA" \
"GPIO5_USAGE:NOT_USED" \
"GPIO6_FAB_CAPT:false" \
"GPIO6_TYPE:CONNECTION_IOA" \
"GPIO6_USAGE:NOT_USED" \
"GPIO7_FAB_CAPT:false" \
"GPIO7_TYPE:CONNECTION_IOA" \
"GPIO7_USAGE:NOT_USED" \
"GPIO8_FAB_CAPT:false" \
"GPIO8_TYPE:CONNECTION_IOA" \
"GPIO8_USAGE:NOT_USED" \
"GPIO9_FAB_CAPT:false" \
"GPIO9_TYPE:CONNECTION_IOA" \
"GPIO9_USAGE:NOT_USED" \
"GPIO10_FAB_CAPT:false" \
"GPIO10_TYPE:CONNECTION_IOA" \
"GPIO10_USAGE:NOT_USED" \
"GPIO11_FAB_CAPT:false" \
"GPIO11_TYPE:CONNECTION_IOA" \
"GPIO11_USAGE:NOT_USED" \
"GPIO12_FAB_CAPT:false" \
"GPIO12_TYPE:CONNECTION_IOA" \
"GPIO12_USAGE:NOT_USED" \
"GPIO13_FAB_CAPT:false" \
"GPIO13_TYPE:CONNECTION_IOA" \
"GPIO13_USAGE:NOT_USED" \
"GPIO14_FAB_CAPT:false" \
"GPIO14_TYPE:CONNECTION_IOA" \
"GPIO14_USAGE:NOT_USED" \
"GPIO15_FAB_CAPT:false" \
"GPIO15_TYPE:CONNECTION_IOA" \
"GPIO15_USAGE:NOT_USED" \
"GPIO16_FAB_CAPT:false" \
"GPIO16_TYPE:CONNECTION_IOA" \
"GPIO16_USAGE:NOT_USED" \
"GPIO17_FAB_CAPT:false" \
"GPIO17_TYPE:CONNECTION_IOA" \
"GPIO17_USAGE:NOT_USED" \
"GPIO18_FAB_CAPT:false" \
"GPIO18_TYPE:CONNECTION_IOA" \
"GPIO18_USAGE:NOT_USED" \
"GPIO19_FAB_CAPT:false" \
"GPIO19_TYPE:CONNECTION_IOA" \
"GPIO19_USAGE:NOT_USED" \
"GPIO20_FAB_CAPT:false" \
"GPIO20_TYPE:CONNECTION_IOA" \
"GPIO20_USAGE:NOT_USED" \
"GPIO21_FAB_CAPT:false" \
"GPIO21_TYPE:CONNECTION_IOA" \
"GPIO21_USAGE:NOT_USED" \
"GPIO22_FAB_CAPT:false" \
"GPIO22_TYPE:CONNECTION_IOA" \
"GPIO22_USAGE:NOT_USED" \
"GPIO23_FAB_CAPT:false" \
"GPIO23_TYPE:CONNECTION_IOA" \
"GPIO23_USAGE:NOT_USED" \
"GPIO24_FAB_CAPT:false" \
"GPIO24_TYPE:CONNECTION_IOA" \
"GPIO24_USAGE:NOT_USED" \
"GPIO25_FAB_CAPT:false" \
"GPIO25_TYPE:CONNECTION_IOA" \
"GPIO25_USAGE:NOT_USED" \
"GPIO26_FAB_CAPT:false" \
"GPIO26_TYPE:CONNECTION_IOA" \
"GPIO26_USAGE:NOT_USED" \
"GPIO27_FAB_CAPT:false" \
"GPIO27_TYPE:CONNECTION_IOA" \
"GPIO27_USAGE:NOT_USED" \
"GPIO28_FAB_CAPT:false" \
"GPIO28_TYPE:CONNECTION_IOA" \
"GPIO28_USAGE:NOT_USED" \
"GPIO29_FAB_CAPT:false" \
"GPIO29_TYPE:CONNECTION_IOA" \
"GPIO29_USAGE:NOT_USED" \
"GPIO30_FAB_CAPT:false" \
"GPIO30_TYPE:CONNECTION_IOA" \
"GPIO30_USAGE:NOT_USED" \
"GPIO31_FAB_CAPT:false" \
"GPIO31_TYPE:CONNECTION_IOA" \
"GPIO31_USAGE:NOT_USED" \
"GPIO_7_0_SET_RESET_SRC:GPIO_SYSREG" \
"GPIO_7_0_SET_RESET_VAL:RESET_STATE_0" \
"GPIO_15_8_SET_RESET_SRC:GPIO_SYSREG" \
"GPIO_15_8_SET_RESET_VAL:RESET_STATE_1" \
"GPIO_23_16_SET_RESET_SRC:GPIO_SYSREG" \
"GPIO_23_16_SET_RESET_VAL:RESET_STATE_1" \
"GPIO_31_24_SET_RESET_SRC:GPIO_SYSREG" \
"GPIO_31_24_SET_RESET_VAL:RESET_STATE_1" \
"USE_RESET_FABRIC:false" }
mss_save_instance_config -component_name ${mss_name} -instance_name {GPIO}

# Enabling/Disabling the instance 'I2C_0' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {I2C_0}

# Enabling/Disabling the instance 'I2C_1' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {I2C_1}

# Configuring the instance 'INTR' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {INTR} -params {\
"CC_INTERRUPT_EN:0x7" \
"DDRB_INTERRUPT_EN:0x3FF" \
"F2H_INTERRUPT_USED:false" \
"H2F_INTERRUPT_USED:false" \
"SW_INTERRUPT_EN:0x7F" }
mss_save_instance_config -component_name ${mss_name} -instance_name {INTR}

# Enabling/Disabling the instance 'MAC' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {MAC}

# Enabling/Disabling the instance 'MDDR' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {MDDR}

# Enabling/Disabling the instance 'MMUART_0' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {MMUART_0}

# Enabling/Disabling the instance 'MMUART_1' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {MMUART_1}

# Configuring the instance 'RESET' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {RESET} -params {\
"FAB_M3_RESET_N_USED:false" \
"FPGA_RESET_N_USED:true" \
"USER_MSS_RESET_N_USED:true" }
mss_save_instance_config -component_name ${mss_name} -instance_name {RESET}

# Enabling/Disabling the instance 'RTC' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {RTC}

# Configuring the instance 'SECURITY' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {SECURITY} -params {\
"CC_CACHEREGION_OVERRIDE:true" \
"CC_CONFIG_OVERRIDE:true" \
"CC_FLUSHINDX_OVERRIDE:true" \
"CC_LOCKBASEADDR_OVERRIDE:true" \
"DDR_CONFIG_OVERRIDE:true" \
"DDRB_BUF_TIMER_OVERRIDE:true" \
"DDRB_CONFIG_OVERRIDE:true" \
"DDRB_NB_ADR_OVERRIDE:true" \
"DDRB_NB_SIZE_OVERRIDE:true" \
"EDAC_ENABLE_OVERRIDE:true" \
"EDAC_INT_ENABLE_OVERRIDE:true" \
"ENVM_CONFIG_OVERRIDE:true" \
"ENVM_FAB_REMAP_OVERRIDE:true" \
"ENVM_REMAP_BASE_OVERRIDE:true" \
"ESRAM_CONFIG_OVERRIDE:true" \
"ESRAM_MAX_LAT_OVERRIDE:true" \
"ESRAM_PIPELINE_CONFIG_OVERRIDE:true" \
"FAB_IF_OVERRIDE:true" \
"FABRIC_MEMPROT_BASE_ADDRESS:0x00000000" \
"FABRIC_MEMPROT_IS_USED:false" \
"FABRIC_MEMPROT_REGION_SIZE:8MB" \
"GPIN_SRC_SEL_OVERRIDE:true" \
"GPIO_SYSRESET_SEL_OVERRIDE:true" \
"LOOPBACK_CTRL_OVERRIDE:true" \
"M3_CONFIG_OVERRIDE:true" \
"MAC_CONFIG_OVERRIDE:true" \
"MASTER_WEIGHT_CONFIG0_OVERRIDE:true" \
"MASTER_WEIGHT_CONFIG1_OVERRIDE:true" \
"MDDR_CONFIG_OVERRIDE:true" \
"MDDR_IO_CALIB_OVERRIDE:true" \
"MM0_1_2_MS0_ALLOWED_R:true" \
"MM0_1_2_MS0_ALLOWED_W:true" \
"MM0_1_2_MS1_ALLOWED_R:true" \
"MM0_1_2_MS1_ALLOWED_W:true" \
"MM0_1_2_MS2_ALLOWED_R:true" \
"MM0_1_2_MS2_ALLOWED_W:true" \
"MM0_1_2_MS3_ALLOWED_R:true" \
"MM0_1_2_MS3_ALLOWED_W:true" \
"MM0_1_2_MS6_ALLOWED_R:true" \
"MM0_1_2_MS6_ALLOWED_W:true" \
"MM3_6_7_8_MS0_ALLOWED_R:true" \
"MM3_6_7_8_MS0_ALLOWED_W:true" \
"MM3_6_7_8_MS1_ALLOWED_R:true" \
"MM3_6_7_8_MS1_ALLOWED_W:true" \
"MM3_6_7_8_MS2_ALLOWED_R:true" \
"MM3_6_7_8_MS2_ALLOWED_W:true" \
"MM3_6_7_8_MS3_ALLOWED_R:true" \
"MM3_6_7_8_MS3_ALLOWED_W:true" \
"MM3_6_7_8_MS6_ALLOWED_R:true" \
"MM3_6_7_8_MS6_ALLOWED_W:true" \
"MM4_5_FIC64_MS0_ALLOWED_R:true" \
"MM4_5_FIC64_MS0_ALLOWED_W:true" \
"MM4_5_FIC64_MS1_ALLOWED_R:true" \
"MM4_5_FIC64_MS1_ALLOWED_W:true" \
"MM4_5_FIC64_MS2_ALLOWED_R:true" \
"MM4_5_FIC64_MS2_ALLOWED_W:true" \
"MM4_5_FIC64_MS3_ALLOWED_R:true" \
"MM4_5_FIC64_MS3_ALLOWED_W:true" \
"MM4_5_FIC64_MS6_ALLOWED_R:true" \
"MM4_5_FIC64_MS6_ALLOWED_W:true" \
"MM9_MS0_ALLOWED_R:true" \
"MM9_MS0_ALLOWED_W:true" \
"MM9_MS1_ALLOWED_R:true" \
"MM9_MS1_ALLOWED_W:true" \
"MM9_MS2_ALLOWED_R:true" \
"MM9_MS2_ALLOWED_W:true" \
"MM9_MS3_ALLOWED_R:true" \
"MM9_MS3_ALLOWED_W:true" \
"MM9_MS6_ALLOWED_R:true" \
"MM9_MS6_ALLOWED_W:true" \
"MSS_INTERRUPT_ENABLE_OVERRIDE:true" \
"MSSDDR_CLK_CALIB_CONFIG_OVERRIDE:true" \
"MSSDDR_FACC_CONFIG_1_OVERRIDE:true" \
"MSSDDR_FACC_CONFIG_2_OVERRIDE:true" \
"MSSDDR_PLL_STATUS_HIGH_OVERRIDE:true" \
"MSSDDR_PLL_STATUS_LOW_OVERRIDE:true" \
"NVM0_LOWER_ALLOWED:true" \
"NVM0_LOWER_FABRIC_ACCESS:true" \
"NVM0_LOWER_M3ACCESS:true" \
"NVM0_LOWER_OTHERS_ACCESS:true" \
"NVM0_UPPER_ALLOWED:true" \
"NVM0_UPPER_FABRIC_ACCESS:true" \
"NVM0_UPPER_M3ACCESS:true" \
"NVM0_UPPER_OTHERS_ACCESS:true" \
"NVM1_LOWER_ALLOWED:true" \
"NVM1_LOWER_FABRIC_ACCESS:true" \
"NVM1_LOWER_M3ACCESS:true" \
"NVM1_LOWER_OTHERS_ACCESS:true" \
"NVM1_UPPER_ALLOWED:true" \
"NVM1_UPPER_FABRIC_ACCESS:true" \
"NVM1_UPPER_M3ACCESS:true" \
"NVM1_UPPER_OTHERS_ACCESS:true" \
"PERIPH_CLOCK_MUX_SEL_OVERRIDE:true" \
"PLL_LOCK_EN_OVERRIDE:true" \
"RTC_WAKEUP_CONFIG_OVERRIDE:true" \
"SOFT_INTERRUPT_OVERRIDE:true" \
"SOFTRESET_CAN_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_COMBLK_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_ENVM0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_ENVM1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_ESRAM0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_ESRAM1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_FIC32_0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_FIC32_1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_FPGA_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_G4SPI0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_G4SPI1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_HPDMA_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_I2C0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_I2C1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MAC_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MDDR_CTLR_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MDDR_FIC64_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MMUART0_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MMUART1_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MSS_GPIO_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_MSS_GPOUT_7_0_SOFT_RESET_OVERRIDE:true" \
"SOFTRESET_MSS_GPOUT_15_8_SOFT_RESET_OVERRIDE:true" \
"SOFTRESET_MSS_GPOUT_23_16_SOFT_RESET_OVERRIDE:true" \
"SOFTRESET_MSS_GPOUT_31_24_SOFT_RESET_OVERRIDE:true" \
"SOFTRESET_PDMA_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_TIMER_SOFTRESET_OVERRIDE:true" \
"SOFTRESET_USB_SOFTRESET_OVERRIDE:true" \
"SPARE_OUT_OVERRIDE:true" \
"USB_CONFIG_OVERRIDE:true" \
"USB_IO_INPUT_SEL_OVERRIDE:true" \
"WDOGCONFIG_OVERRIDE:true" }
mss_save_instance_config -component_name ${mss_name} -instance_name {SECURITY}

# Enabling/Disabling the instance 'SPI_0' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {SPI_0}

# Enabling/Disabling the instance 'SPI_1' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {SPI_1}

# Configuring the instance 'SWITCH' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {SWITCH} -params {\
"CORTEXM3_REMAP:ENVM" \
"ENVM_REMAP_BASE_ADDRESS:0x00000000" \
"ENVM_REMAP_FABRIC_BASE_ADDRESS:0x00000000" \
"ENVM_REMAP_FABRIC_IS_USED:false" \
"ENVM_REMAP_REGION_SIZE:256" \
"ESRAM_MAX_LAT_SW_MAX_LATENCY_ESRAM0:8" \
"ESRAM_MAX_LAT_SW_MAX_LATENCY_ESRAM1:8" \
"SW_WEIGHT_FAB_0:1" \
"SW_WEIGHT_FAB_1:1" \
"SW_WEIGHT_G:1" \
"SW_WEIGHT_GIGE:1" \
"SW_WEIGHT_HPDMA:1" \
"SW_WEIGHT_IC:1" \
"SW_WEIGHT_PDMA:1" \
"SW_WEIGHT_S:1" \
"SW_WEIGHT_USB:1" }
mss_save_instance_config -component_name ${mss_name} -instance_name {SWITCH}

# Enabling/Disabling the instance 'USB' in the MSS component
mss_enable_instance -component_name ${mss_name} -instance_name {USB}

# Configuring the instance 'USB' in the MSS component
mss_configure_instance -component_name ${mss_name} -instance_name {USB} -params {\
"DATA0_CONNECTION:DATA0_B" \
"DATA1_CONNECTION:DATA1_B" \
"DATA2_CONNECTION:DATA2_B" \
"DATA3_CONNECTION:DATA3_B" \
"DATA4_CONNECTION:DATA4_B" \
"DATA5_CONNECTION:DATA5_B" \
"DATA6_CONNECTION:DATA6_B" \
"DATA7_CONNECTION:DATA7_B" \
"DATA_WIDTH:7" \
"DDR_SELECT:DDR_SELECT_SDR" \
"DIR_CONNECTION:DIR_B" \
"NXT_CONNECTION:NXT_B" \
"STP_CONNECTION:STP_B" \
"ULPI_DDR_SELECTED:true" \
"ULPI_SELECTED:true" \
"USB_CONFIG:CONFIG_ULPI_B" \
"XCLK_CONNECTION:XCLK_B" }
mss_save_instance_config -component_name ${mss_name} -instance_name {USB}

# Enabling/Disabling the instance 'WATCHDOG' in the MSS component
mss_disable_instance -component_name ${mss_name} -instance_name {WATCHDOG}

# Generating the MSS component
generate_component -component_name ${mss_name}

# Exporting Component Description of MC_System_sb_MSS to TCL done
