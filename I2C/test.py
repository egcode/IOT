#from smbus import SMBus
#
#addr = 0x8 # bus address
#bus = SMBus(1) # indicates /dev/ic2-1
#######################################
# from smbus import SMBus

# addr = 0x8 # bus address
# bus = SMBus(1) # indicates /dev/ic2-1

# numb = 1

# print("Enter 1 for ON or 0 for OFF")
# while numb == 1:

# 	ledstate = input(">>>>   ")

# 	if ledstate == "1":
# 		bus.write_byte(addr, 0x1) # switch it on
# 	elif ledstate == "0":
# 		bus.write_byte(addr, 0x0) # switch it on
# 	else:
# 		numb = 0
#######################################



# import smbus
# import time

# bus = smbus.SMBus(1)
# address = 0x8 # bus address

# def writeNumber(a,b):
#     bus.write_i2c_block_data(address, a, [b])
#     return -1

# try:
#     while True:
#         # Take input from the user
#         number1 = int(input("Enter a number1: "))
#         number2 = int(input("Enter a number2: "))

#         writeNumber(number1,number2)
#         time.sleep(1)                    #delay one second

# except KeyboardInterrupt:
#     print("KeyboardInterrupt, exiting.")
# finally:
#     bus.close()



import smbus
import time

bus = smbus.SMBus(1)
address = 0x8 # bus address

# num1 = int(23492)

def bitSplitIntegerAs16Bit(num):
    print("num as int: "+ str(num))
    print("num:        " + format(num, '0{}b'.format(16))) # 0101101111000100
    # mask = (1 << 16) - 1 # int_16 max 65535 makes first 8 as ones  1111111111111111

    num_part1 = ((1 << 8) - 1) & num
    print("num_part1:  " + format(num_part1, '0{}b'.format(16))) # 0000000011000100
    print("num_part1 as 8 bit:  " + str(num_part1))

    num_part2 = ((1 << 8) - 1) & (num >> 8)
    print("num_part2:  " + format(num_part2, '0{}b'.format(16))) # 0000000001011011
    print("num_part2 as 8 bit:  " + str(num_part2))

    return num_part1, num_part2


def bitSplitIntegerAs32Bit(num):
    print("num as int: "+ str(num))
    print("num:        " + format(num, '0{}b'.format(32))) # 01101101111000000110100100000100
    # mask = (1 << 32) - 1 # int_32 max 4294967295  1111111111111111111111111111111

    num_part1 = ((1 << 8) - 1) & num
    print("num_part1:  " + format(num_part1, '0{}b'.format(32))) # 0000000011000100
    print("num_part1 as 8 bit:  " + str(num_part1))

    num_part2 = ((1 << 8) - 1) & (num >> 8)
    print("num_part2:  " + format(num_part2, '0{}b'.format(32))) # 0000000001011011
    print("num_part2 as 8 bit:  " + str(num_part2))

    num_part3 = ((1 << 8) - 1) & (num >> 16)
    print("num_part3:  " + format(num_part3, '0{}b'.format(32))) # 0000000001011011
    print("num_part3 as 8 bit:  " + str(num_part3))

    num_part4 = ((1 << 8) - 1) & (num >> 24)
    print("num_part4:  " + format(num_part4, '0{}b'.format(32))) # 0000000001011011
    print("num_part4 as 8 bit:  " + str(num_part4))


    return num_part1, num_part2, num_part3, num_part4


try:
    while True:
        # Take input from the user
        num1 = int(input("Enter a number1: "))
        num2 = int(input("Enter a number2: "))

        print("original num1:  " + str(num1))
        print("original num2:  " + str(num2))

        # max16BitInt = (1 << 16) - 1 # int_16 max 65535 makes first 8 as ones  1111111111111111
        # num1 = max(0, min(max16BitInt, num1))
        # num2 = max(0, min(max16BitInt, num2))

        max32BitInt = (1 << 32) - 1 # int_16 max 65535 makes first 8 as ones  1111111111111111111111111111111
        num1 = max(0, min(max32BitInt, num1))
        num2 = max(0, min(max32BitInt, num2))

        num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
        num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)

        bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])

        time.sleep(1)

except KeyboardInterrupt:
    print("KeyboardInterrupt, exiting.")
finally:
    bus.close()

# num_part1, num_part2 = bitSplitIntegerAs16Bit(num1)
# bus.write_i2c_block_data(address, num_part1, [num_part2])
# time.sleep(1)                 


# num16BitTest = int(23492)
# num_part1, num_part2 = bitSplitIntegerAs16Bit(num16BitTest)


# num32BitTest = int(1843423492)
# num_part1, num_part2, num_part3, num_part4 = bitSplitIntegerAs32Bit(num32BitTest)
