----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 22.08.2021 16:24:32
-- Design Name: 
-- Module Name: sev_seg - Behavioral
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

entity sev_seg is
    Port ( DBus : in STD_LOGIC_VECTOR (3 downto 0);
           SSDBus : out STD_LOGIC_VECTOR (6 downto 0);
           Reset : in STD_LOGIC);
end sev_seg;

architecture Behavioral of sev_seg is

begin
    process(DBus, Reset)
    begin
        if(Reset = '1') then
            SSDBus <= "1110111";
        else
        
            if(DBus = "0000") then
                SSDBus <= "1111110";
            elsif(DBus = "0001") then
                SSDBus <= "0110000";
            elsif(DBus = "0010") then
                SSDBus <= "1101101";
            elsif(DBus = "0011") then
                SSDBus <= "1111001";
            elsif(DBus = "0100") then
                SSDBus <= "0110011";   
            elsif(DBus = "0101") then
                SSDBus <= "1011011";
            elsif(DBus = "0110") then
                SSDBus <= "1011111";
            elsif(DBus = "0111") then
                SSDBus <= "1110000";
            elsif(DBus = "1000") then
                SSDBus <= "1111111";
            elsif(DBus = "1001") then
                SSDBus <= "1111011";
            elsif(DBus = "1010") then
                SSDBus <= "1110111";
            elsif(DBus = "1011") then
                SSDBus <= "0011111";
            elsif(DBus = "1100") then
                SSDBus <= "0111101";
            elsif(DBus = "1101") then
                SSDBus <= "1001111";
            elsif(DBus = "1111") then
                SSDBus <= "1000111";
            end if;
            end if;
    end process;

end Behavioral;
