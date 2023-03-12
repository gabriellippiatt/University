from display_base import DisplayRandomBase, DisplayPartiallySortedBase


class DisplayRandom(DisplayRandomBase):
    # Initialisation
    def __init__(self,lines):
        super().__init__(lines)

    # Merge function for the time
    def merge_time(self, arr, l, m, r):

        n1 = m - l + 1
        n2 = r - m
        L = [None] * (n1)
        R = [None] * (n2)
    

        for i in range(0, n1):
            L[i] = arr[l + i]
    
        for j in range(0, n2):
            R[j] = arr[m + 1 + j]
    
        # Merge the temp arrays back into arr
        i = 0
        j = 0
        k = l
        while i < n1 and j < n2:
            if L[i].time <= R[j].time:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1
    
        # Copy the remaining elements of L
        while i < n1:
            arr[k] = L[i]
            i += 1
            k += 1
    
        # Copy the remaining elements of R
        while j < n2:
            arr[k] = R[j]
            j += 1
            k += 1

    # Sort function for the time
    def sort_time(self,arr,l,r):
        if l < r:

            m = l+(r-l)//2
    
            # Sort first and second halves
            self.sort_time(arr, l, m)
            self.sort_time(arr, m+1, r)
            self.merge_time(arr, l, m, r)
    
    # Merge function for the name
    def merge_name(self, arr, l, m, r):
        n1 = m - l + 1
        n2 = r - m
        L = [None] * (n1)
        R = [None] * (n2)

        for i in range(0, n1):
            L[i] = arr[l + i]
    
        for j in range(0, n2):
            R[j] = arr[m + 1 + j]
    
        # Merge the temp arrays back into arr
        i = 0
        j = 0
        k = l

        while i < n1 and j < n2:
            if L[i].plane_number <= R[j].plane_number:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1
    
        # Copy the remaining elements of L
        while i < n1:
            arr[k] = L[i]
            i += 1
            k += 1
    
        # Copy the remaining elements of R
        while j < n2:
            arr[k] = R[j]
            j += 1
            k += 1

    # Sort function for the name
    def sort_name(self,arr,l,r):
        if l < r:

            m = l+(r-l)//2
    
            # Sort first and second halves
            self.sort_name(arr, l, m)
            self.sort_name(arr, m+1, r)
            self.merge_name(arr, l, m, r)

    # Main sort function
    def sort(self):

        # Sort by name first
        self.sort_name(self.data, 0, (len(self.data) - 1))
        # Sort by time last
        self.sort_time(self.data, 0, (len(self.data) - 1))

        return self.data


class DisplayPartiallySorted(DisplayPartiallySortedBase):
    # Initialisation
    def __init__(self, schedule_lines, extra_plane_lines):
        super().__init__(schedule_lines, extra_plane_lines)
        self.combined_lines = self.schedule + self.extra_planes

    # Inseriton sort function for Time
    def insertionSort(self, arr):
        keystart = len(self.schedule)
        # Loop through given array and sort
        for i in range(keystart, len(arr)):
    
            key = arr[i]
            j = i-1
            while j >= 0 and (key.time < arr[j].time or \
                    (key.time == arr[j].time and key.name < arr[j].name)) :
                    arr[j + 1] = arr[j]
                    j -= 1
            arr[j + 1] = key
    
    # Main sort function
    def sort(self):

        self.insertionSort(self.combined_lines)
        return self.combined_lines


