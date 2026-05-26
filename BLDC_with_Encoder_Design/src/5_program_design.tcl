# Generate FPGA Array Data
## run_tool -name GENERATEPROGRAMMINGDATA
run_tool_wrapper "run_tool -name GENERATEPROGRAMMINGDATA"

                  
# Configure and generate Design Initialization Data and Memories
# The following can be configured:
#   - Design initialization source - sNVM/uPROM/SPI-Flash
#   - sNVM user clients
#   - uPROM user clients
#   - Fabric RAM initialization content
#   - SPI-Flash user clients
# Examples TBD
#
# Example for configuring user snvm clients 
# Note that if using relative path, the path to the mem file specified in the SNVM.cfg file is relative to the libero *.prjx file. So please adjust the path according to the project location and where the SNVM.cfg and mem files are located. 
#configure_snvm -cfg_file {../snvm/SNVM.cfg} 

#configure_envm -cfg_file {./src/cfg/ENVM.cfg} 

#run_tool_wrapper "run_tool -name {UPDATE_ENVM} -script {./src/cxf/ENVM.cfg}"  

# -script "./src/cfg/ENVM.cfg""


#generate_design_initialization_data
    
configure_tool \
         -name {GENERATEPROGRAMMINGFILE} \
         -params {program_envm:false} \
         -params {program_fabric:false} \
         -params {program_mode:selected_features} \
         -params {program_security:false} \
         -params {sanitize_envm:false} 
		 
## run_tool -name GENERATEPROGRAMMINGFILE
run_tool_wrapper "run_tool -name GENERATEPROGRAMMINGFILE"

puts "Programmingfile generated successfully\n"

# Export STAPL file
export_bitstream_file \
    -file_name {SK1ABLEN_2024} \
    -export_dir ${PrjLocation}/designer/top/export \
    -format STP \
    -master_file 0 \
    -encrypted_uek1_file 0 \
    -encrypted_uek1_file_components {} \
    -encrypted_uek2_file 0 \
    -encrypted_uek2_file_components {} \
    -trusted_facility_file 1 \
    -trusted_facility_file_components "FABRIC ENVM"

puts "Exported bit stream successfully\n"

# Export Programming Job
# Programming job files can be imported in FlasPro Express standalone for programming the device
export_prog_job \
    -job_file_name {SK1ABLEN_2024} \
    -export_dir ${PrjLocation}/designer/top/export \
    -bitstream_file_type {TRUSTED_FACILITY} \
    -bitstream_file_components {FABRIC ENVM} \
    -include_plaintext_passkey 0 \
	-design_bitstream_format {STP}
	
puts "Exported job file successfully\n"
puts "Full design flow passed execution\n"	
