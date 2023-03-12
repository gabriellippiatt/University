"""Data Cleanser"""

__author__ = "Gabriel Lippiatt 45296710"
__date__ = "16/03/18"


from assign1_utilities import get_column, replace_column, truncate_string


def remove_athlete_id(row) :
    """
    Removes the Athlete ID by removing all characters before the
    first comma plus one (the space between the first comma and the event name)
    """
    return row[row.find(',') + 1:]

def is_string_valid(string, valid_characters):
    # iff = if and only if

    """
    (bool) Returns True iff string is made up of valid characters.

    Parameters:
        string (str): The string to check
        valid_characters (str): The only valid characters
    """

    i = 0
    while i <len(string):
        character = string[i]
        if character not in valid_characters:
            return False # invalid string
        i += 1

    return True # valid string


def required_value(string):
    return string # Used to test if field has required value

import string # Imports what Python determins as valid characters
VALID = string.ascii_letters + string.digits + "-' "

def main() :

    # Main functionality of program.
    
    with open("athlete_data.csv", "r") as raw_data_file, \
         open("athlete_data_clean.csv", "w") as clean_data_file :
        for row in raw_data_file :
            corrupt = False
            row = remove_athlete_id(row)
            row_to_process = row # Saves row in original state,
                                 # minus athlete id

            """
            Bellow is the code that checks for valid characters
            in the first 4 columns
            """

            for i in (0, 1, 2, 3):
                if is_string_valid(get_column(row, i), VALID) == False:
                    corrupt = True # Invalid characters -> Corrupt
                    
            """
            Truncates the first three columns to 30 characters
            and makes sure there is a value in the required field
            for the first 4 columns
            """

            for i in range(3):
                if len(get_column(row, i)) > 30 :
                    row_to_process = \
                        replace_column(row, \
                                   truncate_string(get_column(row, i), 30), i)
                                 # Limits field to 30 characters -> Not Corrupt
            for i in range(4):
                if required_value(get_column(row, i)) == "":
                    corrupt = True # No characters in required field -> Corrupt
    

            # COUNTRY CODE


            if len(get_column(row, 3)) > 3 :
                corrupt = True # Code with more than 3 characters -> Corrupt
                
            country = get_column(row, 3)
            row_to_process = replace_column(row_to_process, country.upper(),3)
                                                # Capatalises all country codes
                

            # PLACE

                
            if len(get_column(row, 4)) > 3 :
                corrupt = True # Place with more than 3 characters -> Corrupt
            """
            The below code states that any value that is not a digit, DNS, DNF,
            PEN or NONE, is corrupt

                DNS - Did not start
                DNF - Did not finish
                PEN - Penalty
                NONE ("") - No Place
                
            If there is a value present there must be a corrisponding score
            or time. place_present represents whether or not there is a legal
            value in the field.
            """
            place = get_column(row, 4) # 4th Column is the Athlete's place
            
            if place in ("DNS", "DNF", "PEN", ""):
                place_present = 0 # They did not place
            elif place.isdigit() == True:
                place_present = 1 # They did place and require
                                  # a corrisponding score or time
            else :
                corrupt = True  # if there is not a legal value in the field,
                                # then the line is corrupt
            """
            The below code checks to see if the values in column 5 and 6
            are valid. If not, line is Corrupt.
            """

            for i in (5, 6):
                if is_string_valid(get_column(row, i), string.digits + ".") \
                                                                       == False:
                    corrupt = True # Invalid characters -> Corrupt


            # SCORE


            score = get_column(row, 5) # 5th Column is the Athlete's score
            
            if len(get_column(row, 5)) > 6 :
                corrupt = True # Score with more than 6 characters -> Corrupt

            if score == "":
                score_present = 0 # Valid score is NOT present
            else:
                score_present = 1 # Valid score IS present
                

            # TIME

            
            time = get_column(row, 6) # 6th Column is the Athlete's time
           
            if len(get_column(row, 6)) > 8 :
                corrupt = True # Score with more than 8 characters -> Corrupt
                
            if time == "":
                time_present = 0 # Valid time is NOT present
            else :
                time_present = 1 # Valid time IS present

            """
            If the Athlete has a place then they must have a corrisponding
            score or time but not both. The below code prevents the Athelete
            from placing and having both a score and a time. If they do, then
            the row is corrupt.
            """
                
            if (time_present == 1) and (score_present == 1) :
                corrupt = True # Athlete with both score and time -> Corrupt
            elif (time_present == 0) and (score_present == 0) and \
                                                        (place_present == 1) :
                corrupt = True  # If Athlete places then they must have a score
                                # or a time. If not, then line is Corrupt
            elif (time_present == 0) and (score_present == 0) and \
                                                        (place_present == 0) :
                pass # If Athlete does not place then there will be no
                     # corrisponding time or score.

            
            # MEDALS


            medal_value = get_column(row, 7) # 7th Column is Athlete's Medal
            
            medal_name = medal_value.title() # Capatalises the first character
                                             # and the rest lower case
                                             
            row_to_process = replace_column(row_to_process, medal_name,7)
                    # Replaces value in column 7 with the re-capatalised value

            """
            If the Athelete comes first second or third they require a medal
                First Place - Gold
                Second Place - Silver
                Third PLace - Bronze
            """
            if (medal_name == "Gold") and (place == "1") or \
                            (medal_name == "Silver") and (place == "2") or \
                            (medal_name == "Bronze") and (place == "3") or \
                                                            medal_name == "":
                pass
            else:
                corrupt = True  # Validate column 7. There can only be 4 values:
                                # Gold, Silver, Brone or "" (no medal/value)
                
            if place == "1" and medal_name != "Gold":
                corrupt = True
            elif place == "2" and medal_name != "Silver":
                corrupt = True
            elif place == "3" and medal_name != "Bronze":
                corrupt = True  # If they placed 1st, 2nd, or 3rd but there is
                                # no medal value in the 7th collumn then the
                                # line is corrupt
            """
            The code below checks to see if the character in the 8th, 9th,
            and 10th column are less than 8 character. If not -> Corrupt
            """

            for i in (8, 9, 10):
                if len(get_column(row, i)) > 8 :
                    corrupt = True # Record with more than 8 characters
                                   # -> Corrupt


            # OLYMPIC RECORD


            olympic_record = get_column(row, 8) # 8th Column is the Athlete's
                                                # Olympic record if set

                
            # WORLD RECORD 


            world_record = get_column(row, 9)   # 9th Column is the Athlete's
                                                # World record if set
            """
            If a world record is set then it must be the same as the olympic
            record otherwise the line is corrupt
            """
            if (world_record.isdigit() == True) and \
                                               (world_record != olympic_record):
                corrupt = True  # if World record does not equal olympic record
                                # then the line is corrupt

            """
            A track record may be set but it may not be the world or olympic
            record and therefore does not require any constraints
            """

            
            # Save the row data to the cleaned data file.
            if not corrupt :
                clean_data_file.write(row_to_process)
            else :
                row = row[:-1]      # Remove new line character at end of row.
                clean_data_file.write(row + ",CORRUPT\n")    



# Call the main() function if this module is executed
if __name__ == "__main__" :
    main()
