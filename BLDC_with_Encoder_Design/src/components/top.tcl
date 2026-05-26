# Creating SmartDesign "top"
set sd_name {top}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Scalar Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_ADC_SDI_I} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {DEVRST_N} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {QA_I} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {QB_I} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_DIR} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_NXT} -port_direction {IN} -port_is_pad {1}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_XCLK} -port_direction {IN} -port_is_pad {1}

sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_ADC_CS_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_ADC_SCK_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_ADC_SDO_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_UH_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_UL_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_VH_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_VL_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_WH_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_PWM_WL_O} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {A0_RUNNING_LED} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {GPIO_0_M2F} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {USB_ULPI_STP} -port_direction {OUT} -port_is_pad {1}


# Create top level Bus Ports
sd_create_bus_port -sd_name ${sd_name} -port_name {USB_ULPI_DATA} -port_direction {INOUT} -port_range {[7:0]} -port_is_pad {1}

# Add BLDC_Encoder_Axis_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {BLDC_Encoder_Axis} -instance_name {BLDC_Encoder_Axis_0}



# Add MC_System_0 instance
sd_instantiate_component -sd_name ${sd_name} -component_name {MC_System} -instance_name {MC_System_0}
sd_create_pin_group -sd_name ${sd_name} -group_name {USB_ULPI_PADS} -instance_name {MC_System_0} -pin_names {"USB_ULPI_DIR" "USB_ULPI_NXT" "USB_ULPI_STP" "USB_ULPI_DATA" "USB_ULPI_XCLK" }
sd_create_pin_group -sd_name ${sd_name} -group_name {GPIO_FABRIC} -instance_name {MC_System_0} -pin_names {"GPIO_0_M2F" }
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_0:FAB_CCC_GL1}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {MC_System_0:AMBA_SLAVE_1}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_ADC_CS_O" "BLDC_Encoder_Axis_0:adc_cs_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_ADC_SCK_O" "BLDC_Encoder_Axis_0:adc_sck_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_ADC_SDI_I" "BLDC_Encoder_Axis_0:adc_sdi_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_ADC_SDO_O" "BLDC_Encoder_Axis_0:adc_sdo_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_UH_O" "BLDC_Encoder_Axis_0:pwm_uh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_UL_O" "BLDC_Encoder_Axis_0:pwm_ul" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_VH_O" "BLDC_Encoder_Axis_0:pwm_vh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_VL_O" "A0_RUNNING_LED" "BLDC_Encoder_Axis_0:pwm_vl" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_WH_O" "BLDC_Encoder_Axis_0:pwm_wh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"A0_PWM_WL_O" "BLDC_Encoder_Axis_0:pwm_wl" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_Encoder_Axis_0:qa_i" "QA_I" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_Encoder_Axis_0:qb_i" "QB_I" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_Encoder_Axis_0:reset_i" "MC_System_0:FIC_0_LOCK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_Encoder_Axis_0:sys_clk_i" "MC_System_0:FIC_0_CLK" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"DEVRST_N" "MC_System_0:DEVRST_N" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"GPIO_0_M2F" "MC_System_0:GPIO_0_M2F" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_0:USB_ULPI_DIR" "USB_ULPI_DIR" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_0:USB_ULPI_NXT" "USB_ULPI_NXT" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_0:USB_ULPI_STP" "USB_ULPI_STP" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_0:USB_ULPI_XCLK" "USB_ULPI_XCLK" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"MC_System_0:USB_ULPI_DATA" "USB_ULPI_DATA" }

# Add bus interface net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_Encoder_Axis_0:BLDC_APB_IF" "MC_System_0:AMBA_SLAVE_0" }

# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the SmartDesign 
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign "top"
generate_component -component_name ${sd_name}
