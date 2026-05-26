
#  read only
#create_clock  -name { MC_System_0/MC_System_sb_0/FABOSC_0/I_RCOSC_25_50MHZ/CLKOUT } -period 20.000 -waveform { 0.000 10.000 } { \
 #MC_System_0/MC_System_sb_0/FABOSC_0/I_RCOSC_25_50MHZ/CLKOUT } 
#
#create_generated_clock  -name { MC_System_0/MC_System_sb_0/CCC_0/GL0 } \
    #-divide_by 2  -multiply_by 4  \
    #-source { MC_System_0/MC_System_sb_0/CCC_0/CCC_INST/RCOSC_25_50MHZ }  \
    #{ MC_System_0/MC_System_sb_0/CCC_0/GL0  } 



set_input_delay -max 3.000 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SDI_I  } 
set_input_delay -min 1.300 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SDI_I  } 

set_input_delay -max 3.000 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { QA_I  } 
set_input_delay -min 1.300 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { QA_I  } 

set_input_delay -max 3.000 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { QB_I  } 
set_input_delay -min 1.300 -clock { MC_System_0/MC_System_sb_0/CCC_0/GL0 } { QB_I  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_WL_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_WL_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_UH_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_UH_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0} { A0_PWM_UL_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_UL_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_VH_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0} { A0_PWM_VH_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_VL_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0} { A0_PWM_VL_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_PWM_WH_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0} { A0_PWM_WH_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SDO_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SDO_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SCK_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_SCK_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0} { A0_ADC_CS_O  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_ADC_CS_O  } 

set_output_delay -max 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_RUNNING_LED  } 
set_output_delay -min 0.300 -clock {MC_System_0/MC_System_sb_0/CCC_0/GL0 } { A0_RUNNING_LED  } 