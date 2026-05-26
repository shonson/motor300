
file copy -force "./src/softconsole/MotorControl_5_1.hex" "./$Prjname/MotorControl_5_1.hex"

import_component  -file {./src/cxf/MC_System_sb_MSS.cxf}

generate_component -component_name MC_System_sb_MSS

import_files -hdl_source {./src/hdl/adc_interface_795x.vhd}
import_files -hdl_source {./src/hdl/apb3_en_if.vhd}

# Import source files
build_design_hierarchy 

# Create, configure and generate core components
source ./src/components/apb3_en_if.tcl
source ./src/components/BLDC_Encoder_Axis.tcl
#source ./src/components/MC_System_sb_MSS.tcl
source ./src/components/MC_System_sb.tcl 
source ./src/components/MC_System.tcl 

# Generate SmartDesign Components
build_design_hierarchy 
source ./src/components/top.tcl 

# Set top level module
build_design_hierarchy 
set_root -module {top::work} 

save_project
puts "Design generated successfully\n"
