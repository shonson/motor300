# Creating SmartDesign "Stepper_Axis"
set sd_name {Stepper_Axis}
create_smartdesign -sd_name ${sd_name}

# Disable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 0

# Create top level Scalar Ports
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sdi_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {penable_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {psel_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwrite_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {reset_i} -port_direction {IN}
sd_create_scalar_port -sd_name ${sd_name} -port_name {sys_clk_i} -port_direction {IN}

sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_cs_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sck_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {adc_sdo_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pready_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pslverr_o} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_uh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_ul} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_vh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_vl} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_wh} -port_direction {OUT}
sd_create_scalar_port -sd_name ${sd_name} -port_name {pwm_wl} -port_direction {OUT}


# Create top level Bus Ports
sd_create_bus_port -sd_name ${sd_name} -port_name {paddr_i} -port_direction {IN} -port_range {[31:0]}
sd_create_bus_port -sd_name ${sd_name} -port_name {pwdata_i} -port_direction {IN} -port_range {[31:0]}

sd_create_bus_port -sd_name ${sd_name} -port_name {prdata_o} -port_direction {OUT} -port_range {[31:0]}


# Create top level Bus interface Ports
sd_create_bif_port -sd_name ${sd_name} -port_name {STEPPER_APB_IF} -port_bif_vlnv {AMBA:AMBA2:APB:r0p0} -port_bif_role {slave} -port_bif_mapping {\
"PADDR:paddr_i" \
"PSELx:psel_i" \
"PENABLE:penable_i" \
"PWRITE:pwrite_i" \
"PRDATA:prdata_o" \
"PWDATA:pwdata_i" \
"PREADY:pready_o" \
"PSLVERR:pslverr_o" } 

# Add adc_interface_795x_0 instance
sd_instantiate_hdl_module -sd_name ${sd_name} -hdl_module_name {adc_interface_795x} -hdl_file {hdl\adc_interface_795x.vhd} -instance_name {adc_interface_795x_0}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {adc_interface_795x_0:channels_to_sample_i} -value {0000000000000101}
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



# Add apb3_if_st_0 instance
sd_instantiate_hdl_core -sd_name ${sd_name} -hdl_core_name {apb3_if_st} -instance_name {apb3_if_st_0}
# Exporting Parameters of instance apb3_if_st_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {apb3_if_st_0} -params {\
"g_ADC_CONFIG_REG_WIDTH:5" \
"g_APB3_IF_DATA_WIDTH:32" \
"g_CONST_WIDTH:12" \
"g_STD_IO_WIDTH:18" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {apb3_if_st_0}
sd_update_instance -sd_name ${sd_name} -instance_name {apb3_if_st_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_if_st_0:adc_control_reg_val_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_if_st_0:adc_channel_ch0_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_if_st_0:adc_channel_ch1_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_if_st_0:adc_channel_ch2_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {apb3_if_st_0:adc_channel_ch3_o}



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
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:start_clarke_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:start_iclarke_i} -value {GND}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:clarke_done_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:iclarke_done_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:ia_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:ib_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:valpha_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:vbeta_i} -value {GND}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:ialpha_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:ibeta_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:va_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:vb_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {FOC_TRANSFORMS_0:vc_o}



# Add PWM3PH_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:PWM3PH:$PWM3PHver -instance_name {PWM3PH_0}
# Exporting Parameters of instance PWM3PH_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {PWM3PH_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {PWM3PH_0}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM3PH_0:PWM_CH_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM3PH_0:PWM_CL_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PWM3PH_0:en_dual_trig_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PWM3PH_0:vc_i} -value {GND}



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
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {PWM_SCALING_0:vc_i} -value {GND}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {PWM_SCALING_0:Vc_o}



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
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:cl_status_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:reset_rl_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:soft_stop_o}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:sensor_reset_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:speed_ref_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:speed_cl_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:vq_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SEQ_CONTROLLER_0:max_ar_i} -value {GND}



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
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_en_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_start_i} -value {GND}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_done_o}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_init_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:iq_init_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ref_input_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_act_input_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_kp_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ki_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ymax_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_ymin_i} -value {GND}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:idq_pi_ymax_i} -value {010000000000000000}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:idq_pi_ymin_i} -value {110000000000000000}
sd_connect_pins_to_constant -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:iq_pi_ref_input_i} -value {GND}
sd_mark_pins_unused -sd_name ${sd_name} -pin_names {SPEED_ID_IQ_PI_0:speed_pi_output_o}



# Add STEPPER_THETA_0 instance
sd_instantiate_core -sd_name ${sd_name} -core_vlnv Microsemi:SolutionCore:STEPPER_THETA:$STEPPER_THETAver -instance_name {STEPPER_THETA_0}
# Exporting Parameters of instance STEPPER_THETA_0
sd_configure_core_instance -sd_name ${sd_name} -instance_name {STEPPER_THETA_0} -params {\
"FAMILY:19" \
"HDL_License:O" \
"testbench:User" }\
-validate_rules 0
sd_save_core_instance_config -sd_name ${sd_name} -instance_name {STEPPER_THETA_0}



# Add scalar net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_results_rdy_i" "adc_interface_795x_0:done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:calib_done_o" "SEQ_CONTROLLER_0:calib_done_i" "STEPPER_THETA_0:enable_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:done_o" "FOC_TRANSFORMS_0:start_park_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:over_current_fault_o" "SEQ_CONTROLLER_0:fault_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:reset_i" "FOC_TRANSFORMS_0:reset_i" "PWM3PH_0:reset_i" "PWM_SCALING_0:reset_i" "SEQ_CONTROLLER_0:reset_i" "SPEED_ID_IQ_PI_0:reset_i" "STEPPER_THETA_0:reset_i" "adc_interface_795x_0:reset_i" "apb3_if_st_0:preset_i" "reset_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:sys_clk_i" "FOC_TRANSFORMS_0:sys_clk_i" "PWM3PH_0:sys_clk_i" "PWM_SCALING_0:sys_clk_i" "SEQ_CONTROLLER_0:sys_clk_i" "SPEED_ID_IQ_PI_0:sys_clk_i" "STEPPER_THETA_0:sys_clk_i" "adc_interface_795x_0:sys_clk_i" "apb3_if_st_0:pclk_i" "sys_clk_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:ipark_done_o" "PWM_SCALING_0:start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:park_done_o" "SPEED_ID_IQ_PI_0:iq_pi_start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:start_ipark_i" "SPEED_ID_IQ_PI_0:id_pi_done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_AH_o" "pwm_uh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_AL_o" "pwm_ul" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_BH_o" "pwm_vh" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:PWM_BL_o" "pwm_vl" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:en_pwm_i" "SEQ_CONTROLLER_0:en_pwm_o" "pwm_wh" "pwm_wl" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:midmatch_o" "SEQ_CONTROLLER_0:pwm_midmatch_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM_SCALING_0:done_o" "SEQ_CONTROLLER_0:foc_done_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:adc_start_o" "adc_interface_795x_0:start_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:en_idq_pi_o" "SPEED_ID_IQ_PI_0:idq_en_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:fault_clear_i" "SEQ_CONTROLLER_0:stop_motor_i" "apb3_if_st_0:stop_motor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:motor_stopped_o" "SPEED_ID_IQ_PI_0:clear_buffer_i" "STEPPER_THETA_0:clear_buffer_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:start_motor_i" "apb3_if_st_0:start_motor_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_start_i" "SPEED_ID_IQ_PI_0:iq_pi_done_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_THETA_0:mode_i" "apb3_if_st_0:theta_gen_mode_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_cs_o" "adc_interface_795x_0:cs_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sck_o" "adc_sck_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sdi_i" "adc_sdi_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:sdo_o" "adc_sdo_o" }

# Add bus net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_result_ch0_i" "adc_interface_795x_0:result_ch0_o" "apb3_if_st_0:adc_ch0_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_result_ch1_i" "adc_interface_795x_0:result_ch2_o" "apb3_if_st_0:adc_ch2_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:adc_scale_val_i" "apb3_if_st_0:adc_scale_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:fault_threshold_i" "apb3_if_st_0:adc_oc_thresh_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:ia_adc_o" "FOC_TRANSFORMS_0:ialpha_i" "apb3_if_st_0:ia_clarke_in_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"ADC_SCALING_0:ib_adc_o" "FOC_TRANSFORMS_0:ibeta_i" "apb3_if_st_0:ib_clarke_in_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:id_o" "SPEED_ID_IQ_PI_0:id_pi_act_input_i" "apb3_if_st_0:id_park_output_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:iq_o" "SPEED_ID_IQ_PI_0:iq_pi_act_input_i" "apb3_if_st_0:iq_park_output_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:theta_i" "STEPPER_THETA_0:theta_output_o" "apb3_if_st_0:theta_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:valpha_o" "PWM_SCALING_0:va_i" "apb3_if_st_0:valpha_ipark_output_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vbeta_o" "PWM_SCALING_0:vb_i" "apb3_if_st_0:vbeta_ipark_output_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vd_i" "SPEED_ID_IQ_PI_0:id_pi_output_o" "apb3_if_st_0:id_pi_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"FOC_TRANSFORMS_0:vq_i" "SPEED_ID_IQ_PI_0:iq_pi_output_o" "apb3_if_st_0:iq_pi_out_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:dead_time_i" "apb3_if_st_0:pwm_dead_time_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:delay_time_i" "apb3_if_st_0:pwm_delay_time_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:pwm_period_i" "PWM_SCALING_0:pwm_gain_i" "PWM_SCALING_0:pwm_period_i" "apb3_if_st_0:pwm_period_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:va_i" "PWM_SCALING_0:Va_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"PWM3PH_0:vb_i" "PWM_SCALING_0:Vb_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:config_reg_i" "apb3_if_st_0:seq_cntl_config_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SEQ_CONTROLLER_0:state_o" "apb3_if_st_0:state_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_init_i" "apb3_if_st_0:olmng_dv_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_ki_i" "SPEED_ID_IQ_PI_0:iq_pi_ki_i" "apb3_if_st_0:idq_pi_ki_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_kp_i" "SPEED_ID_IQ_PI_0:iq_pi_kp_i" "apb3_if_st_0:idq_pi_kp_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"SPEED_ID_IQ_PI_0:id_pi_ref_input_i" "apb3_if_st_0:id_ref_val_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_THETA_0:cmd_step_no_i" "apb3_if_st_0:theta_gen_cmd_step_no_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_THETA_0:rate_limit_i" "apb3_if_st_0:theta_gen_rate_limit_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_THETA_0:slew_cnt_i" "apb3_if_st_0:theta_gen_slew_count_o" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_THETA_0:step_count_o" "apb3_if_st_0:step_cnt_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:result_ch1_o" "apb3_if_st_0:adc_ch1_val_i" }
sd_connect_pins -sd_name ${sd_name} -pin_names {"adc_interface_795x_0:result_ch3_o" "apb3_if_st_0:adc_ch3_val_i" }

# Add bus interface net connections
sd_connect_pins -sd_name ${sd_name} -pin_names {"STEPPER_APB_IF" "apb3_if_st_0:STEPPER_APB_IF" }

# Re-enable auto promotion of pins of type 'pad'
auto_promote_pad_pins -promote_all 1
# Save the SmartDesign 
save_smartdesign -sd_name ${sd_name}
# Generate SmartDesign "Stepper_Axis"
generate_component -component_name ${sd_name}
