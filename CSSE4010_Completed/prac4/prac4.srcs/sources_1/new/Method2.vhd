----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 19.09.2021 21:27:42
-- Design Name: 
-- Module Name: Method2 - Behavioral
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

entity Method2 is
    Port ( Clock : in STD_LOGIC;
           InputSeq : in STD_LOGIC;
           Reset : in STD_LOGIC;
           ZFlag : out STD_LOGIC);
end Method2;

architecture Behavioral of Method2 is
component FSM1
    Port ( Clock : in STD_LOGIC;
           Input : in STD_LOGIC;
           Reset : in STD_LOGIC;
           Detector : inout STD_LOGIC);
end component;
component FSM2
    Port ( Clock : in STD_LOGIC;
           State : in STD_LOGIC;
           Z : out STD_LOGIC);
end component;

SIGNAL raiseflag :  std_logic;

begin

F1: FSM1 PORT MAP (
    Clock => Clock,
    Input => InputSeq,
    Reset => Reset,
    Detector => raiseflag
);

F2: FSM2 PORT MAP (
    Clock => Clock,
    State => raiseflag,
    Z => ZFlag
);

end Behavioral;
