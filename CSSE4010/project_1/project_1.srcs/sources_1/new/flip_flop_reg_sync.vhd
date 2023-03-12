----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 22.08.2021 19:00:29
-- Design Name: 
-- Module Name: flip_flop_reg_sync - Behavioral
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

entity flip_flop_reg_sync is
    Port ( DBus : in STD_LOGIC_VECTOR (3 downto 0);
           Reset : in STD_LOGIC;
           Set : in STD_LOGIC;
           QBus : inout STD_LOGIC_VECTOR (3 downto 0);
           Clk : in STD_LOGIC
           );
end flip_flop_reg_sync;

architecture Behavioral of flip_flop_reg_sync is

begin
    process(Set, Reset, Clk)
    begin
        if(Reset = '1') then
            QBus <= "0000";
        elsif(rising_edge(Set) and rising_edge(Clk)) then
            QBus <= DBus;
        end if;
    end process;
end Behavioral;
