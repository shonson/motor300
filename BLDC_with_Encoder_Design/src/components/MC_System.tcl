# Creating SmartDesign "MC_System"
set sd_name {MC_System}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Scalar Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PREADYS0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PSLVERRS0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PREADYS1} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PSLVERRS1} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {DEVRST_N} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_DIR} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_NXT} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_XCLK} -port_direction {IN} -port_is_pad {1}

sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PENABLES} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PSELS0} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PWRITES} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PSELS1} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FAB_CCC_GL1} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FIC_0_CLK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {FIC_0_LOCK} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {GPIO_0_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_STP} -port_direction {OUT} -port_is_pad {1}


# Create top level Bus Ports
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PRDATAS0} -port_direction {IN} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1_PRDATAS1} -port_direction {IN} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PADDRS} -port_direction {OUT} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0_PWDATAS} -port_direction {OUT} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {USB_ULPI_DATA} -port_direction {INOUT} -port_range {[7:0]} -port_is_pad {1}

# Create top level Bus interface Ports
sd_create_bif_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_0} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {mirroredSlave} -port_bif_mapping {\
"PADDR:AMBA_SLAVE_0_PADDRS" \
"PSELx:AMBA_SLAVE_0_PSELS0" \
"PENABLE:AMBA_SLAVE_0_PENABLES" \
"PWRITE:AMBA_SLAVE_0_PWRITES" \
"PRDATA:AMBA_SLAVE_0_PRDATAS0" \
"PWDATA:AMBA_SLAVE_0_PWDATAS" \
"PREADY:AMBA_SLAVE_0_PREADYS0" \
"PSLVERR:AMBA_SLAVE_0_PSLVERRS0" } 

sd_create_bif_port -sd_name ${sd_name} -port_name {AMBA_SLAVE_1} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {mirroredSlave} -port_bif_mapping {\
"PADDR:AMBA_SLAVE_0_PADDRS" \
"PSELx:AMBA_SLAVE_1_PSELS1" \
"PENABLE:AMBA_SLAVE_0_PENABLES" \
"PWRITE:AMBA_SLAVE_0_PWRITES" \
"PRDATA:AMBA_SLAVE_1_PRDATAS1" \
"PWDATA:AMBA_SLAVE_0_PWDATAS" \
"PREADY:AMBA_SLAVE_1_PREADYS1" \
"PSLVERR:AMBA_SLAVE_1_PSLVERRS1" } 

# Add MC_System_sb_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {MC_System_sb} -instance_name {MC_System_sb_0}
sd_create_pin_group -sd_name ${sd_name} -group_name {USB_ULPI_PADS} -instance_name {MC_System_sb_0} -pin_names {"USB_ULPI_DIR" "USB_ULPI_NXT" "USB_ULPI_STP" "USB_ULPI_DATA" "USB_ULPI_XCLK" }
sd_create_pin_group -sd_name ${sd_name} -group_name {FIC_0_PINS} -instance_name {MC_System_sb_0} -pin_names {"FIC_0_CLK" "FIC_0_LOCK" }
sd_create_pin_group -sd_name ${sd_name} -group_name {FAB_CCC_PINS} -instance_name {MC_System_sb_0} -pin_names {"FAB_CCC_GL1" "FAB_CCC_LOCK" }
sd_create_pin_group -sd_name ${sd_name} -group_name {GPIO_FABRIC} -instance_name {MC_System_sb_0} -pin_names {"GPIO_0_M2F" }
sd_create_pin_group -sd_name ${sd_name} -group_name {INIT_PINS} -instance_name {MC_System_sb_0} -pin_names {"INIT_DONE" }
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {MC_System_sb_0:FAB_RESET_N} -value {VCC}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_0:FAB_CCC_LOCK}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_0:INIT_DONE}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_0:MSS_READY}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_sb_0:POWER_ON_RESET_N}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"DEVRST_N" "MC_System_sb_0:DEVRST_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FAB_CCC_GL1" "MC_System_sb_0:FAB_CCC_GL1" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FIC_0_CLK" "MC_System_sb_0:FIC_0_CLK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FIC_0_LOCK" "MC_System_sb_0:FIC_0_LOCK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"GPIO_0_M2F" "MC_System_sb_0:GPIO_0_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_0:USB_ULPI_DIR" "USB_ULPI_DIR" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_0:USB_ULPI_NXT" "USB_ULPI_NXT" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_0:USB_ULPI_STP" "USB_ULPI_STP" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_0:USB_ULPI_XCLK" "USB_ULPI_XCLK" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_sb_0:USB_ULPI_DATA" "USB_ULPI_DATA" }

# Add bus interface net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"AMBA_SLAVE_0" "MC_System_sb_0:AMBA_SLAVE_0" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"AMBA_SLAVE_1" "MC_System_sb_0:AMBA_SLAVE_1" }

# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the SmartDesign 
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign "MC_System"
generate_component -component_name ${sd_name}
