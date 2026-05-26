--=============================================================================
-- File Name                           : apb3_en_if.vhd

-- Description                         : This module implements the APB3 slave
--                                       Interface to communicate with APB3
--                                       Master

-- Targeted device                     : Microsemi-SoC
-- Author                              : India Solutions Team
--
-- SVN Revision Information            :
-- SVN $Revision                       :
-- SVN $Date                           :
--
-- COPYRIGHT 2017 BY MICROSEMI
-- THE INFORMATION CONTAINED IN THIS DOCUMENT IS SUBJECT TO LICENSING
-- RESTRICTIONS FROM MICROSEMI CORP. IF YOU ARE NOT IN POSSESSION OF WRITTEN
-- AUTHORIZATION FROM MICROSEMI FOR USE OF THIS FILE, THEN THE FILE SHOULD BE
-- IMMEDIATELY DESTROYED AND NO BACK-UP OF THE FILE SHOULD BE MADE.
--
--=============================================================================

--=============================================================================
-- Libraries
--=============================================================================
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

--=============================================================================
-- apb3_en_if entity declaration
--=============================================================================
ENTITY apb3_en_if IS
GENERIC (
-- Generic list
    -- Specifies pwdata_i,prdata_o signal width
    g_APB3_IF_DATA_WIDTH    : INTEGER := 32;

    -- Specifies the width of constants
    g_CONST_WIDTH           : INTEGER := 12;

    -- Specifies the width of ADC module outputs
    g_ADC_OUT_WIDTH         : INTEGER := 12;

    -- Specifies the width of ADC configuration register
    g_ADC_CONFIG_REG_WIDTH  : INTEGER := 6;

    -- Specifies width of all input and output ports
    g_STD_IO_WIDTH          : INTEGER := 18
);
PORT (
-- Port list
    -- APB reset
    preset_i                : IN  STD_LOGIC;
    -- APB clock
    pclk_i                  : IN  STD_LOGIC;

    -- APB slave interface
    psel_i                  : IN  STD_LOGIC;
    pwrite_i                : IN  STD_LOGIC;
    penable_i               : IN  STD_LOGIC;
    paddr_i                 : IN  STD_LOGIC_VECTOR(g_APB3_IF_DATA_WIDTH-1
                                    DOWNTO 0);
    pwdata_i                : IN  STD_LOGIC_VECTOR(g_APB3_IF_DATA_WIDTH-1
                                    DOWNTO 0);
    pready_o                : OUT STD_LOGIC;
    pslverr_o               : OUT STD_LOGIC;
    prdata_o                : OUT STD_LOGIC_VECTOR(g_APB3_IF_DATA_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the ia input of clarke module
    ia_i                    : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the ib input of clarke module
    ib_i                    : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the ialpha output of clarke module
    ialpha_i                : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the ibeta output of clarke module
    ibeta_i                 : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the id output of park module
    id_i                    : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the iq output of park module
    iq_i                    : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the valpha output of ipark module
    valpha_i                : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the vbeta output of ipark module
    vbeta_i                 : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the output of rate limiter module
    rl_out_i                : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the speed pi output
    speed_pi_out_i          : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the id pi output
    id_pi_out_i             : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the iq pi output
    iq_pi_out_i             : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the closed loop theta information
    theta_i                 : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the adc output for channel 0
    adc_ch0_val_i           : IN  STD_LOGIC_VECTOR(g_ADC_OUT_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the adc output for channel 1
    adc_ch1_val_i           : IN  STD_LOGIC_VECTOR(g_ADC_OUT_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the adc output for channel 2
    adc_ch2_val_i           : IN  STD_LOGIC_VECTOR(g_ADC_OUT_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the adc output for channel 3
    adc_ch3_val_i           : IN  STD_LOGIC_VECTOR(g_ADC_OUT_WIDTH-1
                                    DOWNTO 0);

    -- Sequence controller FSM state
    fsm_state_val_i         : IN  STD_LOGIC_VECTOR(3 DOWNTO 0);

    -- Data Input specifies the encoder angle input value
    en_angle_i              : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Input specifies the speed filter output
    en_speed_filter_out_i   : IN  STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Control Output specifies the start signal for the sequence controller
    start_motor_o           : OUT STD_LOGIC;

    -- Control Output specifies the stop signal for the sequence controller
    stop_motor_o            : OUT STD_LOGIC;

    -- Control Output specifies fault clear signal
    fault_clr_o             : OUT STD_LOGIC;

    -- Control Output specifies the direction
    direction_config_o      : OUT STD_LOGIC;

    -- Data Output specifies the sequence controller configuration value
    seq_cntl_config_o       : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);

    -- Enable position PI
    position_pi_en_o        : OUT STD_LOGIC;

    -- Data Output specifies the kp value for position pi controller
    position_pi_kp_o        : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the ki value for position pi controller
    position_pi_ki_o        : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the reference value for position pi controller
    position_pi_ref_o       : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the kp value for speed pi controller
    speed_pi_kp_o           : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the ki value for speed pi controller
    speed_pi_ki_o           : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the kp value for current pi controller
    idq_pi_kp_o             : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the ki value for current pi controller
    idq_pi_ki_o             : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

   -- Data Output specifies the rate limiter reference input value
    rate_limit_ref_in_o     : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the rate limiter slew count value
    rate_limit_slew_cnt_o   : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the rate limiter rate count value
    rate_limit_rate_cnt_o   : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the theta factor value for olmng module
    olmng_theta_factor_o    : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the dv value for olmng module
    olmng_dv_o              : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the iq reference value for olmng module
    olmng_iq_ref_in_o       : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the threshold speed above which motor runs
    -- in closed loop
    seq_cntl_cl_omega_o     : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the control register information for
    -- the ADC module
    adc_control_reg_val_o   : OUT STD_LOGIC_VECTOR(g_ADC_CONFIG_REG_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the ADC channel 0 config value
    adc_channel_ch0_o       : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);

    -- Data Output specifies the ADC channel 1 config value
    adc_channel_ch1_o       : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);

    -- Data Output specifies the ADC channel 2 config value
    adc_channel_ch2_o       : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);

    -- Data Output specifies the ADC channel 3 config value
    adc_channel_ch3_o       : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);

    -- Data Output specifies the ADC scaling value
    adc_scale_val_o         : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the ADC over-current threshold value
    adc_oc_thresh_o         : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the PWM period value
    pwm_period_val_o        : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the dead time register information
    pwm_dead_time_val_o     : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the delay time register information
    pwm_delay_time_val_o    : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the PWM gain information
    pwm_gain_val_o          : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data output specifies the number of auto restarts allowed before fault
    auto_restart_no_o       : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);

    -- Data Output specifies the filter factor for omega computation
    en_filter_factor_omega_o : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);

    -- Data Output specifies the speed factor value for Encoder block
    speed_factor_o          : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the speed window information
    speed_window_o          : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the angle factor value for Encoder block
    angle_factor_o          : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0);

    -- Data Output specifies the angle count max value for Encoder block
    angle_count_max_o       : OUT STD_LOGIC_VECTOR(g_STD_IO_WIDTH-1
                                    DOWNTO 0)
);
END apb3_en_if;

--=============================================================================
-- apb3_if architecture body
--=============================================================================

ARCHITECTURE apb3_en_if OF apb3_en_if IS

--=============================================================================
-- Component declarations
--=============================================================================
--NA--

--=============================================================================
-- Synthesis Attributes
--=============================================================================
--NA--

--=============================================================================
-- Signal declarations
--=============================================================================
--ADC Register Addresses
CONSTANT C_ADC_CONFIG_REG_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"000";
CONSTANT C_CH0_CONFIG_VAL_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"004";
CONSTANT C_CH1_CONFIG_VAL_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"008";
CONSTANT C_CH2_CONFIG_VAL_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"00C";
CONSTANT C_CH3_CONFIG_VAL_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"010";
--ADC Scaling Register Addresses
CONSTANT C_ADC_SCALE_VAL_ADDR          : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"020";
CONSTANT C_OVER_CURRENT_THRESHOLD      : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"024";
--PWM Register Addresses
CONSTANT C_PWM_PERIOD_VAL_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"034";
CONSTANT C_PWM_DEAD_TIME_ADDR          : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"038";
CONSTANT C_PWM_DELAY_TIME_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"03C";
CONSTANT C_PWM_PRESCALE_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"040";
--Speed, ID, IQ PI Controller Register Addresses
CONSTANT C_SPEED_PI_KP_ADDR            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"050";
CONSTANT C_SPEED_PI_KI_ADDR            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"054";
CONSTANT C_IDQ_PI_KP_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"060";
CONSTANT C_IDQ_PI_KI_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"064";
--Rate Limiter Register Addresses
CONSTANT C_RATE_LIMIT_REF_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"070";
CONSTANT C_RATE_LIMIT_SLEW_CNT_ADDR    : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"074";
CONSTANT C_RATE_LIMIT_RATE_CNT_ADDR    : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"078";
--Sequence Manager Register Addresses
CONSTANT C_DIRECTION_CONFIG_ADDR       : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"090";
CONSTANT C_START_MOTOR_ADDR            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"094";
CONSTANT C_OLMNG_THETA_FACTOR_ADDR     : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"098";
CONSTANT C_SEQ_CNTL_CONFIG_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"09C";
CONSTANT C_OLMNG_DV_ADDR               : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0A0";
CONSTANT C_OLMNG_IQ_REF_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0A4";
CONSTANT C_SEQ_CNTL_CL_OMEGA_ADDR      : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0A8";
CONSTANT C_STOP_MOTOR_ADDR             : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0AC";
CONSTANT C_FSM_DEBUG_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0B0";
CONSTANT C_NO_AUTO_RESTARTS            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0B4";
CONSTANT C_CLR_FAULT_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"0B8";
--Encoder Register Addresses
CONSTANT C_EN_SPEED_FACTOR_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"300";
CONSTANT C_EN_ANGLE_FACTOR_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"304";
CONSTANT C_EN_ANGLE_CNT_MAX_ADDR       : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"308";
CONSTANT C_EN_SPD_FILTER_FACTOR_ADDR   : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"30C";
CONSTANT C_EN_SPEED_WINDOW_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"310";
CONSTANT C_POSITION_PI_KP_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"314";
CONSTANT C_POSITION_PI_KI_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"318";
CONSTANT C_POSITION_PI_EN_ADDR         : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"31C";
CONSTANT C_POSITION_PI_REF_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"320";

--Plotting
CONSTANT C_IA_REG_ADDR                 : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"800";
CONSTANT C_IB_REG_ADDR                 : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"804";
CONSTANT C_IALPHA_REG_ADDR             : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"808";
CONSTANT C_IBETA_REG_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"80C";
CONSTANT C_ID_PARK_REG_ADDR            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"810";
CONSTANT C_IQ_PARK_REG_ADDR            : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"814";
CONSTANT C_VALPHA_IPARK_REG_ADDR       : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"818";
CONSTANT C_VBETA_IPARK_REG_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"81C";
CONSTANT C_RATE_LIMITER_OUT_ADDR       : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"820";
CONSTANT C_SPEED_PI_OUT_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"824";
CONSTANT C_ID_PI_OUT_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"828";
CONSTANT C_IQ_PI_OUT_ADDR              : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"82C";
CONSTANT C_OLMNG_THETA_OUT_ADDR        : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"830";
CONSTANT C_CH0_DATA_VAL_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"838";
CONSTANT C_CH1_DATA_VAL_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"83C";
CONSTANT C_CH2_DATA_VAL_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"840";
CONSTANT C_CH3_DATA_VAL_ADDR           : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"844";
--Encoder specific plotting
CONSTANT C_EN_SPEED_ADDR               : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"A00";
CONSTANT C_EN_ANGLE_ADDR               : STD_LOGIC_VECTOR(g_CONST_WIDTH-1
                                           DOWNTO 0) := x"A04";


BEGIN


--=============================================================================
-- Top level output port assignments
--=============================================================================
pready_o     <= '1';  -- pready_o Is always ready,there will not be any
                      -- latency from the Fabric modules
pslverr_o    <= '0';  -- Slave error is always '0' as there will not be
                      --any slave error.


--=============================================================================
-- Generate blocks
--=============================================================================
--NA--

--=============================================================================
-- Asynchronous blocks
--=============================================================================
--------------------------------------------------------------------------
-- Name       : READ_DECODE_PROC
-- Description: Process implements the APB read operation
--------------------------------------------------------------------------

READ_DECODE_PROC:
    PROCESS (paddr_i,ialpha_i,ibeta_i,id_i,ia_i,ib_i,adc_ch0_val_i,
             adc_ch1_val_i,iq_i,valpha_i,vbeta_i,rl_out_i,adc_ch2_val_i,
             adc_ch3_val_i,speed_pi_out_i,id_pi_out_i,iq_pi_out_i,
             fsm_state_val_i,theta_i,en_angle_i,en_speed_filter_out_i)
    BEGIN
        CASE  paddr_i(11 DOWNTO 0)  IS
--------------------
-- C_IA_REG_ADDR
--------------------
            WHEN C_IA_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)        <= ia_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>ia_i(g_STD_IO_WIDTH -1));
--------------------
-- C_IB_REG_ADDR
--------------------
            WHEN C_IB_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)        <= ib_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>ib_i(g_STD_IO_WIDTH -1));
--------------------
-- C_IALPHA_REG_ADDR
--------------------
            WHEN C_IALPHA_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)        <= ialpha_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>ialpha_i(g_STD_IO_WIDTH -1));
--------------------
-- C_IBETA_REG_ADDR
--------------------
            WHEN C_IBETA_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)        <= ibeta_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>ibeta_i(g_STD_IO_WIDTH -1));
--------------------
-- C_ID_PARK_REG_ADDR
--------------------
            WHEN C_ID_PARK_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= id_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>id_i(g_STD_IO_WIDTH -1));
--------------------
-- C_IQ_PARK_REG_ADDR
--------------------

            WHEN C_IQ_PARK_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= iq_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>iq_i(g_STD_IO_WIDTH -1));
--------------------
-- C_VALPHA_IPARK_REG_ADDR
--------------------
            WHEN C_VALPHA_IPARK_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= valpha_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>valpha_i(g_STD_IO_WIDTH -1));
--------------------
-- C_VBETA_IPARK_REG_ADDR
--------------------
            WHEN C_VBETA_IPARK_REG_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= vbeta_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>vbeta_i(g_STD_IO_WIDTH -1));
--------------------
-- C_RATE_LIMITER_OUT_ADDR
--------------------
            WHEN C_RATE_LIMITER_OUT_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= rl_out_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>rl_out_i(g_STD_IO_WIDTH -1));
--------------------
-- C_SPEED_PI_OUT_ADDR
--------------------
            WHEN C_SPEED_PI_OUT_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= speed_pi_out_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>speed_pi_out_i(g_STD_IO_WIDTH -1));
--------------------
-- C_ID_PI_OUT_ADDR
--------------------
            WHEN C_ID_PI_OUT_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= id_pi_out_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>id_pi_out_i(g_STD_IO_WIDTH -1));
--------------------
-- C_IQ_PI_OUT_ADDR
--------------------
            WHEN C_IQ_PI_OUT_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= iq_pi_out_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>iq_pi_out_i(g_STD_IO_WIDTH -1));

--------------------
-- C_OLMNG_THETA_OUT_ADDR
--------------------
            WHEN C_OLMNG_THETA_OUT_ADDR =>
                prdata_o(g_STD_IO_WIDTH -1 DOWNTO 0)       <= theta_i;
                prdata_o(g_APB3_IF_DATA_WIDTH-1 DOWNTO g_STD_IO_WIDTH)
                                <= (OTHERS=>theta_i(g_STD_IO_WIDTH -1));
--------------------
-- C_CH0_DATA_VAL_ADDR
--------------------
            WHEN C_CH0_DATA_VAL_ADDR =>
                prdata_o(11                DOWNTO 0)       <= adc_ch0_val_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO 12)
                                <= (OTHERS=>'0');
--------------------
-- C_CH1_DATA_VAL_ADDR
--------------------
            WHEN C_CH1_DATA_VAL_ADDR =>
                prdata_o(11                DOWNTO 0)       <= adc_ch1_val_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO 12)
                                <= (OTHERS=>'0');
--------------------
-- C_CH2_DATA_VAL_ADDR
--------------------
            WHEN C_CH2_DATA_VAL_ADDR =>
                prdata_o(11                DOWNTO 0)       <= adc_ch2_val_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO 12)
                                <= (OTHERS=>'0');
--------------------
-- C_CH3_DATA_VAL_ADDR
--------------------
            WHEN C_CH3_DATA_VAL_ADDR =>
                prdata_o(11                DOWNTO 0)       <= adc_ch3_val_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO 12)
                                <= (OTHERS=>'0');
----------------------
---- C_FSM_DEBUG_ADDR
----------------------
            WHEN C_FSM_DEBUG_ADDR =>
                prdata_o(3 DOWNTO 0)                       <= fsm_state_val_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO 4)
                                <= (OTHERS=>'0');
----------------------
---- C_EN_SPEED_ADDR
----------------------
            WHEN C_EN_SPEED_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)  <= en_speed_filter_out_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO g_STD_IO_WIDTH)
                    <= (OTHERS=>en_speed_filter_out_i(g_STD_IO_WIDTH-1));

----------------------
---- C_EN_ANGLE_ADDR
----------------------
            WHEN C_EN_ANGLE_ADDR =>
                prdata_o(g_STD_IO_WIDTH-1 DOWNTO 0)  <= en_angle_i;
                prdata_o(g_APB3_IF_DATA_WIDTH -1 DOWNTO g_STD_IO_WIDTH)
                     <= (OTHERS=>'0');
--------------------
-- OTHERS
--------------------
            WHEN OTHERS =>
            prdata_o <= (OTHERS=>'0');

        END CASE;

    END PROCESS;

--=============================================================================
-- Synchronous blocks
--=============================================================================
--------------------------------------------------------------------------
-- Name       : WRITE_DECODE_PROC
-- Description: Process implements the APB write operation
--------------------------------------------------------------------------
WRITE_DECODE_PROC:
    PROCESS (preset_i, pclk_i)
    BEGIN
        IF(preset_i = '0')THEN
            start_motor_o                        <= '0';
            stop_motor_o                         <= '0';
            direction_config_o                   <= '0';
            fault_clr_o                          <= '0';
            position_pi_en_o                     <= '0';
            seq_cntl_config_o                    <= (OTHERS => '0');
            position_pi_kp_o                     <= (OTHERS => '0');
            position_pi_ki_o                     <= (OTHERS => '0');
            position_pi_ref_o                    <= (OTHERS => '0');
            speed_pi_kp_o                        <= (OTHERS => '0');
            speed_pi_ki_o                        <= (OTHERS => '0');
            idq_pi_kp_o                          <= (OTHERS => '0');
            idq_pi_ki_o                          <= (OTHERS => '0');
            rate_limit_ref_in_o                  <= (OTHERS => '0');
            rate_limit_slew_cnt_o                <= (OTHERS => '0');
            rate_limit_rate_cnt_o                <= (OTHERS => '0');
            olmng_theta_factor_o                 <= (OTHERS => '0');
            olmng_dv_o                           <= (OTHERS => '0');
            olmng_iq_ref_in_o                    <= (OTHERS => '0');
            seq_cntl_cl_omega_o                  <= (OTHERS => '0');
            adc_control_reg_val_o                <= (OTHERS => '0');
            pwm_dead_time_val_o                  <= (OTHERS => '0');
            pwm_delay_time_val_o                 <= (OTHERS => '0');
            pwm_gain_val_o                       <= (OTHERS => '0');
            pwm_period_val_o                     <= (OTHERS => '0');
            adc_scale_val_o                      <= (OTHERS => '0');
            adc_oc_thresh_o                      <= (OTHERS => '0');
            adc_channel_ch0_o                    <= (OTHERS => '0');
            adc_channel_ch1_o                    <= (OTHERS => '0');
            adc_channel_ch2_o                    <= (OTHERS => '0');
            adc_channel_ch3_o                    <= (OTHERS => '0');
            auto_restart_no_o                    <= (OTHERS => '0');
            speed_factor_o                       <= (OTHERS => '0');
            speed_window_o                       <= (OTHERS => '0');
            angle_factor_o                       <= (OTHERS => '0');
            angle_count_max_o                    <= (OTHERS => '0');
            en_filter_factor_omega_o             <= (OTHERS => '0');
        ELSIF (pclk_i'EVENT AND pclk_i = '1') THEN
            IF ((psel_i = '1') AND (pwrite_i = '1') AND (penable_i = '1')) THEN
                CASE paddr_i(11 DOWNTO 0)  IS
--------------------
-- C_POSITION_PI_EN_ADDR
--------------------
                    WHEN C_POSITION_PI_EN_ADDR =>
                        position_pi_en_o <= pwdata_i(0);
--------------------
-- C_POSITION_PI_KP_ADDR
--------------------
                    WHEN C_POSITION_PI_KP_ADDR =>
                        position_pi_kp_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_POSITION_PI_KI_ADDR
--------------------
                    WHEN C_POSITION_PI_KI_ADDR =>
                        position_pi_ki_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);
--------------------
-- C_POSITION_PI_REF_ADDR
--------------------
                    WHEN C_POSITION_PI_REF_ADDR =>
                        position_pi_ref_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_SPEED_PI_KP_ADDR
--------------------
                    WHEN C_SPEED_PI_KP_ADDR =>
                        speed_pi_kp_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_SPEED_PI_KI_ADDR
--------------------
                    WHEN C_SPEED_PI_KI_ADDR =>
                        speed_pi_ki_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_IDQ_PI_KP_ADDR
--------------------
                    WHEN C_IDQ_PI_KP_ADDR =>
                        idq_pi_kp_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_IDQ_PI_KI_ADDR
--------------------
                    WHEN C_IDQ_PI_KI_ADDR =>
                        idq_pi_ki_o <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_DIRECTION_CONFIG_ADDR
--------------------
                    WHEN C_DIRECTION_CONFIG_ADDR =>
                        direction_config_o <= pwdata_i(0);

--------------------
-- C_CLR_FAULT_ADDR
--------------------
                    WHEN C_CLR_FAULT_ADDR =>
                        fault_clr_o <= pwdata_i(0);

--------------------
-- C_RATE_LIMIT_REF_ADDR
--------------------
                    WHEN C_RATE_LIMIT_REF_ADDR  =>
                        rate_limit_ref_in_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_RATE_LIMIT_SLEW_CNT_ADDR
--------------------
                    WHEN C_RATE_LIMIT_SLEW_CNT_ADDR =>
                        rate_limit_slew_cnt_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_RATE_LIMIT_RATE_CNT_ADDR
--------------------
                    WHEN C_RATE_LIMIT_RATE_CNT_ADDR =>
                        rate_limit_rate_cnt_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_OLMNG_THETA_FACTOR_ADDR
--------------------

                    WHEN C_OLMNG_THETA_FACTOR_ADDR =>
                        olmng_theta_factor_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);
--------------------
-- C_SEQ_CNTL_CONFIG_ADDR
--------------------
                    WHEN C_SEQ_CNTL_CONFIG_ADDR =>
                        seq_cntl_config_o <= pwdata_i(3 DOWNTO 0);

--------------------
-- C_OLMNG_DV_ADDR
--------------------
                    WHEN C_OLMNG_DV_ADDR =>
                        olmng_dv_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_OLMNG_IQ_REF_ADDR
--------------------
                    WHEN C_OLMNG_IQ_REF_ADDR  =>
                        olmng_iq_ref_in_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_SEQ_CNTL_CL_OMEGA_ADDR
--------------------
                    WHEN C_SEQ_CNTL_CL_OMEGA_ADDR  =>
                        seq_cntl_cl_omega_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_ADC_SCALE_VAL_ADDR
--------------------
                    WHEN C_ADC_SCALE_VAL_ADDR  =>
                        adc_scale_val_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_OVER_CURRENT_THRESHOLD
--------------------
                    WHEN C_OVER_CURRENT_THRESHOLD  =>
                        adc_oc_thresh_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_PWM_PERIOD_VAL_ADDR
--------------------
                    WHEN C_PWM_PERIOD_VAL_ADDR =>
                        pwm_period_val_o <= pwdata_i (g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_ADC_CONFIG_REG_ADDR
--------------------
                    WHEN C_ADC_CONFIG_REG_ADDR  =>
                        adc_control_reg_val_o
                        <= pwdata_i(g_ADC_CONFIG_REG_WIDTH-1 DOWNTO 0);

--------------------
-- C_PWM_DEAD_TIME_ADDR
--------------------
                    WHEN C_PWM_DEAD_TIME_ADDR   =>
                        pwm_dead_time_val_o    <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_PWM_DELAY_TIME_ADDR
--------------------
                    WHEN C_PWM_DELAY_TIME_ADDR  =>
                        pwm_delay_time_val_o    <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_PWM_PRESCALE_ADDR
--------------------
                    WHEN C_PWM_PRESCALE_ADDR    =>
                        pwm_gain_val_o    <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_NO_AUTO_RESTARTS
--------------------
                    WHEN C_NO_AUTO_RESTARTS =>
                        auto_restart_no_o  <= pwdata_i(3 DOWNTO 0);

--------------------
-- C_CH0_CONFIG_VAL_ADDR
--------------------
                    WHEN C_CH0_CONFIG_VAL_ADDR =>
                        adc_channel_ch0_o        <= pwdata_i(15 DOWNTO 0);

--------------------
-- C_CH1_CONFIG_VAL_ADDR
--------------------
                    WHEN C_CH1_CONFIG_VAL_ADDR =>
                        adc_channel_ch1_o        <= pwdata_i(15 DOWNTO 0);

--------------------
-- C_CH2_CONFIG_VAL_ADDR
--------------------
                    WHEN C_CH2_CONFIG_VAL_ADDR =>
                        adc_channel_ch2_o        <= pwdata_i(15 DOWNTO 0);

--------------------
-- C_CH3_CONFIG_VAL_ADDR
--------------------
                    WHEN C_CH3_CONFIG_VAL_ADDR =>
                        adc_channel_ch3_o        <= pwdata_i(15 DOWNTO 0);
--------------------
-- C_EN_SPEED_FACTOR_ADDR
--------------------
                    WHEN C_EN_SPEED_FACTOR_ADDR =>
                        speed_factor_o          <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);
--------------------
-- C_EN_SPEED_WINDOW_ADDR
--------------------
                    WHEN C_EN_SPEED_WINDOW_ADDR    =>
                        speed_window_o    <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_EN_ANGLE_FACTOR_ADDR
--------------------
                    WHEN C_EN_ANGLE_FACTOR_ADDR =>
                        angle_factor_o          <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);
--------------------
-- C_EN_ANGLE_CNT_MAX_ADDR
--------------------
                    WHEN C_EN_ANGLE_CNT_MAX_ADDR =>
                        angle_count_max_o       <= pwdata_i(g_STD_IO_WIDTH-1
                                                         DOWNTO 0);

--------------------
-- C_EN_SPD_FILTER_FACTOR_ADDR
--------------------
                    WHEN C_EN_SPD_FILTER_FACTOR_ADDR =>
                        en_filter_factor_omega_o     <= pwdata_i(3 DOWNTO 0);
--------------------
-- C_START_MOTOR_ADDR
--------------------
                    WHEN C_START_MOTOR_ADDR =>
                        start_motor_o <= pwdata_i(0);
--------------------
-- C_STOP_MOTOR_ADDR
--------------------
                    WHEN C_STOP_MOTOR_ADDR =>
                        stop_motor_o <= pwdata_i(0);

--------------------
-- OTHERS
--------------------
                    WHEN OTHERS =>
                        NULL;
                END CASE;
            END IF;
        END IF;
    END PROCESS;

--=============================================================================
-- Component Instantiations
--=============================================================================
--NA--

END ARCHITECTURE apb3_en_if;