import tkinter as tk
import serial
import time
import threading
import json
import os 
from random import randrange, uniform
import time
import math
import numpy as np
import statistics

class Multi:
    def __init__(self):
        self.rk = 0
        self.r1 = 0
        self.r2 = 0
        self.r3 = 0
        self.r4 = 0
        self.r5 = 0
        self.r6 = 0
        self.r7 = 0
        self.r8 = 0
        self.r9 = 0
        self.r10 = 0
        self.r11 = 0

        self.x1 = 133
        self.y1 = 0
        self.x2 = 266
        self.y2 = 0
        self.x3 = 400
        self.y3 = 0
        self.x4 = 400
        self.y4 = 133
        self.x5 = 400
        self.y5 = 266
        self.x6 = 400 
        self.y6 = 400
        self.x7 = 266
        self.y7 = 400
        self.x8 = 133
        self.y8 = 400
        self.x9 = 0
        self.y9 = 400
        self.x10 = 266
        self.y10 = 400
        self.x11 = 133
        self.y11 = 400
        self.xk = 0
        self.yk = 0
        
        self.w1 = 0
        self.w2 = 0 

        # #A2
        # self.x1 = 133
        # self.y1 = 0
        # #snode
        # self.x2 = 0
        # self.y2 = 400
        # #snode
        # self.x3 = 400
        # self.y3 = 0

        self.multw_x = 0
        self.multw_y = 0
        self.multk_x = 0
        self.multk_y = 0

    def multiration(self):
        b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2)
        b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2)
        b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
        b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2)
        b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
        b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
        b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
        b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
        b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
        b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
        b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)

        a11 = 2 * -self.x1
        a12 = 2 * -self.y1
        a21 = 2 * -self.x2
        a22 = 2 * -self.y2
        a31 = 2 * -self.x3
        a32 = 2 * -self.y3
        a41 = 2 * -self.x4
        a42 = 2 * -self.y4
        a51 = 2 * -self.x5
        a52 = 2 * -self.y5
        a61 = 2 * -self.x6
        a62 = 2 * -self.y6
        a71 = 2 * -self.x7
        a72 = 2 * -self.y7
        a81 = 2 * -self.x8
        a82 = 2 * -self.y8
        a91 = 2 * -self.x9
        a92 = 2 * -self.y9
        a101 = 2 * -self.x10
        a102 = 2 * -self.y10
        a111 = 2 * -self.y11
        a112 = 2 * -self.y11

        b = np.array([[b1], [b2], [b3],[b4], [b5], [b6],[b7], [b8], [b9],[b10], [b11]])
        A = np.array([[a11, a12], [a21, a22], [a31, a32], [a41,a42], [a51,a52], [a61,a62] ,[a71,a72] , [a81,a82], [a91,a92], [a101,a102], [a111,a112]])
        
        x, y = np.linalg.lstsq(A, b, rcond=None)[0]
        print("XX:",x,"YY:",y)
        x = 0 if x < 0 else x
        x = 400 if 400 < x else x
        y = 0 if y < 0 else y
        y = 400 if 401 < y else y
        print("x:",x,"y:",y)
        return x,y

    def cl_multiration(self,cl):
        b1 = 0
        b2 = 0
        b3 = 0
        b4 = 0
        b5 = 0
        b6 = 0
        b7 = 0
        b8 = 0
        b9 = 10
        b10 = 0
        b11 = 0
        if cl == 1:
            return 0,0
        if cl == 2:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) * 400
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2)
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2)
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        elif cl == 3:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) * 400
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2)
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        elif cl == 3:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2) * 400
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2)
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)

        elif cl == 4:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) * 400
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)

        elif cl == 5:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) * 400
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) 
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 6:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2) * 400
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 7:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2) * 400
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 8:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2) * 400
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 9:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2) * 400
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 10:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2) * 400
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)
        
        elif cl == 11:
            b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) 
            b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2) 
            b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
            b4 = math.pow(self.r4, 2) - math.pow(self.rk, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2) 
            b5 = math.pow(self.r5, 2) - math.pow(self.rk, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2) 
            b6 = math.pow(self.r6, 2) - math.pow(self.rk, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)
            b7 = math.pow(self.r7, 2) - math.pow(self.rk, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
            b8 = math.pow(self.r8, 2) - math.pow(self.rk, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
            b9 = math.pow(self.r9, 2) - math.pow(self.rk, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
            b10 = math.pow(self.r10, 2) - math.pow(self.rk, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
            b11 = math.pow(self.r11, 2) - math.pow(self.rk, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2) * 400
        
        



        a11 = 2 * -self.x1
        a12 = 2 * -self.y1
        a21 = 2 * -self.x2
        a22 = 2 * -self.y2
        a31 = 2 * -self.x3
        a32 = 2 * -self.y3
        a41 = 2 * -self.x4
        a42 = 2 * -self.y4
        a51 = 2 * -self.x5
        a52 = 2 * -self.y5
        a61 = 2 * -self.x6
        a62 = 2 * -self.y6
        a71 = 2 * -self.x7
        a72 = 2 * -self.y7
        a81 = 2 * -self.x8
        a82 = 2 * -self.y8
        a91 = 2 * -self.x9
        a92 = 2 * -self.y9
        a101 = 2 * -self.x10
        a102 = 2 * -self.y10
        a111 = 2 * -self.y11
        a112 = 2 * -self.y11

        b = np.array([[b1], [b2], [b3],[b4], [b5], [b6],[b7], [b8], [b9],[b10], [b11]])
        A = np.array([[a11, a12], [a21, a22], [a31, a32], [a41,a42], [a51,a52], [a61,a62] ,[a71,a72] , [a81,a82], [a91,a92], [a101,a102], [a111,a112]])
        
        x, y = np.linalg.lstsq(A, b, rcond=None)[0]
        print("XX:",x,"YY:",y)
        x = 0 if x < 0 else x
        x = 400 if 400 < x else x
        y = 0 if y < 0 else y
        y = 400 if 401 < y else y
        print("x:",x,"y:",y)
        return x,y

    def weigthed_multi(self):
        b1 = math.pow(self.r1, 2) - math.pow(self.w1, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2)
        b2 = math.pow(self.r2, 2) - math.pow(self.w1, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2)
        b3 = math.pow(self.r3, 2) - math.pow(self.w1, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)
        b4 = math.pow(self.r4, 2) - math.pow(self.w1, 2) - math.pow(self.x4, 2) - math.pow(self.y4, 2)
        b5 = math.pow(self.r5, 2) - math.pow(self.w1, 2) - math.pow(self.x5, 2) - math.pow(self.y5, 2)
        b6 = (math.pow(self.w2, 2) - math.pow(self.w1, 2) - math.pow(self.x6, 2) - math.pow(self.y6, 2)) * 100
        b7 = math.pow(self.r7, 2) - math.pow(self.w1, 2) - math.pow(self.x7, 2) - math.pow(self.y7, 2)
        b8 = math.pow(self.r8, 2) - math.pow(self.w1, 2) - math.pow(self.x8, 2) - math.pow(self.y8, 2)
        b9 = math.pow(self.r9, 2) - math.pow(self.w1, 2) - math.pow(self.x9, 2) - math.pow(self.y9, 2)
        b10 = math.pow(self.r10, 2) - math.pow(self.w1, 2) - math.pow(self.x10, 2) - math.pow(self.y10, 2)
        b11 = math.pow(self.r11, 2) - math.pow(self.w1, 2) - math.pow(self.x11, 2) - math.pow(self.y11, 2)

        a11 = 2 * -self.x1
        a12 = 2 * -self.y1
        a21 = 2 * -self.x2
        a22 = 2 * -self.y2
        a31 = 2 * -self.x3
        a32 = 2 * -self.y3
        a41 = 2 * -self.x4
        a42 = 2 * -self.y4
        a51 = 2 * -self.x5
        a52 = 2 * -self.y5
        a61 = 2 * -self.x6
        a62 = 2 * -self.y6
        a71 = 2 * -self.x7
        a72 = 2 * -self.y7
        a81 = 2 * -self.x8
        a82 = 2 * -self.y8
        a91 = 2 * -self.x9
        a92 = 2 * -self.y9
        a101 = 2 * -self.x10
        a102 = 2 * -self.y10
        a111 = 2 * -self.y11
        a112 = 2 * -self.y11

        b = np.array([[b1], [b2], [b3],[b4], [b5], [b6],[b7], [b8], [b9],[b10], [b11]])
        A = np.array([[a11, a12], [a21, a22], [a31, a32], [a41,a42], [a51,a52], [a61,a62] ,[a71,a72] , [a81,a82], [a91,a92], [a101,a102], [a111,a112]])
        
        x, y = np.linalg.lstsq(A, b, rcond=None)[0]
        print("XPW:",x,"YPW:",y)
        x = 0 if x < 0 else x
        x = 400 if 400 < x else x
        y = 0 if y < 0 else y
        y = 400 if 400 < y else y
        print("xPw:",x,"yPw:",y)
        self.multw_x = x
        self.multw_y = y
        return x,y
    
    def multiration_kalman(self):
        b1 = math.pow(self.r1, 2) - math.pow(self.rk, 2) - math.pow(self.x1, 2) - math.pow(self.y1, 2) * 4
        b2 = math.pow(self.r2, 2) - math.pow(self.rk, 2) - math.pow(self.x2, 2) - math.pow(self.y2, 2)
        b3 = math.pow(self.r3, 2) - math.pow(self.rk, 2) - math.pow(self.x3, 2) - math.pow(self.y3, 2)

        a11 = 2 * -self.x1
        a12 = 2 * -self.y1
        a21 = 2 * -self.x2
        a22 = 2 * -self.y2
        a31 = 2 * -self.x3
        a32 = 2 * -self.y3

        b = np.array([[b1], [b2], [b3]])
        A = np.array([[a11, a12], [a21, a22], [a31, a32]])
        
        x, y = np.linalg.lstsq(A, b, rcond=None)[0]
        print("XX:",x,"YY:",y)
        x = 0 if x < 0 else x
        x = 400 if 400 < x else x
        y = 0 if y < 0 else y
        y = 400 if 401 < y else y
        print("x:",x,"y:",y)
        self.multk_x = x
        self.multk_y = y
        return x,y
        
class Kalman:
    def __init__(self, x_init, cov_init, meas_err, proc_err):
        self.ndim = len(x_init)
        self.A = np.eye(self.ndim)        #state transition model
        self.H = np.eye(self.ndim)        #observation model
        self.x_hat =  x_init
        self.cov = cov_init
        self.Q_k = np.eye(self.ndim)*proc_err #covariance matrix of process noise
        self.R = np.eye(len(self.H))*meas_err   #covariance matrix of observation noise
        self.x_hat_est = np.dot(self.A,self.x_hat)
        
    def update(self, obs):

        # Make prediction
        self.x_hat_est = np.dot(self.A,self.x_hat)
        self.cov_est = np.dot(self.A,np.dot(self.cov,np.transpose(self.A))) + self.Q_k

        # Update estimate
        self.error_x = obs - np.dot(self.H,self.x_hat_est)
        self.error_cov = np.dot(self.H,np.dot(self.cov_est,np.transpose(self.H))) + self.R
        self.K = np.dot(np.dot(self.cov_est,np.transpose(self.H)),np.linalg.inv(self.error_cov))
        self.x_hat = self.x_hat_est + np.dot(self.K,self.error_x)
        if self.ndim>1:
            self.cov = np.dot((np.eye(self.ndim) - np.dot(self.K,self.H)),self.cov_est)
        else:
            self.cov = (1-self.K)*self.cov_est 


def untilcom(st,dict):
    val = ""
    counter = 1
    for i in range(0,len(st)):
        if counter > 14:
            break
        if st[i] != ',' and st[i] != '\n' and st[i] != '\r':
            val += st[i]
        else:
            dict[counter].append((val))
            val = ""
            counter += 1
    return dict
            
def until_rssi(st):
    b = ""
    for i in st:
        if i != ":":
            b += i
        else:
            return int(b)
    return -1

def closest_node(rsvals):
    min = -200
    index = 100
    for i in range(0,len(rsvals)):
        if rsvals[i] > min:
            min = rsvals[i]
            index = i + 1
    return index

def convert_rssi_to_dist(rssival):
        return math.pow(2.718281,((rssival + 58) / -8))*100


def untilcom(st,dict):
    val = ""
    counter = 1
    for i in range(0,len(st)):
        if counter > 14:
            break
        if st[i] != ',' and st[i] != '\n' and st[i] != '\r':
            val += st[i]
        else:
            dict[counter].append((val))
            val = ""
            counter += 1
    return dict

def until_rssi(st):
    b = ""
    for i in st:
        if i != ":":
            b += i
        else:
            return int(b)
    return -1

# Main class for the GUI
class p2gui(object):
    def __init__(self, master):
        self._master = master
        self._master.title("CSSE4011 Prac 2")
        self._master.resizable(0, 0)
        self.offset = 30
        self.RSSI = [-20,-50,-63,-62,-51,-57,-27,-44,-57,-51,-61,-61]
        self.USonic = [1, 1]
        self.map = {
            "G": (400,400)
        }
        
        self.c = tk.Canvas(master=self._master, height=600+(2*self.offset), width=400+(2*self.offset), bg='white')
        #self.main()
        self._updater = threading.Thread(target=self.updater)
        self._updater.daemon = True # Thread is terminated when program is closed
        self._updater.start()

    # Creates the background grid
    def create_grid(self):
        
        w = 400 # Get current width of canvas
        h = 400 # Get current height of canvas
        self.c.delete('grid_line') # Will only remove the grid_line

        # Creates all vertical lines at intevals of 100
        for i in range(0+self.offset, w+self.offset, 50):
            self.c.create_line([(i, 0+self.offset), (i, h+self.offset)], tag='grid_line')

        # Creates all horizontal lines at intevals of 100
        for i in range(0+self.offset, h+self.offset, 50):
            self.c.create_line([(0+self.offset, i), (w+self.offset, i)], tag='grid_line')
        
        # Draw the border
        self.c.create_line([(0+self.offset, 0+self.offset), (0+self.offset, h+self.offset)], tag='grid_line')
        self.c.create_line([(h+self.offset, 0+self.offset), (h+self.offset, h+self.offset)], tag='grid_line')
        self.c.create_line([(0+self.offset, 0+self.offset), (0+self.offset, w+self.offset)], tag='grid_line')
        self.c.create_line([(0+self.offset, w+self.offset), (w+self.offset, w+self.offset)], tag='grid_line')

    # Draws the dot at the given coordinate location
    def draw_dot(self,x,y):
        a = int(x)+self.offset
        b = 400-int(y)+self.offset
        self.c.delete('dotty0') # Will only remove the grid_line
        self.c.create_oval((a-20),(b-20),(a+20),(b+20), activefill = "red", fill="red", width=2,tag='dotty0')
        self.c.create_text(a+3, b+3, font="Times 20 bold", fill="white", text= ":)", angle=270, tag='dotty0')

        # Draws the dot at the given coordinate location
    def draw_wdot(self,x,y):
        a = int(x)+self.offset
        b = 400-int(y)+self.offset
        self.c.delete('dotty') # Will only remove the grid_line
        self.c.create_oval((a-20),(b-20),(a+20),(b+20), activefill = "blue", fill="blue", width=2,tag='dotty')
        self.c.create_text(a+3, b+3, font="Times 20 bold", fill="white", text= ":)", angle=270, tag='dotty')


            # Draws the dot at the given coordinate location
    def draw_kdot(self,x,y):
        a = int(x)+self.offset
        b = 400-int(y)+self.offset
        self.c.delete('dotty1') # Will only remove the grid_line
        self.c.create_oval((a-20),(b-20),(a+20),(b+20), activefill = "green", fill="green", width=2,tag='dotty1')
        self.c.create_text(a+3, b+3, font="Times 20 bold", fill="white", text= ":)", angle=270, tag='dotty1')
    
    # (This can be deleted) The inputs and button for entering a coordinate
    def draw_requirements(self):
        self.c.create_text(50+self.offset, 450+self.offset, font="Times 20 bold", text= "X:")
        self.c.create_text(150+self.offset, 450+self.offset, font="Times 20 bold", text= "Y:")
        entry1 = tk.Entry(root, font="Times 20", bd=4)
        self.c.create_window(100+self.offset, 450+self.offset, width=50, window=entry1)
        entry2 = tk.Entry(root, font="Times 20", bd=4)
        self.c.create_window(200+self.offset, 450+self.offset, width=50, window=entry2)
        btn = tk.Button(root,font="Times 20 bold", text='Draw', command= lambda : self.draw_dot(entry1.get(),entry2.get()))
        btn.place(x=250+self.offset, y=425+self.offset)

    # def show_entry_fields(self):
    #     print("x: %d, y: %d\n" % (int(self.entry1.get()),int(self.entry2.get())))

    # Draws the nodes
    def draw_nodes(self):
        r = 4
        w = 5
        nodeOffset = 10
        self.c.create_oval((0-r+self.offset),(0-r+self.offset),(0+r+self.offset),(0+r+self.offset),fill="black", width=w)
        self.c.create_text((0-nodeOffset+self.offset),(0-nodeOffset+self.offset), text= "3")
        self.c.create_oval((400-r+self.offset),(0-r+self.offset),(400+r+self.offset),(0+r+self.offset),fill="black", width=w)
        self.c.create_text((400-nodeOffset+self.offset),(0-nodeOffset+self.offset), text= "2")
        self.c.create_oval((400-r+self.offset),(400-r+self.offset),(400+r+self.offset),(400+r+self.offset),fill="black", width=w)
        self.c.create_text((400-nodeOffset+self.offset),(400-nodeOffset+self.offset), text= "4")
        self.c.create_oval((0-r+self.offset),(400-r+self.offset),(0+r+self.offset),(400+r+self.offset),fill="black", width=w)
        self.c.create_text((0-nodeOffset+self.offset),(400-nodeOffset+self.offset), text= "1")

    # draws the cute little axis of the graph
    def draw_axis(self):
        w = 400 # Get current width of canvas
        h = 400 # Get current height of canvas
        axisOffset = 20
        self.c.create_line([(0+self.offset-axisOffset, h+self.offset+axisOffset), (50+self.offset-axisOffset, h+self.offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
        self.c.create_line([(0+self.offset-axisOffset, h+self.offset+axisOffset), (0+self.offset-axisOffset, h-50+self.offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
        self.c.create_text(0+self.offset-axisOffset, h-50+self.offset+axisOffset-5, text= "Y")
        self.c.create_text(50+self.offset-axisOffset+5, h+self.offset+axisOffset, text= "X")

    # Displays the RSSI and Ultra Sonic values given in the array in the init function
    # Will require a function to update these text boxes when the values are update/changed
    def display_measurements(self):
        p = 50
        j = 50
        k = 50

        self.c.delete('rssi_values')
        for i, x in enumerate(self.RSSI):
            if i < 6:
                self.c.create_text(50+self.offset, p+450+self.offset, font="Times 12 bold", text= ("RSSI " + str(i+1) + ": " + str(x)),anchor=tk.W, tag='rssi_values')
                p = p + 20
            else :
                self.c.create_text(150+self.offset, j+450+self.offset, font="Times 12 bold", text= ("RSSI " + str(i+1) + ": " + str(x)),anchor=tk.W, tag='rssi_values')
                j = j + 20

        for y in range(0,len(self.USonic)):
            self.c.create_text(250+self.offset, k+450+self.offset, font="Times 12 bold", text= ("Ultra Sonic " + str(y+1) + ": " + str(self.USonic[y])),anchor=tk.W, tag='rssi_values')
            k = k + 20
    

    def updater(self):
        self.draw_requirements()
        self.draw_nodes()
        self.draw_axis()
        self.display_measurements()
        self.create_grid()
        self.c.pack(fill=tk.BOTH, expand=True)
        ser = serial.Serial("/dev/serial/by-id/usb-Fardeen_DONGLE_FADS_-_Zephyr_E9D853851B535197-if00", baudrate=115200, timeout=3.0)
        counter  = 0
        ind = 1
        placehold = 0
        prevCounter = 0
        d = {1:[],2:[],3:[],4:[],5:[],6:[],7:[],8:[],9:[],10:[],11:[],12:[],13:[],14:[]}
        vals = {1:[],2:[],3:[],4:[],5:[],6:[],7:[],8:[],9:[],10:[],11:[],12:[],13:[],14:[]}
        m = Multi()
        ndim = 2
        x_init = np.array([200, 200])
        kalman = Kalman(x_init, np.ones((ndim)), 0.15, 0.001)
        while(1):
            self.RSSI = []
            self.USonic = []
            if counter > prevCounter + 2:
                rsVals = []
                rssi1 = statistics.mean(vals[1][placehold:placehold+2])
                rssi2 = statistics.mean(vals[2][placehold:placehold+2])
                rssi3 = statistics.mean(vals[3][placehold:placehold+2])
                rssi4 = statistics.mean(vals[4][placehold:placehold+2])
                rssi5 = statistics.mean(vals[5][placehold:placehold+2])
                rssi6 = statistics.mean(vals[6][placehold:placehold+2])
                rssi7 = statistics.mean(vals[7][placehold:placehold+2])
                rssi8 = statistics.mean(vals[8][placehold:placehold+2])
                rssi9 = statistics.mean(vals[9][placehold:placehold+2])
                rssi10 = statistics.mean(vals[10][placehold:placehold+2])
                rssi11 = statistics.mean(vals[11][placehold:placehold+2])
                rssi12 = statistics.mean(vals[12][placehold:placehold+2])
                m.rk = convert_rssi_to_dist(rssi1)
                m.r2 = convert_rssi_to_dist(rssi2)
                m.r3 = convert_rssi_to_dist(rssi3)
                m.r4 = convert_rssi_to_dist(rssi4)
                m.r5 = convert_rssi_to_dist(rssi5)
                m.r6 = convert_rssi_to_dist(rssi6)
                m.r7 = convert_rssi_to_dist(rssi7)
                m.r8 = convert_rssi_to_dist(rssi8)
                m.r9 = convert_rssi_to_dist(rssi9)
                m.r10 = convert_rssi_to_dist(rssi10)
                m.r11 = convert_rssi_to_dist(rssi11)
                m.w1 = vals[13][placehold]
                m.w2 = vals[14][placehold]
                print("CL:",close)
                if close != 4:
                    close = close - 1
                x1,y1 = m.cl_multiration(close)
                x2,y2 = m.weigthed_multi()
                self.draw_kdot(m.multk_x, m.multk_y)
                self.draw_dot(x1,y1)
               # self.draw_wdot(x2,y2)
                obs = np.array([m.multk_x,m.multk_y])
                kalman.update(obs[:])
                placehold = placehold + 2
                prevCounter = counter
                print(kalman.x_hat_est[0],kalman.x_hat_est[1],"jer")

            serialString = ser.readline()
            # Print the contents of the serial data
            sw = serialString.decode('Ascii')
            d = untilcom(sw,d)
            for i in range(1,15):
                vals[i].append(until_rssi(d[i][counter]))
                if i < 13:
                    self.RSSI.append(int(vals[i][counter]))
                else: 
                    self.USonic.append(int(vals[i][counter]))
            close = closest_node(self.RSSI)
            counter += 1
            ind = 1
            j = json.dumps(d,indent = 2)
            with open('data.json','w') as f:
                f.write(j)
            self.display_measurements()

            self.c.pack(fill=tk.BOTH, expand=True)
        
if __name__ == "__main__":
    root = tk.Tk()
    a = p2gui(root)
    root.mainloop()