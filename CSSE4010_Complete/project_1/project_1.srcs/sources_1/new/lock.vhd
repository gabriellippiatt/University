----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 21.08.2021 18:34:12
-- Design Name: 
-- Module Name: lock - Behavioral
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
----------------------------------------------------------------------------------6710


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity lock is
    Port ( A1Bus : in STD_LOGIC_VECTOR (3 downto 0);
           A2Bus : in STD_LOGIC_VECTOR (3 downto 0);
           B1Bus : in STD_LOGIC_VECTOR (3 downto 0);
           B2Bus : in STD_LOGIC_VECTOR (3 downto 0);
           Reset : in STD_LOGIC;
           Lock : out STD_LOGIC;
           Unlock : out STD_LOGIC);
end lock;

architecture Behavioral of lock is

begin
    process(A1Bus, A2Bus, B1Bus, B2Bus, Reset)
    begin
        if (Reset = '1') then
            Lock <= '1';
            Unlock <= '0';
        end if;
        if (A1Bus = x"6" and A2Bus = x"7" and B1Bus = x"1" and B2Bus = x"0") then
            Lock <= '0';
            Unlock <= '1';
        else
            Lock <= '1';
            Unlock <= '0';
        end if;
    end process;


end Behavioral;
