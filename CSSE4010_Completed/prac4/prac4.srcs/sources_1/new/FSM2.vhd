----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 21:08:11
-- Design Name: 
-- Module Name: FSM2 - Behavioral
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

entity FSM2 is
    Port ( Clock : in STD_LOGIC;
           State : in STD_LOGIC;
           Z : out STD_LOGIC);
end FSM2;

architecture Behavioral of FSM2 is
type FSM is (FlagZero, FlagOne, FlagTwo);
signal CurrentState, NextState: FSM;
begin

-- Clock
process(Clock)
begin
 if(rising_edge(clock)) then
  CurrentState <= NextState;
 end if;
end process;

-- Sequence Detection
process(CurrentState,State)
begin
 case(CurrentState) is
 when FlagZero =>
  z <= '0';
  if(State='1') then
   NextState <= FlagOne;
  else
   NextState <= FlagZero;
  end if;
 when FlagOne =>
  z <= '1';
  if(State='1') then
   NextState <= FlagTwo;
  else
   NextState <= FlagTwo;
  end if;  
 when FlagTwo => 
  z <= '1';
  if(State='1') then
   NextState <= FlagZero; -- Note Possible
  else
   NextState <= FlagZero;
  end if;  
 end case;
end process;
end Behavioral;
