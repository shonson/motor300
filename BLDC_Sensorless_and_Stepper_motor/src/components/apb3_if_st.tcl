# Exporting core apb3_if_st to TCL
# Exporting Create HDL core command for module apb3_if_st
create_hdl_core -file {hdl/apb3_if_st.vhd} -module {apb3_if_st} -library {work} -package {}
# Exporting BIF information of  HDL core command for module apb3_if_st
hdl_core_add_bif -hdl_core_name {apb3_if_st} -bif_definition {APB:AMBA:AMBA2:slave} -bif_name {STEPPER_APB_IF} -signal_map {\
"PADDR:paddr_i" \
"PSELx:psel_i" \
"PENABLE:penable_i" \
"PWRITE:pwrite_i" \
"PRDATA:prdata_o" \
"PWDATA:pwdata_i" \
"PREADY:pready_o" \
"PSLVERR:pslverr_o" }
