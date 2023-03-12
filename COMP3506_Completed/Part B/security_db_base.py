import sys


class HashTableBase(object):
    """
    Python base code for Security Database.

    Use the following line to raise an alert
    sys.stderr.write("Suspicious behaviour")

    """
    max_capacity = 1021

    def __init__(self, n_planes, n_passengers):
        """
        Create an empty hashtable and a variable to count non-empty elements

        :param n_planes: planes per day
        :param n_passengers: passengers per plane
        """
        pass

    def __len__(self):
        """
        Returns the actual size of the hashtable, including the empty buckets
        :return: the size of the hashtable
        """
        raise NotImplementedError('must be implemented by subclass')

    def __getitem__(self, passport_id):
        """
        Finds a passenger's name by their passport id

        :param passport_id: string, represents passport id
        :return: The name of a person if they are in the system,
                 None otherwise
        """
        raise NotImplementedError('must be implemented by subclass')

    def __delitem__(self, passport_id):
        """
        Remove a passenger from the system

        :param passport_id: string, represents passport id
        :return: True is the passenger was deleted, False if they have not been found
        """
        raise NotImplementedError('must be implemented by subclass')

    def __contains__(self, passport_id):
        if self.__getitem__(passport_id):
            return True
        else:
            return False

    @staticmethod
    def hash_codes(key: str):
        """
        Calculate the hashcode based on the key

        :param key:
        :return: hashcode integer
        """
        raise NotImplementedError('must be implemented')

    def add_passenger(self, name, passport_id):
        """
        Add passenger to the hashtable

        :param name: string, represents passenger name
        :param passport_id: string, represents passport id
        :return: True if the passenger was successfully added,
                 False otherwise
        """
        raise NotImplementedError('must be implemented by subclass')

    def count(self):
        """
        Counts the number of passengers in the hashtable

        :return: the number of passengers
        """
        raise NotImplementedError('must be implemented by subclass')

    def get_index(self, passport_id):
        """
        Returns the bucket index of the passenger in the hashtable

        :param passport_id: string, represents passport id
        :return: integer index
        """
