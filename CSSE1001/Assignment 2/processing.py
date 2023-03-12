"""
    Logical processing classes used in the second assignment for CSSE1001/7030.

    ProcessResults: Abstract class that defines the logical processing interface.
    AthleteResults: Provides details of one athlete’s results for all of the
                    events in which they competed.
    CountryResults: Provides a summary of the results of all athletes who
                    competed for one country.
    EventResults  : Provides details of the results of all athlete's who
                    competed in one event.
    DeterminePlaces: Determines the place ranking of all athletes who competed
                     in one event.
"""

__author__ = "Gabriel Lippiatt - s4529671"
__email__ = "g.lippiatt@uqconnect.edu.au"



from entities import Athlete, Result, Event, Country, ManagedDictionary
from entities import all_athletes, all_countries, all_events, load_data


class ProcessResults(object) :
    """Superclass for the logical processing commands."""

    _processing_counter = 0  # Number of times any process command has executed.

    def process(self) :
        """Abstract method representing collecting and processing results data.
        """
        ProcessResults._processing_counter += 1

    def get_results(self) :
        """Abstract method representing obtaining the processed results.

        Return:
            list: Subclasses will determine the contents of the resulting list.
        """
        raise NotImplementedError()


class AthleteResults(ProcessResults) :
    """Determines the results achieved by one athlete in all events."""

    _athlete_results_counter = 0  # Number of times this command has executed.

    def __init__(self, athlete) :
        """
        Parameters:
            athlete (Athlete): Athlete for whom we wish to
                                                    determine their results.
        """
        self._athlete = athlete
        self._results = []


    def process(self) :
        """Obtain all the results for this athlete and
           order them from best to worst placing.
           If two or more results have the same place they should be ordered
           by event name in ascending alphabetical order.
        """
        super().process()
        AthleteResults._athlete_results_counter += 1

        event_result = []

        # Find all the events an Athlete has participated in and
            # collect the results.
        for event in self._athlete.get_events():
            event_result.append(\
                            (event.get_name(), self._athlete.get_result(event)))

        # Sort the results in order of place and event name.
        event_result.sort(key = lambda x: (int(x[1].get_place()), x[0]))

        # Create a list of sorted results.
        self._results = [x[1] for x in event_result]


    def get_results(self) :
        """Obtain the processed results for _athlete.

        Return:
            list[Result]: Ordered list of the _athlete's results.
                          Sorted from best to worst, based on place in event.
                          Results with the same place are ordered by event name
                          in ascending alphabetical order.

        Raises:
            ValueError: If process has not yet been executed.

        """
        # If the process above has not been executed raise ValueError
            # otherwise return the list.
        if AthleteResults._athlete_results_counter == 0:
            raise ValueError
        else:
            return self._results

    def get_usage_ratio() :
        """Ratio of usage of the AthleteResults command against all commands.

        Return:
            float: ratio of _athlete_results_counter by _processing_counter.
        """
        return (AthleteResults._athlete_results_counter
                / AthleteResults._processing_counter)

    def __str__(self) :
        """(str) Return a formatted string of the results for this athlete."""
        """Implementation of this is optional but useful for observing your
           programs behaviour.
        """
        return ""


class CountryResults(ProcessResults) :
    """Determines the results achieved by all athletes in one country."""

    _country_results_counter = 0  # Number of times this command has executed.

    def __init__(self, country) :
        """
        Parameters:
            country (Country): Country for whom we wish to
                               determine all the results.
        """
        self._country = country
        self._results = []

    def process(self) :
        """Obtain all the results for this country and
           order them from best to worst placing.
           If two or more results have the same place they should be ordered
           by event name in ascending alphabetical order.
        """
        super().process()
        CountryResults._country_results_counter += 1

        gold = 0
        silver = 0
        bronze = 0
        number_of_athletes = 0

        # Count the mount of athletes in a certain countryself.
        # Count the amount of medals revieved by an athlete from a country.
        for athlete in self._country.get_athletes():
            number_of_athletes += 1
            for event in athlete.get_events():
                if athlete.get_result(event).get_medal() == "Gold":
                    gold += 1
                elif athlete.get_result(event).get_medal() == "Silver":
                    silver += 1
                elif athlete.get_result(event).get_medal() == "Bronze":
                    bronze += 1

        # Put variables into list.
        self._results = [gold, silver, bronze, number_of_athletes]

    def get_num_gold(self):
        """(int) Number of gold medals for country"""
        return self._results[0]

    def get_num_silver(self):
        """(int) Number of silver medals for country"""
        return self._results[1]

    def get_num_bronze(self):
        """(int) Number of bronze medals for country"""
        return self._results[2]

    def get_num_athletes(self):
        """(int) Number of athletes in a country"""
        return self._results[3]

    def get_results(self) :
        """Obtain the processed results for _country.

        Return:
            list[Result]: Ordered list of the _countries medals.
                          Sorted from best to worst, based on place in event.
                          Results with the same place are ordered by event name
                          in ascending alphabetical order.

        Raises:
            ValueError: If process has not yet been executed.
        """
        # If the process above has not been executed raise ValueError
            # otherwise return the list.
        if CountryResults._country_results_counter == 0:
            raise ValueError
        else:
            return self._results

    def get_usage_ratio() :
        """Ratio of usage of the CountryResults command against all commands.

        Return:
            float: ratio of _country_results_counter by _processing_counter.
        """
        return (CountryResults._country_results_counter
                / CountryResults._processing_counter)

    def __str__(self) :
        """(str) Return a formatted string of the results for this athlete."""
        """Implementation of this is optional but useful for observing your
           programs behaviour.
        """
        return ""


class EventResults(ProcessResults) :
    """Determines the results achieved by all athletes in one event."""

    _event_results_counter = 0  # Number of times this command has executed.

    def __init__(self, event) :
        """
        Parameters:
            event (Event): Event that we want to find all the athletes results.
        """
        self._event = event
        self._results = []

    def process(self) :
        """Obtain all the results for all the athletes in this event and
           order them from best to worst placing.
           If two or more results have the same place they should be ordered
           by event name in ascending alphabetical order.
        """
        super().process()
        EventResults._event_results_counter += 1

        athlete_result = []

        # Find all the athletes that have participated in an event and
            # collect the results.
        for athlete in self._event.get_athletes():
            athlete_result.append((athlete, athlete.get_result(self._event)))

        # Sort the results in order of place and event name.
        athlete_result.sort(\
                key = lambda x: (int(x[1].get_place()), x[0].get_full_name()))

        # Create a list of sorted results.
        self._results = [x[0] for x in athlete_result]


    def get_results(self) :
        """Obtain the processed results for _event.

        Return:
            list[Result]: Ordered list of the _event's results.
                          Sorted from best to worst, based on place in event.
                          Results with the same place are ordered by event name
                          in ascending alphabetical order.

        Raises:
            ValueError: If process has not yet been executed.
        """
        # If the process above has not been executed raise ValueError
            # otherwise return the list.
        if EventResults._event_results_counter == 0:
            raise ValueError
        else:
            return self._results

    def get_usage_ratio() :
        """Ratio of usage of the EventResults command against all commands.

        Return:
            float: ratio of _event_results_counter by _processing_counter.
        """
        return (EventResults._event_results_counter
                / EventResults._processing_counter)

    def __str__(self) :
        """(str) Return a formatted string of the results for this athlete."""
        """Implementation of this is optional but useful for observing your
           programs behaviour.
        """
        return ""


class DeterminePlaces(ProcessResults) :
    """Determines the results achieved by all athletes in one country."""

    _determine_places_counter = 0  # Number of times this command has executed.

    def __init__(self, event) :
        """
        Parameters:
            athlete (Athlete): Athlete for whom we wish to determine their results.
        """
        self._event = event
        self._results = []

    def process(self) :
        """Obtain all the results for this athlete and
           order them from best to worst placing.
           If two or more results have the same place they should be ordered
           by event name in ascending alphabetical order.
        """
        super().process()
        DeterminePlaces._determine_places_counter += 1

        pass

    def get_results(self) :
        """Obtain the processed results for _athlete.

        Return:
            list[Result]: Ordered list of the _athlete's results.
                          Sorted from best to worst, based on place in event.
                          Results with the same place are ordered by event name
                          in ascending alphabetical order.

        Raises:
            ValueError: If process has not yet been executed.
        """
        # If the process above has not been executed raise ValueError
            # otherwise return the list.
        if self._results == []:
            raise ValueError
        else:
            results = self._results.sort()
            return results

    def get_usage_ratio() :
        """Ratio of usage of the AthleteResults command against all commands.

        Return:
            float: ratio of _athlete_results_counter by _processing_counter.
        """
        return (DeterminePlaces._determine_places_counter
                / DeterminePlaces._processing_counter)

    def __str__(self) :
        """(str) Return a formatted string of the results for this athlete."""
        """Implementation of this is optional but useful for observing your
           programs behaviour.
        """
        return ""


def demo_entities() :
    """Simple test code to demonstrate using the entity classes.
       Output is to console.
    """
    TIMED = True
    SCORED = False

    print("Demonstrate creating country objects:")
    CAN = Country("Canada", "CAN")
    AUS = Country("Australia", "AUS")
    all_countries.add_item(CAN.get_country_code(), CAN)
    all_countries.add_item(AUS.get_country_code(), AUS)
    for country in all_countries.get_items() :
        print(country)

    print("\nDemonstrate creating athlete objects, adding them to",
          "all_athletes and countries:")
    a1 = Athlete("1", "Athlete", "One", CAN)
    a2 = Athlete("2", "Athlete", "Two", CAN)
    a3 = Athlete("10", "Athlete", "Three", CAN)
    a4 = Athlete("4", "Athlete", "Four", AUS)
    a5 = Athlete("5", "Athlete", "Five", AUS)
    a6 = Athlete("20", "Athlete", "Six", AUS)
    for athlete in [a1, a2, a3, a4, a5, a6] :
        all_athletes.add_item(athlete.get_id(), athlete)
    athletes = all_athletes.get_items()
    for athlete in athletes :
        print(athlete)
    CAN.add_athletes([a1, a2, a3])
    AUS.add_athletes([a4, a5, a6])
    print("\nDemonstrate finding an athlete in all_athletes:")
    print(all_athletes.find_item("2"))

    # Create event objects and add athletes to the events.
    e1 = Event("Event1", TIMED, [a1, a2, a3, a4, a5])
    all_events.add_item(e1.get_name(), e1)
    a2.add_event(e1)
    a3.add_event(e1)
    a4.add_event(e1)
    a5.add_event(e1)
    e2 = Event("Event2", SCORED, [a1, a2, a3, a5, a6])
    all_events.add_item(e2.get_name(), e2)
    a2.add_event(e2)
    a3.add_event(e2)
    a5.add_event(e2)
    a6.add_event(e2)
    a1.add_events([e1, e2])

    # Create result objects for each athlete in the events.
    a1.add_result(e1, Result(10.5))
    a2.add_result(e1, Result(9.5))
    a3.add_result(e1, Result(11.5))
    a4.add_result(e1, Result(8.5))
    a5.add_result(e1, Result(12.5))

    a1.add_result(e2, Result(100.5))
    a2.add_result(e2, Result(99.5))
    a3.add_result(e2, Result(98.5))
    a5.add_result(e2, Result(90.5))
    a6.add_result(e2, Result(89.5))



def demo_processing() :
    """Simple test code to demonstrate using the processing classes.
       Output is to console.
    """
    print("\n\nDemonstrate processing of results:")
    for athlete in all_athletes.get_items() :
        athlete_results = AthleteResults(athlete)
        athlete_results.process()
        results = athlete_results.get_results()
        # Do something with this athlete's results.

    print("\nDemonstrate listing the results for an event:")
    event = all_events.find_item("Event1")
    results_dict = {}
    for athlete in event.get_athletes() :
        results_dict[athlete.get_result(event).get_result()] = \
            athlete.get_result(event)
    for result in sorted(results_dict) :
        print(result)

    print("\nAthleteResults was used",
          "{0:.1%}".format(AthleteResults.get_usage_ratio()),
          "of the time of all results processing commands.")



if __name__ == "__main__" :
    demo_entities()
    demo_processing()
