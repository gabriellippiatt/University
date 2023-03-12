from security_db_base import HashTableBase
import sys

class Passengers:
    def __init__(self, pass_name, pass_id):
        self.pass_id = pass_id
        self.pass_name = pass_name
        self.pass_counter = 0

class HashTable(HashTableBase):
    """
        Implement all the necessary methods here
    """
    def __init__(self, n_planes, n_passengers): 
        self.n_planes = n_planes
        self.n_passengers = n_passengers
        self.size = self.get_prime(n_planes*n_passengers)
        self.passengers_list = [None]*self.size
        self.passenger_count = 0


    def __len__(self):
        """
        Returns the actual size of the hashtable, including the empty buckets
        :return: the size of the hashtable
        """
        return self.size

    def __getitem__(self, passport_id):
        """
        Finds a passenger's name by their passport id

        :param passport_id: string, represents passport id
        :return: The name of a person if they are in the system,
                 None otherwise
        """
        if self.passengers_list[self.get_index(passport_id)] == None:
            return None
        else :
            return self.passengers_list[self.get_index(passport_id)].pass_name

    def __delitem__(self, passport_id):
        """
        Remove a passenger from the system

        :param passport_id: string, represents passport id
        :return: True is the passenger was deleted, False if they have not been found
        """
        if self.__contains__(passport_id):
            self.passengers_list[self.get_index(passport_id)] = None
            self.passenger_count -= 1

            return True
        else :
            return False

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
        hashcode = 0
        buffer = [None] * len(key)
        counter = 0

        for x in key :
            buffer[counter] = ord(x)
            hashcode += 1
            counter += 1
            for y in buffer :
                if y != None :
                    hashcode += y
        return hashcode

    def compression(self, hashcode):
        m = self.n_passengers * self.n_planes

        return hashcode % m

    def add_passenger(self, name, passport_id):
        """
        Add passenger to the hashtable

        :param name: string, represents passenger name
        :param passport_id: string, represents passport id
        :return: True if the passenger was successfully added,
                 False otherwise
        """
        passenger = Passengers(name, passport_id)
        
        if self.passenger_count != 1021:
            if self.passengers_list[self.get_index(passport_id)] != None :
                if self.passengers_list[self.get_index(passport_id)].pass_name != name :
                    sys.stderr.write("Suspicious behaviour")
                    return False

                if self.passengers_list[self.get_index(passport_id)].pass_name == name :
                    self.passengers_list[self.get_index(passport_id)].pass_counter += 1
                    
                    if self.passengers_list[self.get_index(passport_id)].pass_counter > 5:
                        sys.stderr.write("Suspicious behaviour")
                        return False
                    else :
                        return True

            elif self.passengers_list[self.get_index(passport_id)] == None :
                if not self.__contains__(passport_id):
                    self.passengers_list[self.get_index(passport_id)] = passenger
                    self.passenger_count += 1
                    passenger.pass_counter += 1
                    return True
                else:
                    return False
        else :
            return False

    def count(self):
        """
        Counts the number of passengers in the hashtable

        :return: the number of passengers
        """
        return self.passenger_count

    def get_index(self, passport_id):
        """
        Returns the bucket index of the passenger in the hashtable

        :param passport_id: string, represents passport id
        :return: integer index
        """
        self.check_index = self.compression(self.hash_codes(passport_id))
        self.bucket_index = self.check_index
        flag = False

        # resize the passenger list if required
        if self.passenger_count >= self.size:
            self.passengers_list += [None]*(1021 - self.size)
            self.size = 1021

        # get the index
        while flag == False:
            if self.passengers_list[self.bucket_index] == None:
                flag = True
            elif self.passengers_list[self.bucket_index].pass_id == passport_id :
                flag = True
            else :
                if self.bucket_index != len(self.passengers_list) :
                    self.bucket_index += 1
                else :
                    self.bucket_index = 0

        return self.bucket_index


    def isPrime(self, number):
     
        if(number <= 1):
            return False
        if(number <= 3):
            return True

        if(number % 2 == 0 or number % 3 == 0):
            return False
        
        for i in range(5,int(number**0.5 + 1), 6):
            if(number % i == 0 or number % (i + 2) == 0):
                return False
        
        return True
    

    def get_prime(self, number) :

        if (number <= 1):
            return 2
    
        prime = number
        found = False
    
        while(not found):
            prime = prime + 1
    
            if(self.isPrime(prime) == True):
                found = True
    
        return prime



# if __name__ == "__main__":
#     """
#         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#         REMOVE THE MAIN FUNCTION BEFORE SUBMITTING TO THE AUTOGRADER 
#         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#         The following main function is provided for simple debugging only
#     """

#     # ht = HashTable(3, 2)
#     # # add
#     # ht.add_passenger("Rob Bekker", "Asb23f")
#     # ht.add_passenger("Kira Adams", "MKSD23")
#     # ht.add_passenger("Kira Adams", "MKSD24")
#     # # for i in ht.passengers_list:
#     # #     if i != None :
#     # #         print(i.pass_id)
#     # assert "Asb23f" in ht

#     # # count
#     # assert ht.count() == 3

#     # # del
#     # del ht["MKSD23"]
#     # # for i in ht.passengers_list:
#     # #     if i != None :
#     # #         print(i.pass_id)
#     # assert "MKSD23" not in ht
#     # assert "Asb23f" in ht

#     # # hashcodes
#     # assert ht.hash_codes("Asb23f") == 1717

#     # # suspicious
#     # ht = HashTable(3, 2)
#     # ht.add_passenger("Rob Bekker", "Asb23f")
#     # ht.add_passenger("Robert Bekker", "Asb23f")
#     # Should raise a warning
#     # sys.stderr.write("Suspicious behaviour")
    
#     ht = HashTable(3, 2)
#     ht.add_passenger("Gabe Lippy", "GLJJ77")
#     ht.add_passenger("Gabe Lippy", "GLJJ77")
#     ht.add_passenger("Gabe Lippy", "GLJJ77")
#     ht.add_passenger("Gabe Lippy", "GLJJ77")
#     ht.add_passenger("Gabe Lippy", "GLJJ77")
#     ht.add_passenger("Gabe Lippy", "GLJJ77")

# /////////////////////////// Fadz/////////////////////////////

    # ht = HashTable(3, 2)
    # # add
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Kira Adams", "MKSD23")
    # ht.add_passenger("Kira Adams", "MKSD24")
    # assert "Asb23f" in ht

    # # count
    # assert ht.count() == 3

    # # del
    # del ht["MKSD23"]
    # assert "MKSD23" not in ht
    # assert "Asb23f" in ht

    # # hashcodes
    # assert ht.hash_codes("Asb23f") == 1717

    # # suspicious
    # ht = HashTable(3, 2)
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Robert Bekker", "Asb23f")
    # # Should raise a warning
    # # sys.stderr.write("Suspicious behaviour")

    # # Check resize
    # ht = HashTable(2, 1)
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekkera", "Asb23a")
    # assert len(ht) == 3
    # ht.add_passenger("Rob Bekker1", "Asb232")
    # ht.add_passenger("Rob Bekker2", "Afb23f")
    # ht.add_passenger("Rob Bekker3", "A1b23f")
    # assert len(ht) == ht.max_capacity
    # assert ht.count() == 5

    # del ht['Asb23f']
    # assert 'Asb23f' not in ht

    # # Check remove passenger that does not exist
    # ht = HashTable(1, 2)
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # assert 'MKSD23' not in ht
    # assert len(ht) == 3
    # assert not ht.__delitem__('MKSD23')

    # # Check 5 passes
    # ht = HashTable(1, 6)
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # ht.add_passenger("Rob Bekker", "Asb23f")
    # # Should raise a warning
    # # sys.stderr.write("Suspicious behaviour")
    # assert 'Asb23f' in ht
    # # Check theres only 1 passenger
    # assert ht.count() == 1

    # ht = HashTable(1, 2)
    # assert ht.add_passenger("Rob Bekker", "Asb23f")
    # assert not ht.add_passenger("Rob Bekkera", "Asb23f")
    # # Should raise a warning
    # # sys.stderr.write("Suspicious behaviour")
    # assert not ht.add_passenger("Rob Bekker", "Asb23f")