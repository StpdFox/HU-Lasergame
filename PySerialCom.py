import serial_asyncio
import asyncio
import os

global new_data
import binascii
import serial

class ParseMsg():
    def parse_data_struct(self, msg):
        splitted_str = msg.split("&")
        print(splitted_str)

    def write_data_struct_to_file(self, msg, file_name="test_file.txt"):
        splitted_str = msg.split("&")
        player_id = int(splitted_str[0].split(":")[1])
        file_object = open("player_" + splitted_str[0].split(":")[1] + ".stats", "w")

        for i in splitted_str:
            file_object.write(i + "\n")
        file_object.flush()
        file_object.close()

class Output(asyncio.Protocol):
    msg = bytes();
    header_validated = False
    begin_node = "000"
    msg_size = 0

    def connection_made(self, transport):
        self.transport = transport
        print('port opened', transport)
        transport.serial.rts = False
        transport.write(b'r')
        # time.sleep(5)

    def validate_msg(self, data):
        self.msg = self.msg + data
        if (self.header_validated == False and len(self.msg) >= 6):
            self.validate_header()
        elif self.header_validated == True and len(self.msg) >= 6 + self.msg_size:
            if self.validate_end():
                print("Valid end of msg!")
                print(self.msg[6: self.msg_size + 6].decode('ascii'))
                parsed = ParseMsg()
                parsed.write_data_struct_to_file(self.msg[6: self.msg_size + 6].decode('ascii'), "p1.txt")
            else:
                print("error: msg ending invalid!")
        else:
            print(len(self.msg))
            print(self.msg_size)

    def validate_header(self):
        if self.validate_data_type() == True:
            self.msg_size = 256 * self.msg[2] + self.msg[3]
            self.header_validated = True
        else:
            self.header_validated = False
            print(self.msg)
            print("error: data type invalid!")

    def validate_end(self):
        if self.msg[len(self.msg) - 3: len(self.msg)] == b'END':
            return True
        return False

    def validate_data_type(self):
        chopped = self.msg[0:2]
        cut_str = binascii.b2a_uu(self.msg[0:2])
        print("printed values")
        print(self.msg[0:2])
        if (self.msg[0:2] == b'PD'):
            return True
        else:
            return False

    def data_received(self, data):
        print(repr(data))
        self.validate_msg(data)

    def connection_lost(self, exc):
        print('port closed')
        asyncio.get_event_loop().stop()


# Todo
# If wanting to read -> send 'r'
# gets results as xml
# store xml results with name as player id
# file_object  = open("game_results", "w")
# file.write()

if os.name == 'nt':
    loop = asyncio.get_event_loop()
    coro = serial_asyncio.create_serial_connection(loop, Output, 'COM3', baudrate=38400, parity=serial.PARITY_NONE,
                                                   stopbits=serial.STOPBITS_ONE,
                                                   bytesize=serial.EIGHTBITS,
                                                   timeout=2,
                                                   dsrdtr=True)
    loop.run_until_complete(coro)
    loop.run_forever()

elif os.name == 'posix':
    loop = asyncio.get_event_loop()
    coro = serial_asyncio.create_serial_connection(loop, Output, '/dev/ttyUSB0', baudrate=38400, parity=serial.PARITY_NONE,
                                                   stopbits=serial.STOPBITS_ONE,
                                                   bytesize=serial.EIGHTBITS,
                                                   timeout=2,
                                                   dsrdtr=True)
    loop.run_until_complete(coro)
    loop.run_forever()
elif os.name == 'java':
    print("os type java not supported!")
else:
    print("operating system not recognized!")
