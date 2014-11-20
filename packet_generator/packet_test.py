#-*-coding:utf-8-*-
#network clinet parse
import struct, sqlite3, network, sys, threading, time

reload(sys) 
sys.setdefaultencoding('utf-8')

# dict type
parse_type = {
'char' : ['B', 1],
'uchar' : ['b', 1],
'short' : ['h', 2],
'int16' : ['h', 2],
'ushort' : ['H', 2],
'uint16' : ['H', 2],
'int' : ['i', 4],
'int32' : ['i', 4],
'uint' : ['I', 4],
'uint32' : ['I', 4],
'int64' : ['q', 8],
'uint64' : ['Q', 8],
'string' : ['s', 8]
}

endian = '>'

def GetSendData(dbpath, protocol_unique_no ) :
    print(dbpath)
    conn = sqlite3.connect(dbpath)
    cur = conn.cursor()

    query = 'select sendvalue, header_no, transfer_no from Protocol where protocol_no = ' + str(protocol_unique_no)
    print(query)
    cur.execute(query)

    fetchlist = cur.fetchone()
    send_value = str(fetchlist[0])
    send_value_list = send_value.split(',')

    header_no = str(fetchlist[1])
    transfer_no = str(fetchlist[2])

    cur.execute('select * from header where unique_no = ' + header_no)

    buffer = (b'');
    index = 0   
    for row in cur :
        pack_type = endian + parse_type[row[2]][0]
        print(pack_type)
        buffer += struct.pack(pack_type, int(send_value_list[index]));
        index = index + 1

    cur.execute('select * from send where unique_no = ' + str(transfer_no))
    for row in cur :
        pack_type = endian + parse_type[row[2]][0]

        print(pack_type)
        if(pack_type == endian + 's') :
            size = len(send_value_list[index])
            pack_type = pack_type[0] + str(size) + pack_type[1]
            buffer += struct.pack(pack_type, send_value_list[index].encode());
        else :
            buffer += struct.pack(pack_type, int(send_value_list[index]));
        index = index + 1
    cur.close()

    buffer += struct.pack(endian + 'I', 51409)

    return buffer

def GetSend(dbpath, protocol_unique_no) :
    conn = sqlite3.connect(dbpath)
    cur = conn.cursor()

    cur.execute('select sendvalue from Protocol where protocol_no = ' + str(protocol_unique_no))

    send_value = cur.fetchone()[0]
    send_value_list = send_value.split(',')

    cur.execute('select * from send where unique_no = (select transfer_no from Protocol);')

    buffer = (b'');
    index = len(send_value_list) - 1
    for row in cur :
        pack_type = endian + parse_type[row[2]][0]

        print(pack_type)
        if(pack_type == endian + 's') :
            size = len(send_value_list[index])
            pack_type = pack_type[0] + str(size) + pack_type[1]
            buffer += struct.pack(pack_type, send_value_list[index]);
        else :
            buffer += struct.pack(pack_type, int(send_value_list[index]));
        index = index - 1

    cur.close()
    return buffer

def TaskNetwork(host, port, timeout, sendbuffer) :
    s = network.connect(host, port, timeout)
    network.send(s, sendbuffer)
    if uselog == 1 :
        print(network.recv(s))
    else :
        data = network.recv(s)
    s.close();

def TransferProcess(host, port, count, timeout, uselog, protocol_unique) :
    sendbuffer = (b'');    
    sendbuffer += GetSendData(sqlitepath, protocol_unique)

    threads = []
    
    for number in range(0, count) :
        t = threading.Thread(target=TaskNetwork, args=(host, port, timeout, sendbuffer))
        t.start()
        threads.append(t);

    for th in threads :
        th.join()        

## this is entry point
if (len(sys.argv) >= 8) :
    sqlitepath = sys.argv[1]
    host = sys.argv[2]
    port = sys.argv[3]
    count = sys.argv[4]
    timeout = sys.argv[5]
    uselog = sys.argv[6]
    protocol_unique = sys.argv[7]

    TransferProcess(host, int(port), int(count), int(timeout), int(uselog), int(protocol_unique))
    
else:
    sqlitepath = './packet_define_db/packet_define.sqlite'
    sendbuffer = (b'');
    sendbuffer += GetSendData(sqlitepath, 101)

    s = network.connect('127.0.0.1', 10030, 1)
    network.send(s, sendbuffer)
    print(network.recv(s, 1024))
    s.close();