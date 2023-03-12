----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 21:17:08
-- Design Name: 
-- Module Name: FSM2_Test - Behavioral
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

entity FSM2_Test is
--  Port ( );
end FSM2_Test;

architecture Behavioral of FSM2_Test is
    component FSM2
    Port ( Clock : in STD_LOGIC;
           State : in STD_LOGIC;
           Z : out STD_LOGIC);
end component;

signal inClock: std_logic;
signal inState: std_logic;
signal OutZ: std_logic;

constant ClockPeriod : time := 10 ps;
begin
uut: FSM2 port map (
    Clock => inClock,
    State => inState,
    Z => outZ
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

inState <= '0';
wait for 10 ps;
inState <= '1';
wait for 10 ps;
inState <= '0';
wait for 10 ps;
wait for 10 ps;
wait for 10 ps;
end process;
end Behavioral;
