################################################################################################################################################
# © 2024 Microchip Technology Inc. and its subsidiaries
# All rights reserved.
# ANY USE OR REDISTRIBUTION IN PART OR IN WHOLE MUST BE HANDLED IN
# ACCORDANCE WITH THE MICROCHIP LICENSE AGREEMENT AND MUST BE APPROVED
# IN ADVANCE IN WRITING
##################################################################################################################################################
# Title        : script.tcl
# Description  : This is the top-level script, which creates the FPGA HW Project. 
#                It invokes all the below TCLs which are required to complete the FPGA design flow.
#         	      -- 1_create_design.tcl: Executes all the required core configuration TCLs with necessary user HDL files to generate the design.
#		          -- 2_constrain_design.tcl: Applies all the necessary constraints for the design.
#      		      -- 3_sim_flow.tcl : Imports the testbench, other required simulation files and simulates the design.
#		          -- 4_implement_design.tcl : Runs synthesis, place & route, and verify timing design flow steps.
#        		  -- 5_program_design.tcl : Creates and exports the programming job file.              
####################################################################################################################################################

# Call common.tcl which contains required cores with the versions numbers to run the TCL scripts
source ./common/common.tcl

# A common procedure to compute the runtime for each command run in Libero.
# Get tool name from the command
proc run_tool_wrapper { cmd } {
  regexp {run_tool\s+-name\s+\{*(\w*)\}*} $cmd full1 tool;
  puts "Starting $tool command";
  set full_cmd "time \{ $cmd \}";
  set TIME_start [clock seconds];
  set runtime [ eval $full_cmd ];
  set TIME_taken [expr [clock seconds] - $TIME_start];
  puts "\nRUNTIME:$tool=$TIME_taken secs\n";
  set runtime_secs [ expr [lindex $runtime 0]/1000000 ];
  puts "\nRUNTIME_bytime:$tool=$runtime_secs secs\n";
}

# Device-specific Variables
set die_advdev {M2S010}
#set die_eval {M2S090TS}
set advdev_package {484 FBGA}
#set eval_package {484 FBGA} 
#set eval_part_range {COM}
set advdev_part_range {COM}

set Prjname "SK1ABLEN_2024"
set PrjLocation "./$Prjname"

#variable used in the design
set SimTime 500us
set Effort_Level true
set Repair_Min_Delay true
set Multi_Pass_Layout false


# Remove existing project if present
file delete -force ${PrjLocation}

# Create and configure new project
new_project \
    -name "$Prjname" \
    -location "$PrjLocation" \
    -family "SmartFusion2" \
    -die $die_advdev \
    -package $advdev_package \
    -die_voltage "1.2" \
	-adv_options {PLL_SUPPLY:PLL_SUPPLY_33} \
    -speed "-1" \
    -part_range $advdev_part_range \
    -hdl "VERILOG" \
    -adv_options {SYSTEM_CONTROLLER_SUSPEND_MODE:0}


puts "Project created successfully"

# Core Downloads

download_core -vlnv {Microsemi:SolutionCore:ADC_SCALING:4.2.0} -location {www.microchip-ip.com/repositories/DirectCore}       
download_core -vlnv {Microsemi:SolutionCore:ENCODER_INTERFACE:4.1.1} -location {www.microchip-ip.com/repositories/DirectCore} 
download_core -vlnv {Microsemi:SolutionCore:FOC_TRANSFORMS:4.1.1} -location {www.microchip-ip.com/repositories/DirectCore}    
download_core -vlnv {Microsemi:SolutionCore:OLMNG:4.1.1} -location {www.microchip-ip.com/repositories/DirectCore}             
download_core -vlnv {Microsemi:SolutionCore:PI_CONTROLLER:4.1.0} -location {www.microchip-ip.com/repositories/DirectCore}     
download_core -vlnv {Microsemi:SolutionCore:PWM3PH:4.1.2} -location {www.microchip-ip.com/repositories/DirectCore}            
download_core -vlnv {Microsemi:SolutionCore:PWM_SCALING:4.1.0} -location {www.microchip-ip.com/repositories/DirectCore}       
download_core -vlnv {Microsemi:SolutionCore:RATE_LIMITER:4.1.0} -location {www.microchip-ip.com/repositories/DirectCore}      
download_core -vlnv {Microsemi:SolutionCore:SEQ_CONTROLLER:4.1.1} -location {www.microchip-ip.com/repositories/DirectCore}    
download_core -vlnv {Microsemi:SolutionCore:SPEED_ID_IQ_PI:4.1.0} -location {www.microchip-ip.com/repositories/DirectCore}    
download_core -vlnv {Microsemi:SolutionCore:SVM:4.1.0} -location {www.microchip-ip.com/repositories/DirectCore}               
download_core -vlnv {Actel:SgCore:FCCC:2.0.201} -location {www.microchip-ip.com/repositories/SgCore}                      
download_core -vlnv {Actel:DirectCore:CoreAPB3:4.2.100} -location {www.microchip-ip.com/repositories/DirectCore}              
download_core -vlnv {Actel:DirectCore:CoreResetP:7.1.100} -location {www.microchip-ip.com/repositories/DirectCore}            
download_core -vlnv {Actel:SgCore:OSC:2.0.101} -location {www.microchip-ip.com/repositories/SgCore}                       
download_core -vlnv {Actel:SmartFusion2MSS:MSS:1.1.500} -location {www.microchip-ip.com/repositories/SgCore}    


# Create design
source ./src/1_create_design.tcl

# Constrain design
source ./src/2_constrain_design.tcl

# Implement design
source ./src/4_implement_design.tcl

# Program design
source ./src/5_program_design.tcl

file delete -force SK1ABLEN_2024_log.txt

save_log -file SK1ABLEN_2024_log.txt

# Close project
close_project -save 1 
