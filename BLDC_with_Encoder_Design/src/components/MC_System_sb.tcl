# Creating SmartDesign "MC_System_sb"
set sd_name {MC_System_sb}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Scalar Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PREADYS0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PSLVERRS0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PREADYS1} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PSLVERRS1} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {DEVRST_N} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FAB_RESET_N} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_DIR} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_NXT} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_XCLK} -port_direction {IN} -port_is_pad {1}

sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PSELS0} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PENABLES} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PSELS1} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PWRITES} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FAB_CCC_GL1} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FAB_CCC_LOCK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FIC_0_CLK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FIC_0_LOCK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {GPIO_0_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {INIT_DONE} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {MSS_READY} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {POWER_ON_RESET_N} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_STP} -port_direction {OUT} -port_is_pad {1}


# Create top level Bus Ports
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PRDATAS0} -port_direction {IN} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PRDATAS1} -port_direction {IN} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PADDRS} -port_direction {OUT} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PWDATAS} -port_direction {OUT} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {USB_ULPI_DATA} -port_direction {INOUT} -port_range {[7:0]} -port_is_pad {1}

# Create top level Bus interface Ports
sd_create_bif_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {mirroredSlave} -port_bif_mapping {\
"PADDR:AMBA_SLAVE_1_PADDRS" \
"PSELx:AMBA_SLAVE_0_PSELS0" \
"PENABLE:AMBA_SLAVE_1_PENABLES" \
"PWRITE:AMBA_SLAVE_1_PWRITES" \
"PRDATA:AMBA_SLAVE_0_PRDATAS0" \
"PWDATA:AMBA_SLAVE_1_PWDATAS" \
"PREADY:AMBA_SLAVE_0_PREADYS0" \
"PSLVERR:AMBA_SLAVE_0_PSLVERRS0" } 

sd_create_bif_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {mirroredSlave} -port_bif_mapping {\
"PADDR:AMBA_SLAVE_1_PADDRS" \
"PSELx:AMBA_SLAVE_1_PSELS1" \
"PENABLE:AMBA_SLAVE_1_PENABLES" \
"PWRITE:AMBA_SLAVE_1_PWRITES" \
"PRDATA:AMBA_SLAVE_1_PRDATAS1" \
"PWDATA:AMBA_SLAVE_1_PWDATAS" \
"PREADY:AMBA_SLAVE_1_PREADYS1" \
"PSLVERR:AMBA_SLAVE_1_PSLVERRS1" } 

# Add CCC_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Actel:SgCore:FCCC:$FCCCver -instance_name {CCC_0}
# Exporting Parameters of instance CCC_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {CCC_0} -params {\
"ADVANCED_TAB_CHANGED:false" \
"CLK0_IS_USED:false" \
"CLK0_PAD_IS_USED:false" \
"CLK1_IS_USED:false" \
"CLK1_PAD_IS_USED:false" \
"CLK2_IS_USED:false" \
"CLK2_PAD_IS_USED:false" \
"CLK3_IS_USED:false" \
"CLK3_PAD_IS_USED:false" \
"DYN_CONF_IS_USED:false" \
"FAMILY:19" \
"GL0_BP_IN_0_FREQ:100" \
"GL0_BP_IN_0_SRC:IO_HARDWIRED_0" \
"GL0_BP_IN_1_FREQ:100" \
"GL0_BP_IN_1_SRC:IO_HARDWIRED_0" \
"GL0_FREQUENCY_LOCKED:true" \
"GL0_IN_0_SRC:PLL" \
"GL0_IN_1_SRC:UNUSED" \
"GL0_IS_INVERTED:false" \
"GL0_IS_USED:true" \
"GL0_OUT_0_FREQ:100" \
"GL0_OUT_1_FREQ:50" \
"GL0_OUT_IS_GATED:false" \
"GL0_PLL_IN_0_PHASE:0" \
"GL0_PLL_IN_1_PHASE:0" \
"GL1_BP_IN_0_FREQ:100" \
"GL1_BP_IN_0_SRC:IO_HARDWIRED_0" \
"GL1_BP_IN_1_FREQ:100" \
"GL1_BP_IN_1_SRC:IO_HARDWIRED_0" \
"GL1_FREQUENCY_LOCKED:false" \
"GL1_IN_0_SRC:PLL" \
"GL1_IN_1_SRC:UNUSED" \
"GL1_IS_INVERTED:false" \
"GL1_IS_USED:true" \
"GL1_OUT_0_FREQ:100" \
"GL1_OUT_1_FREQ:50" \
"GL1_OUT_IS_GATED:false" \
"GL1_PLL_IN_0_PHASE:0" \
"GL1_PLL_IN_1_PHASE:0" \
"GL2_BP_IN_0_FREQ:100" \
"GL2_BP_IN_0_SRC:IO_HARDWIRED_0" \
"GL2_BP_IN_1_FREQ:100" \
"GL2_BP_IN_1_SRC:IO_HARDWIRED_0" \
"GL2_FREQUENCY_LOCKED:false" \
"GL2_IN_0_SRC:PLL" \
"GL2_IN_1_SRC:UNUSED" \
"GL2_IS_INVERTED:false" \
"GL2_IS_USED:false" \
"GL2_OUT_0_FREQ:100" \
"GL2_OUT_1_FREQ:50" \
"GL2_OUT_IS_GATED:false" \
"GL2_PLL_IN_0_PHASE:0" \
"GL2_PLL_IN_1_PHASE:0" \
"GL3_BP_IN_0_FREQ:100" \
"GL3_BP_IN_0_SRC:IO_HARDWIRED_0" \
"GL3_BP_IN_1_FREQ:100" \
"GL3_BP_IN_1_SRC:IO_HARDWIRED_0" \
"GL3_FREQUENCY_LOCKED:false" \
"GL3_IN_0_SRC:PLL" \
"GL3_IN_1_SRC:UNUSED" \
"GL3_IS_INVERTED:false" \
"GL3_IS_USED:false" \
"GL3_OUT_0_FREQ:100" \
"GL3_OUT_1_FREQ:50" \
"GL3_OUT_IS_GATED:false" \
"GL3_PLL_IN_0_PHASE:0" \
"GL3_PLL_IN_1_PHASE:0" \
"GPD0_IS_USED:false" \
"GPD0_NOPIPE_RSTSYNC:true" \
"GPD0_SYNC_STYLE:G3STYLE_AND_NO_LOCK_RSTSYNC" \
"GPD1_IS_USED:false" \
"GPD1_NOPIPE_RSTSYNC:true" \
"GPD1_SYNC_STYLE:G3STYLE_AND_NO_LOCK_RSTSYNC" \
"GPD2_IS_USED:false" \
"GPD2_NOPIPE_RSTSYNC:true" \
"GPD2_SYNC_STYLE:G3STYLE_AND_NO_LOCK_RSTSYNC" \
"GPD3_IS_USED:false" \
"GPD3_NOPIPE_RSTSYNC:true" \
"GPD3_SYNC_STYLE:G3STYLE_AND_NO_LOCK_RSTSYNC" \
"GPD_EXPOSE_RESETS:false" \
"GPD_SYNC_STYLE:G3STYLE_AND_LOCK_RSTSYNC" \
"INIT:0000007FB8000044D74000318C6307C1F18C61EC0404040400301" \
"IO_HARDWIRED_0_IS_DIFF:false" \
"IO_HARDWIRED_1_IS_DIFF:false" \
"IO_HARDWIRED_2_IS_DIFF:false" \
"IO_HARDWIRED_3_IS_DIFF:false" \
"MODE_10V:false" \
"NGMUX0_HOLD_IS_USED:false" \
"NGMUX0_IS_USED:false" \
"NGMUX1_HOLD_IS_USED:false" \
"NGMUX1_IS_USED:false" \
"NGMUX2_HOLD_IS_USED:false" \
"NGMUX2_IS_USED:false" \
"NGMUX3_HOLD_IS_USED:false" \
"NGMUX3_IS_USED:false" \
"NGMUX_EXPOSE_HOLD:false" \
"PLL_DELAY:0" \
"PLL_EXPOSE_BYPASS:false" \
"PLL_EXPOSE_RESETS:false" \
"PLL_EXT_FB_GL:EXT_FB_GL0" \
"PLL_FB_SRC:CCC_INTERNAL" \
"PLL_IN_FREQ:50.000" \
"PLL_IN_SRC:OSC_50MHZ" \
"PLL_IS_USED:true" \
"PLL_LOCK_IND:1024" \
"PLL_LOCK_WND:32000" \
"PLL_SSM_DEPTH:0.5" \
"PLL_SSM_ENABLE:false" \
"PLL_SSM_FREQ:40" \
"PLL_SUPPLY_VOLTAGE:25_V" \
"PLL_VCO_TARGET:700" \
"RCOSC_1MHZ_IS_USED:false" \
"RCOSC_25_50MHZ_IS_USED:true" \
"SD_EXPORT_HIDDEN_PORTS:false" \
"VCOFREQUENCY:800.000" \
"XTLOSC_IS_USED:false" \
"Y0_IS_USED:false" \
"Y1_IS_USED:false" \
"Y2_IS_USED:false" \
"Y3_IS_USED:false" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {CCC_0}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {CCC_0:PLL_ARST_N} -value {VCC}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {CCC_0:PLL_POWERDOWN_N} -value {VCC}



# Add CoreAPB3_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Actel:DirectCore:CoreAPB3:$CoreAPB3ver -instance_name {CoreAPB3_0}
# Exporting Parameters of instance CoreAPB3_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {CoreAPB3_0} -params {\
"APB_DWIDTH:32" \
"APBSLOT0ENABLE:true" \
"APBSLOT1ENABLE:true" \
"APBSLOT2ENABLE:false" \
"APBSLOT3ENABLE:false" \
"APBSLOT4ENABLE:false" \
"APBSLOT5ENABLE:false" \
"APBSLOT6ENABLE:false" \
"APBSLOT7ENABLE:false" \
"APBSLOT8ENABLE:false" \
"APBSLOT9ENABLE:false" \
"APBSLOT10ENABLE:false" \
"APBSLOT11ENABLE:false" \
"APBSLOT12ENABLE:false" \
"APBSLOT13ENABLE:false" \
"APBSLOT14ENABLE:false" \
"APBSLOT15ENABLE:false" \
"FAMILY:19" \
"HDL_license:U" \
"IADDR_OPTION:0" \
"MADDR_BITS:16" \
"SC_0:false" \
"SC_1:false" \
"SC_2:false" \
"SC_3:false" \
"SC_4:false" \
"SC_5:false" \
"SC_6:false" \
"SC_7:false" \
"SC_8:false" \
"SC_9:false" \
"SC_10:false" \
"SC_11:false" \
"SC_12:false" \
"SC_13:false" \
"SC_14:false" \
"SC_15:false" \
"testbench:User" \
"UPR_NIBBLE_POSN:3" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {CoreAPB3_0}



# Add CORERESETP_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Actel:DirectCore:CoreResetP:$CoreResetPver -instance_name {CORERESETP_0}
# Exporting Parameters of instance CORERESETP_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {CORERESETP_0} -params {\
"DDR_WAIT:200" \
"DEVICE_090:0" \
"DEVICE_VOLTAGE:2" \
"ENABLE_SOFT_RESETS:false" \
"EXT_RESET_CFG:0" \
"FDDR_IN_USE:false" \
"MDDR_IN_USE:false" \
"SDIF0_IN_USE:false" \
"SDIF0_PCIE:false" \
"SDIF0_PCIE_HOTRESET:true" \
"SDIF0_PCIE_L2P2:true" \
"SDIF1_IN_USE:false" \
"SDIF1_PCIE:false" \
"SDIF1_PCIE_HOTRESET:true" \
"SDIF1_PCIE_L2P2:true" \
"SDIF2_IN_USE:false" \
"SDIF2_PCIE:false" \
"SDIF2_PCIE_HOTRESET:true" \
"SDIF2_PCIE_L2P2:true" \
"SDIF3_IN_USE:false" \
"SDIF3_PCIE:false" \
"SDIF3_PCIE_HOTRESET:true" \
"SDIF3_PCIE_L2P2:true" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {CORERESETP_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {CORERESETP_0:M3_RESET_N}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {CORERESETP_0:CONFIG1_DONE} -value {VCC}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {CORERESETP_0:CONFIG2_DONE} -value {VCC}



# Add FABOSC_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Actel:SgCore:OSC:$OSCver -instance_name {FABOSC_0}
# Exporting Parameters of instance FABOSC_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {FABOSC_0} -params {\
"FAMILY:19" \
"PARAM_IS_FALSE:false" \
"RCOSC_1MHZ_DRIVES_CCC:0" \
"RCOSC_1MHZ_DRIVES_FAB:0" \
"RCOSC_1MHZ_IS_USED:0" \
"RCOSC_25_50MHZ_DRIVES_CCC:1" \
"RCOSC_25_50MHZ_DRIVES_FAB:1" \
"RCOSC_25_50MHZ_IS_USED:1" \
"VOLTAGE_IS_1_2:true" \
"XTLOSC_DRIVES_CCC:0" \
"XTLOSC_DRIVES_FAB:0" \
"XTLOSC_FREQ:0.0" \
"XTLOSC_IS_USED:0" \
"XTLOSC_SRC:CRYSTAL" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {FABOSC_0}



# Add MC_System_sb_MSS_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {MC_System_sb_MSS} -instance_name {MC_System_sb_MSS_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_MSS_0:FIC_2_APB_M_PCLK}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_MSS_0:FIC_2_APB_MASTER}



# Add SYSRESET_POR instance
sd_instantiate_macro -sd_name ${sd_name} -macro_name {SYSRESET} -instance_name {SYSRESET_POR}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"CCC_0:GL0" "CORERESETP_0:CLK_BASE" "FIC_0_CLK" "MC_System_sb_MSS_0:MCCC_CLK_BASE" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CCC_0:GL1" "FAB_CCC_GL1" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CCC_0:LOCK" "FAB_CCC_LOCK" "FIC_0_LOCK" "MC_System_sb_MSS_0:MCCC_CLK_BASE_PLL_LOCK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:FAB_RESET_N" "FAB_RESET_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:FIC_2_APB_M_PRESET_N" "MC_System_sb_MSS_0:FIC_2_APB_M_PRESET_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:INIT_DONE" "INIT_DONE" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:MSS_HPMS_READY" "MSS_READY" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:POWER_ON_RESET_N" "POWER_ON_RESET_N" "SYSRESET_POR:POWER_ON_RESET_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:RCOSC_25_50MHZ" "FABOSC_0:RCOSC_25_50MHZ_O2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:RESET_N_F2M" "MC_System_sb_MSS_0:MSS_RESET_N_F2M" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CORERESETP_0:RESET_N_M2F" "MC_System_sb_MSS_0:MSS_RESET_N_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"DEVRST_N" "SYSRESET_POR:DEVRST_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"GPIO_0_M2F" "MC_System_sb_MSS_0:GPIO_0_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_MSS_0:USB_ULPI_DIR" "USB_ULPI_DIR" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_MSS_0:USB_ULPI_NXT" "USB_ULPI_NXT" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_MSS_0:USB_ULPI_STP" "USB_ULPI_STP" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_MSS_0:USB_ULPI_XCLK" "USB_ULPI_XCLK" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_MSS_0:USB_ULPI_DATA" "USB_ULPI_DATA" }

# Add bus interface net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"AMBA_SLAVE_0" "CoreAPB3_0:APBmslave0" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"AMBA_SLAVE_1" "CoreAPB3_0:APBmslave1" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CCC_0:RCOSC_25_50MHZ_CCC_IN" "FABOSC_0:RCOSC_25_50MHZ_CCC_OUT" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"CoreAPB3_0:APB3mmaster" "MC_System_sb_MSS_0:FIC_0_APB_MASTER" }

# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the SmartDesign 
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign "MC_System_sb"
generate_component -component_name ${sd_name}
