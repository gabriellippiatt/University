----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 21.08.2021 21:00:09
-- Design Name: 
-- Module Name: test_locktop_sync - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity test_locktop_sync is
--  Port ( );
end test_locktop_sync;

architecture Behavioral of test_locktop_sync is
    component locktop_sync 
    PORT (
           DABus : in STD_LOGIC_VECTOR (3 downto 0);
           DBBus : in STD_LOGIC_VECTOR (3 downto 0);
           But1: in std_logic;
           Clock: in std_logic;
           But2: in std_logic;
           Clr: in std_logic;
           LockOpen : out STD_LOGIC;
           LockClosed : out STD_LOGIC;
           SSD1Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD2Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD3Bus : out STD_LOGIC_VECTOR (6 downto 0);
           SSD4Bus : out STD_LOGIC_VECTOR (6 downto 0)
    );
    end component;
    signal inA: std_logic_vector(3 downto 0);
    signal inB: std_logic_vector(3 downto 0);
    signal inBut1: std_logic;
    signal inBut2: std_logic;
    signal inReset: std_logic;
    signal inClock: std_logic;
    signal outLock: std_logic;
    signal outUnlock: std_logic;
    signal TSS1: std_logic_vector(6 downto 0);
    signal TSS2: std_logic_vector(6 downto 0);
    signal TSS3: std_logic_vector(6 downto 0);
    signal TSS4: std_logic_vector(6 downto 0);
    signal testpattern: std_logic_vector(95 downto 0);

begin
uut: locktop_sync port map (
    DABus => inA,
    DBBus => inB,
    But1 => inBut1,
    But2 => inBut2,
    Clr => inReset,
    Clock => inClock,
    LockOpen => outLock,
    LockClosed => outUnlock,
    SSD1Bus => TSS1,
    SSD2Bus => TSS2,
    SSD3Bus => TSS3,
    SSD4Bus => TSS4
);
  input_gen: process
  begin 
    --test pattern: 4529671054387601670B4532
    --test pattern Reversed: 2354B0761067834501769254
    testpattern <= x"2354B0761067834501769254";
    for I in 0 to 5 loop
        inReset <= '1';
        inClock <= '0';
        wait for 5ps;
        inBut1 <= '0';
        inBut2 <= '0';
        inReset <= '0';
        inClock <= '0';
        inA <= testpattern(((16*I+3)) downto (16*I));  
        inB <= testpattern(((16*I+7)) downto (16*I+4)); 
        inBut1 <= '1';
        inClock <= '1';
        inBut2 <= '0';
        wait for 5ps;
        inClock <= '0';
        inBut1 <= '0';
        inBut2 <= '0';
        wait for 2ps;
        inA <= testpattern(((16*I+11)) downto (16*I+8));
        inB <= testpattern(((16*I+15)) downto (16*I+12));
        inBut1 <= '0';
        inClock <= '1';
        inBut2 <= '1';
        wait for 5ps;
        inBut1 <= '0';
        inBut2 <= '0';
        inClock <= '0';
    end loop;
    end process;
end Behavioral;
