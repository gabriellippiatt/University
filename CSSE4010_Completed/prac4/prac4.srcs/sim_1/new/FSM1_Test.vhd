----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 19:03:50
-- Design Name: 
-- Module Name: FSM1_Test - Behavioral
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

entity FSM1_Test is
--  Port ( );
end FSM1_Test;

architecture Behavioral of FSM1_Test is
    component FSM1
    Port ( Clock : in STD_LOGIC;
           Input : in STD_LOGIC;
           Reset : in STD_LOGIC;
           Detector : inout STD_LOGIC);
end component;

signal inClock: std_logic;
signal inInput: std_logic;
signal InReset: std_logic;
signal OutDetector: std_logic;

constant ClockPeriod : time := 10 ps;
begin
uut: FSM1 port map (
    Clock => inClock,
    Input => inInput,
    Reset => inReset,
    Detector => outDetector
);

clock_process :process
begin
inClock <= '0';
wait for ClockPeriod/2;
inClock <= '1';
wait for ClockPeriod/2;
end process;

input_gen: process
begin

inInput <= '0';
inReset <= '1';
wait for 10 ps;
inReset <= '0';
wait for 10 ps;
inInput <= '1';
wait for 10 ps;
inInput <= '1';
wait for 10 ps;
inInput <= '0'; 
wait for 10 ps;
inInput <= '0'; 
wait for 10 ps;
inInput <= '1'; 
wait for 10 ps;
inInput <= '0'; 
wait for 10 ps;

end process;
end Behavioral;
