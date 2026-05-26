# Creating SmartDesign "BLDC_Encoder_Axis"
set sd_name {BLDC_Encoder_Axis}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Scalar Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sdi_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {penable_i_0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {psel_i_0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwrite_i_0} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {qa_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {qb_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {reset_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {sys_clk_i} -port_direction {IN}

sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_cs_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sck_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sdo_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pready_o_0} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pslverr_o_0} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_uh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_ul} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_vh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_vl} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_wh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_wl} -port_direction {OUT}


# Create top level Bus Ports
sd_create_bus_port -sd_name ${sd_name} -port_name {paddr_i_0} -port_direction {IN} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {pwdata_i_0} -port_direction {IN} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {prdata_o_0} -port_direction {OUT} -port_range {[31:0]}


# Create top level Bus interface Ports
sd_create_bif_port -sd_name ${sd_name} -port_name {BLDC_APB_IF} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {slave} -port_bif_mapping {\
"PADDR:paddr_i_0" \
"PSELx:psel_i_0" \
"PENABLE:penable_i_0" \
"PWRITE:pwrite_i_0" \
"PRDATA:prdata_o_0" \
"PWDATA:pwdata_i_0" \
"PREADY:pready_o_0" \
"PSLVERR:pslverr_o_0" } 

# Add adc_interface_795x_0 instance
sd_instantiate_hdl_module -sd_name ${sd_name} -hdl_module_name {adc_interface_795x} -hdl_file {hdl\adc_interface_795x.vhd} -instance_name {adc_interface_795x_0}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {adc_interface_795x_0:channels_to_sample_i} -value {0000000000000011}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch4_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch5_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch6_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch7_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch8_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch9_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch10_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch11_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch12_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch13_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch14_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {adc_interface_795x_0:result_ch15_o}



# Add ADC_SCALING_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:ADC_SCALING:$ADC_SCALINGver -instance_name {ADC_SCALING_0}
# Exporting Parameters of instance ADC_SCALING_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {ADC_SCALING_0} -params {\
"FAMILY:19" \
"g_ADC_RESULTS_WIDTH:12" \
"g_DEBUG:0" \
"g_NO_MCYCLE_PATH:2" \
"g_SIGNED:0" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {ADC_SCALING_0}



# Add apb3_en_if_0 instance
sd_instantiate_hdl_core -sd_name ${sd_name} -hdl_core_name {apb3_en_if} -instance_name {apb3_en_if_0}
# Exporting Parameters of instance apb3_en_if_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {apb3_en_if_0} -params {\
"g_ADC_CONFIG_REG_WIDTH:6" \
"g_ADC_OUT_WIDTH:12" \
"g_APB3_IF_DATA_WIDTH:32" \
"g_CONST_WIDTH:12" \
"g_STD_IO_WIDTH:18" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {apb3_en_if_0}
sd_update_instance -sd_name ${sd_name} -instance_name {apb3_en_if_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_en_if_0:adc_control_reg_val_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_en_if_0:adc_channel_ch0_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_en_if_0:adc_channel_ch1_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_en_if_0:adc_channel_ch2_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_en_if_0:adc_channel_ch3_o}



# Add ENCODER_INTERFACE_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:ENCODER_INTERFACE:$ENCODER_INTERFACEver -instance_name {ENCODER_INTERFACE_0}
# Exporting Parameters of instance ENCODER_INTERFACE_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {ENCODER_INTERFACE_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {ENCODER_INTERFACE_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {ENCODER_INTERFACE_0:dir_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {ENCODER_INTERFACE_0:speed_done_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {ENCODER_INTERFACE_0:speed_filter_done_o}



# Add FOC_TRANSFORMS_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:FOC_TRANSFORMS:$FOC_TRANSFORMSver -instance_name {FOC_TRANSFORMS_0}
# Exporting Parameters of instance FOC_TRANSFORMS_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {FOC_TRANSFORMS_0} -params {\
"FAMILY:19" \
"g_NO_MCYCLE_PATH:2" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {FOC_TRANSFORMS_0}



# Add OLMNG_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:OLMNG:$OLMNGver -instance_name {OLMNG_0}
# Exporting Parameters of instance OLMNG_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {OLMNG_0} -params {\
"FAMILY:19" \
"g_NO_MCYCLE_PATH:2" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {OLMNG_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {OLMNG_0:done_o}



# Add PI_CONTROLLER_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:PI_CONTROLLER:$PI_CONTROLLERver -instance_name {PI_CONTROLLER_0}
# Exporting Parameters of instance PI_CONTROLLER_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {PI_CONTROLLER_0} -params {\
"FAMILY:19" \
"g_NO_MCYCLE_PATH:2" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {PI_CONTROLLER_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PI_CONTROLLER_0:done_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PI_CONTROLLER_0:ymax_i} -value {001111111111111111}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PI_CONTROLLER_0:ymin_i} -value {101111111111111111}



# Add PWM3PH_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:PWM3PH:$PWM3PHver -instance_name {PWM3PH_0}
# Exporting Parameters of instance PWM3PH_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {PWM3PH_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {PWM3PH_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM3PH_0:PWM_AL_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM3PH_0:PWM_BL_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM3PH_0:PWM_CL_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PWM3PH_0:en_dual_trig_i} -value {GND}



# Add PWM_SCALING_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:PWM_SCALING:$PWM_SCALINGver -instance_name {PWM_SCALING_0}
# Exporting Parameters of instance PWM_SCALING_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {PWM_SCALING_0} -params {\
"FAMILY:19" \
"g_NO_MCYCLE_PATH:2" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {PWM_SCALING_0}



# Add RATE_LIMITER_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:RATE_LIMITER:$RATE_LIMITERver -instance_name {RATE_LIMITER_0}
# Exporting Parameters of instance RATE_LIMITER_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {RATE_LIMITER_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {RATE_LIMITER_0}



# Add SEQ_CONTROLLER_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:SEQ_CONTROLLER:$SEQ_CONTROLLERver -instance_name {SEQ_CONTROLLER_0}
# Exporting Parameters of instance SEQ_CONTROLLER_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {SEQ_CONTROLLER_0} -params {\
"FAMILY:19" \
"g_DEBUG:0" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {SEQ_CONTROLLER_0}



# Add SPEED_ID_IQ_PI_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:SPEED_ID_IQ_PI:$SPEED_ID_IQ_PIver -instance_name {SPEED_ID_IQ_PI_0}
# Exporting Parameters of instance SPEED_ID_IQ_PI_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {SPEED_ID_IQ_PI_0} -params {\
"FAMILY:19" \
"g_NO_MCYCLE_PATH:2" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {SPEED_ID_IQ_PI_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_done_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:id_init_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ymax_i} -value {001111111111111111}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ymin_i} -value {110000000000000000}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:id_pi_ref_input_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:idq_pi_ymax_i} -value {001111111111111111}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:idq_pi_ymin_i} -value {110000000000000000}



# Add SVM_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:SVM:$SVMver -instance_name {SVM_0}
# Exporting Parameters of instance SVM_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {SVM_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {SVM_0}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_results_rdy_i" "adc_interface_795x_0:done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:calib_done_o" "SEQ_CONTROLLER_0:calib_done_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:done_o" "FOC_TRANSFORMS_0:start_clarke_i" "RATE_LIMITER_0:start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:over_current_fault_o" "SEQ_CONTROLLER_0:fault_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:reset_i" "ENCODER_INTERFACE_0:reset_i" "FOC_TRANSFORMS_0:reset_i" "OLMNG_0:reset_i" "PI_CONTROLLER_0:reset_i" "PWM3PH_0:reset_i" "PWM_SCALING_0:reset_i" "RATE_LIMITER_0:reset_i" "SEQ_CONTROLLER_0:reset_i" "SPEED_ID_IQ_PI_0:reset_i" "SVM_0:reset_i" "adc_interface_795x_0:reset_i" "apb3_en_if_0:preset_i" "reset_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:sys_clk_i" "ENCODER_INTERFACE_0:sys_clk_i" "FOC_TRANSFORMS_0:sys_clk_i" "OLMNG_0:sys_clk_i" "PI_CONTROLLER_0:sys_clk_i" "PWM3PH_0:sys_clk_i" "PWM_SCALING_0:sys_clk_i" "RATE_LIMITER_0:sys_clk_i" "SEQ_CONTROLLER_0:sys_clk_i" "SPEED_ID_IQ_PI_0:sys_clk_i" "SVM_0:sys_clk_i" "adc_interface_795x_0:sys_clk_i" "apb3_en_if_0:pclk_i" "sys_clk_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:clear_buffer_i" "OLMNG_0:clear_buffer_i" "PI_CONTROLLER_0:clear_buffer_i" "SEQ_CONTROLLER_0:motor_stopped_o" "SPEED_ID_IQ_PI_0:clear_buffer_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:direction_config_i" "OLMNG_0:direction_config_i" "RATE_LIMITER_0:direction_i" "apb3_en_if_0:direction_config_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:fil_trig_i" "PWM3PH_0:midmatch_o" "SEQ_CONTROLLER_0:pwm_midmatch_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:qa_i" "qa_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:qb_i" "qb_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:sensor_reset_i" "SEQ_CONTROLLER_0:sensor_reset_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:clarke_done_o" "FOC_TRANSFORMS_0:start_park_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:iclarke_done_o" "SVM_0:start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:ipark_done_o" "FOC_TRANSFORMS_0:start_iclarke_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:park_done_o" "SPEED_ID_IQ_PI_0:iq_pi_start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:start_ipark_i" "SPEED_ID_IQ_PI_0:id_pi_done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:cl_status_i" "SEQ_CONTROLLER_0:cl_status_o" "SPEED_ID_IQ_PI_0:speed_en_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:start_i" "PI_CONTROLLER_0:start_i" "RATE_LIMITER_0:rl_done_o" "SPEED_ID_IQ_PI_0:speed_pi_start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PI_CONTROLLER_0:pi_en_i" "apb3_en_if_0:position_pi_en_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_AH_o" "pwm_uh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_BH_o" "pwm_vh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_CH_o" "pwm_wh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:en_pwm_i" "SEQ_CONTROLLER_0:en_pwm_o" "pwm_ul" "pwm_vl" "pwm_wl" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:done_o" "SEQ_CONTROLLER_0:foc_done_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:start_i" "SVM_0:done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"RATE_LIMITER_0:reset_rl_i" "SEQ_CONTROLLER_0:reset_rl_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"RATE_LIMITER_0:soft_stop_i" "SEQ_CONTROLLER_0:soft_stop_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:adc_start_o" "adc_interface_795x_0:start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:en_idq_pi_o" "SPEED_ID_IQ_PI_0:idq_en_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:fault_clear_i" "apb3_en_if_0:fault_clr_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:start_motor_i" "apb3_en_if_0:start_motor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:stop_motor_i" "apb3_en_if_0:stop_motor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_start_i" "SPEED_ID_IQ_PI_0:iq_pi_done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_cs_o" "adc_interface_795x_0:cs_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sck_o" "adc_sck_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sdi_i" "adc_sdi_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sdo_o" "adc_sdo_o" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_result_ch0_i" "adc_interface_795x_0:result_ch0_o" "apb3_en_if_0:adc_ch0_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_result_ch1_i" "adc_interface_795x_0:result_ch1_o" "apb3_en_if_0:adc_ch1_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_scale_val_i" "apb3_en_if_0:adc_scale_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:fault_threshold_i" "apb3_en_if_0:adc_oc_thresh_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:ia_adc_o" "FOC_TRANSFORMS_0:ia_i" "apb3_en_if_0:ia_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:ib_adc_o" "FOC_TRANSFORMS_0:ib_i" "apb3_en_if_0:ib_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:angle_count_max_i" "apb3_en_if_0:angle_count_max_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:angle_factor_i" "apb3_en_if_0:angle_factor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:angle_out_o" "OLMNG_0:theta_cl_i" "apb3_en_if_0:en_angle_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:filter_factor_i" "apb3_en_if_0:en_filter_factor_omega_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:line_count_o" "PI_CONTROLLER_0:act_input_i" "apb3_en_if_0:rl_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:omega_out_o" "SPEED_ID_IQ_PI_0:speed_pi_act_input_i" "apb3_en_if_0:en_speed_filter_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:speed_factor_i" "apb3_en_if_0:speed_factor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ENCODER_INTERFACE_0:speed_window_i" "apb3_en_if_0:speed_window_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:ialpha_i" "FOC_TRANSFORMS_0:ialpha_o" "apb3_en_if_0:ialpha_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:ibeta_i" "FOC_TRANSFORMS_0:ibeta_o" "apb3_en_if_0:ibeta_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:id_o" "SPEED_ID_IQ_PI_0:id_pi_act_input_i" "apb3_en_if_0:id_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:iq_o" "SPEED_ID_IQ_PI_0:iq_pi_act_input_i" "apb3_en_if_0:iq_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:theta_i" "OLMNG_0:theta_o" "apb3_en_if_0:theta_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:va_o" "SVM_0:va_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:valpha_i" "FOC_TRANSFORMS_0:valpha_o" "apb3_en_if_0:valpha_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vb_o" "SVM_0:vb_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vbeta_i" "FOC_TRANSFORMS_0:vbeta_o" "apb3_en_if_0:vbeta_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vc_o" "SVM_0:vc_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vd_i" "SPEED_ID_IQ_PI_0:id_pi_output_o" "apb3_en_if_0:id_pi_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vq_i" "SEQ_CONTROLLER_0:vq_i" "SPEED_ID_IQ_PI_0:iq_pi_output_o" "apb3_en_if_0:iq_pi_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:dv_i" "apb3_en_if_0:olmng_dv_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:init_iqpi_o" "SPEED_ID_IQ_PI_0:iq_init_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:init_speedpi_o" "SPEED_ID_IQ_PI_0:speed_init_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:iq_ref_in_i" "apb3_en_if_0:olmng_iq_ref_in_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:speed_ref_i" "PI_CONTROLLER_0:init_i" "RATE_LIMITER_0:rl_out_o" "SEQ_CONTROLLER_0:speed_ref_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"OLMNG_0:theta_factor_i" "apb3_en_if_0:olmng_theta_factor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PI_CONTROLLER_0:ki_i" "apb3_en_if_0:position_pi_ki_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PI_CONTROLLER_0:kp_i" "apb3_en_if_0:position_pi_kp_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PI_CONTROLLER_0:output_y_o" "SPEED_ID_IQ_PI_0:speed_pi_ref_input_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PI_CONTROLLER_0:ref_input_i" "apb3_en_if_0:position_pi_ref_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:dead_time_i" "apb3_en_if_0:pwm_dead_time_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:delay_time_i" "apb3_en_if_0:pwm_delay_time_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:pwm_period_i" "PWM_SCALING_0:pwm_period_i" "apb3_en_if_0:pwm_period_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:va_i" "PWM_SCALING_0:Va_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:vb_i" "PWM_SCALING_0:Vb_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:vc_i" "PWM_SCALING_0:Vc_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:pwm_gain_i" "apb3_en_if_0:pwm_gain_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:va_i" "SVM_0:va3h_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:vb_i" "SVM_0:vb3h_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:vc_i" "SVM_0:vc3h_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"RATE_LIMITER_0:rate_count_i" "apb3_en_if_0:rate_limit_rate_cnt_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"RATE_LIMITER_0:rl_in_i" "apb3_en_if_0:rate_limit_ref_in_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"RATE_LIMITER_0:slew_count_i" "apb3_en_if_0:rate_limit_slew_cnt_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:config_reg_i" "apb3_en_if_0:seq_cntl_config_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:max_ar_i" "apb3_en_if_0:auto_restart_no_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:speed_cl_i" "apb3_en_if_0:seq_cntl_cl_omega_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:state_o" "apb3_en_if_0:fsm_state_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_ki_i" "SPEED_ID_IQ_PI_0:iq_pi_ki_i" "apb3_en_if_0:idq_pi_ki_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_kp_i" "SPEED_ID_IQ_PI_0:iq_pi_kp_i" "apb3_en_if_0:idq_pi_kp_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:iq_pi_ref_input_i" "SPEED_ID_IQ_PI_0:speed_pi_output_o" "apb3_en_if_0:speed_pi_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:speed_pi_ki_i" "apb3_en_if_0:speed_pi_ki_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:speed_pi_kp_i" "apb3_en_if_0:speed_pi_kp_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:result_ch2_o" "apb3_en_if_0:adc_ch2_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:result_ch3_o" "apb3_en_if_0:adc_ch3_val_i" }

# Add bus interface net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"BLDC_APB_IF" "apb3_en_if_0:APB3_IF" }

# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the SmartDesign 
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign "BLDC_Encoder_Axis"
generate_component -component_name ${sd_name}
