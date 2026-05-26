--=============================================================================
-- File Name                           : adc_interface_795x.vhd

-- Description                         : This module implements the top level
--                                       FSM for the ADS795X (ADC) interface.

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
USE IEEE.std_logic_1164.ALL;
USE IEEE.std_logic_unsigned.ALL;

--=============================================================================
-- adc_interface_795x entity declaration
--=============================================================================
ENTITY adc_interface_795x IS
GENERIC(
-- Generic list
    --Specifies ADC Resolution
    g_ADC_RES               : INTEGER := 12;

    --Specifies the SCK divider - (fsck = fsys_clk_i/(2^g_SCK_DIVIDER))
    g_SCK_DIVIDER           : INTEGER RANGE 1 TO 8:= 3
);
PORT (
-- Port list
    -- System reset
    reset_i                 : IN  STD_LOGIC;

    -- System clock
    sys_clk_i               : IN  STD_LOGIC;

    -- Start of ADC interface operation
    start_i                 : IN  STD_LOGIC;

    -- Single bit data input from serial line
    sdi_i                   : IN  STD_LOGIC;

    -- Specifies channels to sample (bit 0 - Channel 0, bit 1 - Channel 1..)
    channels_to_sample_i    : IN STD_LOGIC_VECTOR(15 DOWNTO 0);

    -- Single bit data output to the serial line
    sdo_o                   : OUT STD_LOGIC;

    -- Chip/slave select signal for the serial interface
    cs_o                    : OUT STD_LOGIC;

    -- SCK or the clock signal for the serial interface
    sck_o                   : OUT STD_LOGIC;

    -- ADC results for channels 0-15
    result_ch0_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch1_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch2_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch3_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch4_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch5_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch6_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch7_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch8_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch9_o            : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch10_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch11_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch12_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch13_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch14_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);
    result_ch15_o           : OUT STD_LOGIC_VECTOR(g_ADC_RES - 1 DOWNTO 0);

    -- Results for all the channels are ready
    done_o                  : OUT STD_LOGIC
);
END adc_interface_795x;


--=============================================================================
-- adc_interface_795x architecture body
--=============================================================================
ARCHITECTURE adc_interface OF adc_interface_795x IS

--=============================================================================
-- Component declarations
--=============================================================================
COMPONENT spi_engine IS
GENERIC(
    g_SPI_FRAME_NO_OF_BITS  : INTEGER := 16;
    g_SCK_DIVIDER           : INTEGER RANGE 1 TO 8:= 2
);
PORT (
    reset_i                 : IN  STD_LOGIC;
    sys_clk_i               : IN  STD_LOGIC;
    start_i                 : IN  STD_LOGIC;
    sdi_i                   : IN  STD_LOGIC;
    data_mosi_i             : IN  STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                    DOWNTO 0);
    sdo_o                   : OUT STD_LOGIC;
    cs_o                    : OUT STD_LOGIC;
    sck_o                   : OUT STD_LOGIC;
    data_miso_o             : OUT STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                    DOWNTO 0);
    done_o                  : OUT STD_LOGIC
);
END COMPONENT;

--=============================================================================
-- Synthesis Attributes
--=============================================================================
--NA--

--=============================================================================
-- Signal declarations
--=============================================================================
TYPE adc_op_state is                (IDLE,
                                    CHANNEL_CHECK,
                                    START_SPI,
                                    WAIT_FOR_SPI,
                                    LAST_ITER_1,
                                    LAST_ITER_2
                                    );
CONSTANT C_SPI_FRAME_NO_OF_BITS     : INTEGER RANGE 8 TO 16:= 16;
SIGNAL state                        : adc_op_state;
SIGNAL s_spi_mosi                   : STD_LOGIC_VECTOR(C_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);
SIGNAL s_data_miso                  : STD_LOGIC_VECTOR(C_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);
SIGNAL s_spi_miso                   : STD_LOGIC_VECTOR(C_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);
SIGNAL s_channel_sel                : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL s_index                      : INTEGER RANGE 0 TO 15;
SIGNAL s_all_ch_done                : STD_LOGIC;
SIGNAL s_result_done                : STD_LOGIC;
SIGNAL s_spi_start                  : STD_LOGIC;
SIGNAL s_spi_done                   : STD_LOGIC;

TYPE ADC_CH_INPUTS IS ARRAY (0 to 15) OF
    STD_LOGIC_VECTOR(C_SPI_FRAME_NO_OF_BITS-1 DOWNTO 0);
 CONSTANT C_ADC_CH_INPUTS_16        : ADC_CH_INPUTS:=  (x"1840",
                                                        x"18C0",
                                                        x"1940",
                                                        x"19C0",
                                                        x"1A40",
                                                        x"1AC0",
                                                        x"1B40",
                                                        x"1BC0",
                                                        x"1C40",
                                                        x"1CC0",
                                                        x"1D40",
                                                        x"1DC0",
                                                        x"1E40",
                                                        x"1EC0",
                                                        x"1F40",
                                                        x"1FC0"
                                                        );
BEGIN

--=============================================================================
-- Top level output port assignments
--=============================================================================
done_o      <= s_all_ch_done;

--=============================================================================
-- Generate blocks
--=============================================================================
--NA--

--=============================================================================
-- Asynchronous blocks
--=============================================================================
--NA--

--=============================================================================
-- Synchronous blocks
--=============================================================================
--------------------------------------------------------------------------
-- Name       : ADC_FSM_NS_PROC
-- Description: Process implements FSM to interface external ports and
--              spi_engine.
--------------------------------------------------------------------------
ADC_FSM_NS_PROC:
PROCESS(reset_i,sys_clk_i)
BEGIN
    IF(reset_i = '0')THEN
        s_all_ch_done   <= '0';
        s_result_done   <= '0';
        s_spi_start     <= '0';
        state           <= IDLE;
        s_spi_mosi      <= (OTHERS => '0');
        s_data_miso     <= (OTHERS => '0');
        s_index         <= 0;
    ELSIF(RISING_EDGE(sys_clk_i))THEN
        CASE state IS
--------------------
-- IDLE state
--------------------
            WHEN IDLE =>
                s_all_ch_done           <= '0';
                s_result_done           <= '0';
                s_index                 <= 0;
                IF(start_i = '1')  THEN
                    state               <= CHANNEL_CHECK;
                ELSE
                    state               <= IDLE;
                END IF;

--------------------
-- CHANNEL_CHECK state
--------------------
            WHEN CHANNEL_CHECK =>
                s_result_done           <= '0';
                IF(s_channel_sel(s_index) = '0')THEN
                    IF(s_index = 15)THEN
                        state <= LAST_ITER_1;
                    ELSE
                        state           <= CHANNEL_CHECK;
                        s_index         <= s_index + 1;
                    END IF;
                ELSE
                    state               <= START_SPI;
                END IF;

--------------------
-- START_SPI state
--------------------
            WHEN START_SPI =>
                s_all_ch_done           <= '0';
                state                   <= WAIT_FOR_SPI;
                s_spi_mosi              <= C_ADC_CH_INPUTS_16(s_index);
                s_spi_start             <= '1';

--------------------
-- WAIT_FOR_SPI state
--------------------
            WHEN WAIT_FOR_SPI =>
                s_spi_start             <= '0';
                IF(s_spi_done = '1')THEN
                    s_data_miso         <= s_spi_miso;
                    state               <= CHANNEL_CHECK;
                    s_index             <= s_index + 1;
                    s_result_done       <= '1';
                ELSE
                    state               <= WAIT_FOR_SPI;
                    s_result_done       <= '0';
                END IF;

--------------------
-- LAST_ITER_1 state
--------------------
            WHEN LAST_ITER_1 =>
                s_all_ch_done           <= '0';
                state                   <= LAST_ITER_2;
                s_spi_mosi              <= (OTHERS => '0');
                s_spi_start             <= '1';

--------------------
-- LAST_ITER_2 state
--------------------
            WHEN LAST_ITER_2 =>
                s_spi_start <= '0';
                IF(s_spi_done = '1')THEN
                    s_data_miso         <= s_spi_miso;
                    state               <= IDLE;
                    s_index             <= 0;
                    s_result_done       <= '1';
                    s_all_ch_done       <= '1';
                ELSE
                    state               <= LAST_ITER_2;
                    s_result_done       <= '0';
                    s_all_ch_done       <= '0';
                END IF;

--------------------
-- OTHERS state
--------------------
            WHEN OTHERS =>
               state                    <= IDLE;
       END CASE;
    END IF;
 END PROCESS;

--------------------------------------------------------------------------
-- Name       : ADC_RESULTS_PROC
-- Description: This process assigns the collected ADC data to appropriate
--              channels.
--------------------------------------------------------------------------
ADC_RESULTS_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            result_ch0_o                <= (OTHERS => '0');
            result_ch1_o                <= (OTHERS => '0');
            result_ch2_o                <= (OTHERS => '0');
            result_ch3_o                <= (OTHERS => '0');
            result_ch4_o                <= (OTHERS => '0');
            result_ch5_o                <= (OTHERS => '0');
            result_ch6_o                <= (OTHERS => '0');
            result_ch7_o                <= (OTHERS => '0');
            result_ch8_o                <= (OTHERS => '0');
            result_ch9_o                <= (OTHERS => '0');
            result_ch10_o               <= (OTHERS => '0');
            result_ch11_o               <= (OTHERS => '0');
            result_ch12_o               <= (OTHERS => '0');
            result_ch13_o               <= (OTHERS => '0');
            result_ch14_o               <= (OTHERS => '0');
            result_ch15_o               <= (OTHERS => '0');
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            IF(s_result_done = '1') THEN
                CASE s_data_miso(15 DOWNTO 12) IS
                    WHEN x"0" =>
                        result_ch0_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"1" =>
                        result_ch1_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"2" =>
                        result_ch2_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"3" =>
                        result_ch3_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"4" =>
                        result_ch4_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"5" =>
                        result_ch5_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"6" =>
                        result_ch6_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"7" =>
                        result_ch7_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"8" =>
                        result_ch8_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"9" =>
                        result_ch9_o    <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"A" =>
                        result_ch10_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"B" =>
                        result_ch11_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"C" =>
                        result_ch12_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"D" =>
                        result_ch13_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"E" =>
                        result_ch14_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN x"F" =>
                        result_ch15_o   <= s_data_miso(g_ADC_RES-1 DOWNTO 0);
                    WHEN OTHERS => NULL;
                END CASE;
            END IF;
        END IF;
    END PROCESS;

--------------------------------------------------------------------------
-- Name       : CHANNEL_SEL_LATCH
-- Description: This process latches channel select register at start_i
--------------------------------------------------------------------------
CHANNEL_SEL_LATCH:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_channel_sel       <= (OTHERS => '0');
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            IF(start_i = '1') THEN
                s_channel_sel   <=   channels_to_sample_i;
            END IF;
        END IF;
    END PROCESS;


--=============================================================================
-- Component Instantiations
--=============================================================================
spi_engine_inst: spi_engine
GENERIC MAP(
    g_SPI_FRAME_NO_OF_BITS      => C_SPI_FRAME_NO_OF_BITS,
    g_SCK_DIVIDER               => g_SCK_DIVIDER
)
PORT MAP(
    reset_i                     => reset_i,
    sys_clk_i                   => sys_clk_i,
    start_i                     => s_spi_start,
    sdi_i                       => sdi_i,
    data_mosi_i                 => s_spi_mosi,
    sdo_o                       => sdo_o,
    cs_o                        => cs_o,
    sck_o                       => sck_o,
    data_miso_o                 => s_spi_miso,
    done_o                      => s_spi_done
);

END adc_interface;


--=============================================================================
-- File Name                           : spi_engine.vhd

-- Description                         : This module implements a low level
--                                       SPI engine.

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
USE IEEE.std_logic_1164.ALL;
USE ieee.numeric_std.all;
USE IEEE.std_logic_unsigned.ALL;

--=============================================================================
-- spi_engine entity declaration
--=============================================================================
ENTITY spi_engine IS
GENERIC(
-- Generic list
    --Specifies number of bits in SPI frame
    g_SPI_FRAME_NO_OF_BITS      : INTEGER := 16;
    --Specifies the SCK divider - (fsck = fsys_clk_i/(2^g_SCK_DIVIDER))
    g_SCK_DIVIDER               : INTEGER RANGE 1 TO 8:= 2
);
PORT (
-- Port list
    -- System reset
    reset_i                     : IN  STD_LOGIC;

    -- System clock
    sys_clk_i                   : IN  STD_LOGIC;

    -- Start of SPI operation
    start_i                     : IN  STD_LOGIC;

    -- Single bit data input from serial line
    sdi_i                       : IN  STD_LOGIC;

    -- Configuration value to be sent on SPI MOSI line
    data_mosi_i                 : IN  STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                    DOWNTO 0);

    -- Single bit data output to the serial line
    sdo_o                       : OUT STD_LOGIC;

    -- Chip/slave select signal for the serial interface
    cs_o                        : OUT STD_LOGIC;

    -- SCK or the clock signal for the serial interface
    sck_o                       : OUT STD_LOGIC;

    -- ADC results from SPI MISO
    data_miso_o                 : OUT STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                    DOWNTO 0);

    -- Operation complete
    done_o                      : OUT STD_LOGIC
);
END spi_engine;

--=============================================================================
-- spi_engine architecture body
--=============================================================================
ARCHITECTURE spi_engine OF spi_engine IS

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
SIGNAL   s_cs_n                     : STD_LOGIC;
SIGNAL   s_sck                      : STD_LOGIC;
SIGNAL   s_sdo                      : STD_LOGIC;
SIGNAL   s_sdi                      : STD_LOGIC;
SIGNAL   s_sckctr_done              : STD_LOGIC;
SIGNAL   s_done_dly                 : STD_LOGIC;
SIGNAL   s_done_dly1                : STD_LOGIC;
SIGNAL   s_sck_sync                 : STD_LOGIC;
SIGNAL   s_sck_falling              : STD_LOGIC;
SIGNAL   s_sck_rising               : STD_LOGIC;
SIGNAL   s_sck_enable               : STD_LOGIC;
SIGNAL   s_sys_clk_count            : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL   s_sck_count                : STD_LOGIC_VECTOR(5 DOWNTO 0);
SIGNAL   s_spifr_nbits              : STD_LOGIC_VECTOR(5 DOWNTO 0);
SIGNAL   s_sdo_reg                  : STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);
SIGNAL   s_miso_data                : STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);
SIGNAL   s_miso_data_reg            : STD_LOGIC_VECTOR(g_SPI_FRAME_NO_OF_BITS-1
                                        DOWNTO 0);


BEGIN

--=============================================================================
-- Top level output port assignments
--=============================================================================
sck_o           <= s_sck;
cs_o            <= s_cs_n;
sdo_o           <= s_sdo;
data_miso_o     <= s_miso_data_reg;
done_o          <= NOT(s_done_dly1) AND s_done_dly;

s_sck_falling   <= NOT(s_sck) AND s_sck_sync;
s_sck_rising    <= NOT(s_sck_sync) AND s_sck;
s_spifr_nbits   <= std_logic_vector(to_unsigned(g_SPI_FRAME_NO_OF_BITS, 6));
--------------------------------------------------------------------------
-- Name       : SCK_GEN_PROC
-- Description: This process generates the Serial clock to ADC.
--------------------------------------------------------------------------
SCK_GEN_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF reset_i = '0' THEN
            s_sys_clk_count         <= (OTHERS=>'0');
            s_sck                   <= '0';
            s_sck_sync              <= '0';
        ELSIF RISING_EDGE(sys_clk_i) THEN
            IF s_sck_enable = '1' THEN
                s_sys_clk_count     <= s_sys_clk_count + "00000001";
            ELSE
                s_sys_clk_count     <= (OTHERS=>'0');
            END IF;
            s_sck                   <= s_sys_clk_count(g_SCK_DIVIDER-1);
            s_sck_sync              <= s_sck;
        END IF;
   END PROCESS;

--------------------------------------------------------------------------
-- Name       : START_SIG_PROC
-- Description: This process samples the start_i and generates an
--              intermediate signal for ADC operation.
--------------------------------------------------------------------------
START_SIG_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_cs_n          <= '1';
            s_sck_enable    <= '0';
        ELSIF rising_edge(sys_clk_i) THEN
            IF(start_i = '1') THEN
                s_sck_enable <= '1';
                s_cs_n <= '0';
            ELSIF(s_sckctr_done = '1') THEN
                s_cs_n <= '1';
                s_sck_enable <= '0';
            END IF;
        END IF;
    END PROCESS;


--------------------------------------------------------------------------
-- Name       : DATA_TRANSMIT_PROC
-- Description: This process transmits single bit data on the adc_sdo line
--------------------------------------------------------------------------
DATA_TRANSMIT_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_sdo_reg                           <= (OTHERS => '0');
            s_sdo                               <= '0';
        ELSIF rising_edge(sys_clk_i) THEN
            IF(s_cs_n = '0' ) THEN
                IF (s_sck_rising = '1') THEN
                    s_sdo_reg(g_SPI_FRAME_NO_OF_BITS-1 DOWNTO 1)
                        <= s_sdo_reg(g_SPI_FRAME_NO_OF_BITS-2 DOWNTO 0);
                    s_sdo_reg(0)    <= '0';
                END IF;
                s_sdo    <= s_sdo_reg(g_SPI_FRAME_NO_OF_BITS-1) ;
            ELSE
                s_sdo_reg    <= data_mosi_i;
            END IF;
        END IF;
    END PROCESS;

--------------------------------------------------------------------------
-- Name       : DATA_COLLECTION_PROC
-- Description: This process collects the serial data from the ADC
--------------------------------------------------------------------------
DATA_COLLECTION_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_miso_data     <= (OTHERS=>'0')   ;
            s_sdi           <= '0';
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            IF(s_sck_falling = '1') THEN
                s_sdi <= sdi_i;
                IF(s_cs_n = '0') THEN
                    s_miso_data(g_SPI_FRAME_NO_OF_BITS-1 DOWNTO 0)
                        <= s_miso_data(g_SPI_FRAME_NO_OF_BITS-2 DOWNTO 0)
                            & s_sdi;
                ELSE
                    s_miso_data <= (OTHERS => '0');
                END IF;
            END IF;
        END IF;
    END PROCESS;

--------------------------------------------------------------------------
-- Name       : SCK_CTR_PROC
-- Description: This process counts number of SCK cycles generated
--------------------------------------------------------------------------
SCK_CTR_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_sck_count     <= (OTHERS=>'0')   ;
            s_sckctr_done   <= '0'   ;
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            IF(s_cs_n = '0' AND s_sck_falling = '1')THEN
                IF(s_sck_count = s_spifr_nbits)THEN
                    s_sck_count     <= (OTHERS=>'0');
                    s_sckctr_done   <= '1';
                ELSE
                    s_sck_count <= s_sck_count + 1;
                    s_sckctr_done   <= '0';
                END IF;
            ELSE
                s_sckctr_done   <= '0';
            END IF;
        END IF;
    END PROCESS;

--------------------------------------------------------------------------
-- Name       : DONE_DLY_PROC
-- Description: This process generates delayed done signal
--------------------------------------------------------------------------
DONE_DLY_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_done_dly  <= '0';
            s_done_dly1 <= '0';
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            s_done_dly  <= s_sckctr_done;
            s_done_dly1 <= s_done_dly;
        END IF;
    END PROCESS;

--------------------------------------------------------------------------
-- Name       : MISO_DATA_REG_PROC
-- Description: This process latches MISO data after one transaction
--------------------------------------------------------------------------
MISO_DATA_REG_PROC:
    PROCESS(reset_i,sys_clk_i)
    BEGIN
        IF (reset_i = '0') THEN
            s_miso_data_reg     <= (OTHERS => '0');
        ELSIF (RISING_EDGE(sys_clk_i)) THEN
            IF(s_sckctr_done = '1' AND s_done_dly = '0')THEN
                s_miso_data_reg         <= s_miso_data;
            END IF;
        END IF;
    END PROCESS;


--=============================================================================
-- Component Instantiations
--=============================================================================
--NA--

END spi_engine;
