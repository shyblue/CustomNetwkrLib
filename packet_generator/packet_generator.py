# -*- coding:utf-8 -*-
import os, sys, sqlite3, clr

import System.Drawing
import System.Windows.Forms

#clr.AddReferenceByPartialName("System.Windows.Forms")
#clr.AddReferenceByPartialName("System.Drawing")
from System.Windows.Forms import *
from System.Drawing import *

# 사용할 인코딩 타입을 설정합니다
reload(sys)
sys.setdefaultencoding('utf-8')

type_dictionary ={
'char' : 'int8_t'
,'uchar' : 'uint8_t'
,'int8_t' : 'int8_t'
,'uint8_t' : 'uint8_t'
,'short' : 'int16_t'
,'int16' : 'int16_t'
,'ushort' : 'uint16_t'
,'uint16' : 'uint16_t'
,'int' : 'int32_t'
,'int32' : 'int32_t'
,'uint' : 'uint32_t'
,'uint32' : 'uint32_t'
,'int64' : 'int64_t'
,'uint64' : 'uint64_t'
,'string' : 'std::string'
,'char[]' : 'char*'
}

print_dictionary ={
'char' : '%c'
,'uchar' : '%c'
,'int8_t' : '%c'
,'uint8_t' : '%c'
,'int16_t' : '%d'
,'uint16_t' : '%d'
,'int32_t' : '%d'
,'uint32_t' : '%d'
,'int64_t' : '%lld'
,'uint64_t' : '%lld'
,'std::string' : '%s'
,'char*' : '%s'
}

#특정 자료형을 치환한다.
def type_replacement(def_type) :
    return type_dictionary[def_type]

def print_replacement(def_type) :
    return print_dictionary[def_type]

def get_index_list(arg, index, identify) :
    arg_list=arg.split(identify)
    return arg_list[index]

def get_arg_idx_list(args, index, identify) :
    result = []
    for arg in args :
        result.append(get_index_list(arg, index, identify))

    return result

def make_print_string(args) :
    result = ""
    for arg in args :
        arg_list = arg.split(' ')
        result = result + arg_list[1] + '[' +print_replacement(arg_list[0]) + '], '
    

    return result[:-2]


# type arg ex) uint64_t league_time
def make_arg_string(args) :
    result = ''
    for arg in args :
        result += arg
        result += ', '

    return result[:-2]

# type& arg ex) uint64_t& league)time
def make_arg_ref_string(args) :
    result = ''
    for arg in args :
        arg = arg.replace(' ', '& ')
        result += arg
        result += ', '

    return result[:-2]


# type arg; ex) uint64_t request_league_time; \r\n
def make_arg_declaration(args, front) :
    result = ''
    for arg in args :
        arg = arg.replace(' ', ' '+front)
        arg += ';'
        result += arg
        result += '\n\t'

    return result


def make_arg_enum(args, front) :
    result = ''
    for arg in args :
        arg_list = arg.split(' ')
        arg = arg_list[0] + ' ' + front + arg_list[1]
        result += arg
        result += ', '

    return result[:-2]


# arg, ex) league_time,
def make_ar_2nd_enum(args, front) :
    result = ''
    for arg in args :
        arg_list = arg.split(' ')
        arg = front + arg_list[1]            
        result += arg
        result += ', '

    return result[:-2]


# arg, ex) league_time, string.c_str()
def make_arg_2nd_val_enum(args, front) :
    result = ''
    for arg in args :
        arg_list = arg.split(' ')
        if arg_list[0] == 'std::string' :
            arg = front + arg_list[1] + '.c_str()'
        else :    
            arg = front + arg_list[1]
        result += arg
        result += ', '

    return result[:-2]


def make_arg_get_size(args, front) :
    result = ''
    for arg in args :
        arg_list = arg.split(' ')
        if arg_list[0] == 'std::string' or arg_list[0] == 'char*' :
            arg = front + arg_list[1] + '_length'
        else :
            arg = 'sizeof(' + front + arg_list[1] + ')'
        result += arg
        result += ' + '

    return result[:-2]


def write_header_file(class_name, send_args, recv_args) :
    filename =  output_path + '%s.h' % class_name
    write_file = open(filename, 'w')

    write_data = make_header_string(class_name, send_args, recv_args)
    
    write_file.write(write_data)
    write_file.close()

def write_cpp_file(class_name, send_args, recv_args) :
    file_name =  output_path + '%s.cpp' % class_name
    write_file = open(file_name, 'w')

    write_data = make_cpp_string(class_name, send_args, recv_args)
    
    write_file.write(write_data)
    write_file.close()

def write_define(class_name) :
    write_file = open(define_file_name, 'a')
    write_file.write('#include \"' + class_name + '.h\"\n')
    write_file.close()
    


def make_header_string(class_name, send_args, recv_args) :

    result = '''
#pragma once

#ifndef __{0:s}_H_
#define __{0:s}_H_

#include <boost/tuple/tuple.hpp>

class {1:s} {{
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, {2:s});
    static size_t Deserialize(char* buffer, size_t buffer_size, {3:s});
}};

#endif
'''.format(class_name.upper(), class_name, make_arg_ref_string(recv_args),make_arg_ref_string(send_args))
    
    return result

def make_cpp_string(class_name, send_args, recv_args) :
    result = '''
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "{0:s}.h"

{1:s}

{2:s}

{3:s}

{4:s}

    '''.format(class_name, make_cpp_protocol_process(class_name, send_args, recv_args),
               make_cpp_error_result(class_name),
               make_cpp_serialize(class_name, recv_args), make_cpp_unserialize(class_name, send_args))

    return result


def make_cpp_protocol_process(class_name, send_args, recv_args):
    result = '''
{0:s}::result_object {0:s}::ProtocolProcess(const char * buffer, const size_t size)
{{

    // 클라이언트로부터 전달받은 데이터
    {1:s}

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Deserialize((char*)buffer, size, {2:s});

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != {3:s})
    {{
        LOG_ERROR("[{0:s}] buffer Size Error");
        return ErrorResult(result_value);
    }}

    // 클라이언트에게 전송할 데이터
    {4:s}
    // ----------------- [데이터 처리 시작] -----------------
    // TODO: Packet Handler body 작성

    // ----------------- [데이터 처리 완료] -----------------

    // 프로토콜이 정상적으로 처리 안됬을경우
    if(result_value != 1)
    {{
        return ErrorResult(result_value);
    }}

    response_result_value = static_cast<uint16_t>( result_value );

    // 클라이언트에게 전송할 데이터를 처리 합니다.
    size_t response_size = {5:s};
    char * response_buffer = COMMON_POOL::_new(response_size);
    size_t response_idx = 0;
    response_index = Serialize(response_buffer, response_size, {6:s});

    ST_LOGGER.Info("[{0:s}] Complete response_index[{7:s}]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}}
    '''.format(class_name, make_arg_declaration(send_args, 'request_'),make_ar_2nd_enum(send_args, 'request_'),
               make_arg_get_size(send_args, 'request_'), make_arg_declaration(recv_args, 'response_'),
               make_arg_get_size(recv_args, 'response_'), make_ar_2nd_enum(recv_args, 'response_'),'%d')

    return result

# 프로토콜이 실패시 리턴 함수 제네레이터
def make_cpp_error_result(class_name) :
    result = '''
{0:s}::result_object {0:s}::ErrorResult(int16_t& result_value)
{{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[{0:s}][OutputData][ErrorResult] result_value[{1:s}]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}}
    '''.format(class_name, '%d')

    return result

# 클라이언트에게 전송할 데이터를 직렬화 시킨다.
def make_cpp_serialize(class_name, recv_args) :

    write_str = ''
    for arg in recv_args :
        _type = get_index_list(arg, 0, ' ')
        name = get_index_list(arg, 1, ' ')
        if _type == 'std::string' or _type == 'char*':
            name_len = name +'_length'
            if _type == 'std::string':
                write_str += name + '.resize('+name_len+');\n\t'
                write_str += "MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &" + name + '[0], ' + name_len + ');\n\t'
            elif _type == 'char*':
                write_str += "MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, " + name + ', BYTE_ORDER::network_to_host(' + name_len + '));\n\t'
        else :
            write_str += name + ' = BYTE_ORDER::host_to_network(' + name + ');\n\t'
            write_str += 'MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &' + name + ');\n\t'
    
    result = '''
size_t {0:s}::Serialize(char* buffer, size_t buffer_size, {1:s})
{{
    ST_LOGGER.Info("[{0:s}][OutputData] {2:s}", {3:s});

    size_t buffer_idx = 0;

    {4:s}

    return buffer_idx;
}}
    '''.format(class_name, make_arg_ref_string(recv_args),make_print_string(recv_args),
               make_arg_2nd_val_enum(recv_args, ''), write_str)

    return result

# 클라이언트에게 전송받은 직렬화된 데이터를 데이터화 시킨다.
def make_cpp_unserialize(class_name, send_args) :

    make_print_string(send_args)
    read_str = ''
    for arg in send_args :
        _type = get_index_list(arg, 0, ' ')
        name = get_index_list(arg, 1, ' ')
        if _type == 'std::string' or _type == 'char*':
            name_len = name +'_length'
            if _type == 'std::string':
                read_str += name + '.resize('+name_len+');\n\t'
                read_str += "MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &" + name + '[0], ' + name_len + ');\n\t'
            elif _type == 'char*':
                read_str += "MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, " + name + ', ' + name_len + ');\n\t'
        else :
            if _type == 'uint8_t' or _type == 'int8_t':
                read_str += 'MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &' + name + ');\n\t'
            else :
                read_str += 'MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &' + name + ');\n\t'
                read_str += name + ' = BYTE_ORDER::network_to_host(' + name + ');\n\t'
    
    result = '''
size_t {0:s}::Deserialize(char* buffer, size_t buffer_size, {1:s})
{{
    size_t buffer_idx = 0;

    {2:s}

    ST_LOGGER.Info("[{0:s}][InputData] {3:s}", {4:s});
    return buffer_idx;
}}
    '''.format(class_name,make_arg_ref_string(send_args),
               read_str,make_print_string(send_args),make_arg_2nd_val_enum(send_args, ''))

    return result

# Function : 입력받은 파일정보파일을 읽고 해당하는 함수를 실행시킨다.
def write_generator(db_file) :
    conn = sqlite3.connect(db_file)
    cur = conn.cursor()

    #프로토콜 테이블에서 필요한 정보를 얻어온다.
    cur.execute("select protocol_no, classname, transfer_no from Protocol")
    for row in cur :
        # protocol_no     = row[0]
        class_name       = row[1]
        transfer_no     = row[2]

        #Send 테이블에서 데이터 구조정보를 얻어온다.        
        send_cursor = conn.cursor()
        send_cursor.execute("select type, name from Send where unique_no = %d order by order_no" % transfer_no)

        send_arg = []

        for send_row in send_cursor :
            send_arg.append(type_replacement(send_row[0]) + ' ' + send_row[1])

        #Recv 테이블에서 데이터 구조정보를 얻어온다.
        recv_cursor = conn.cursor()
        recv_cursor.execute("select type, name from Recv where unique_no = %d order by order_no" % transfer_no)

        recv_arg = []
        
        for recv_row in recv_cursor :
            recv_arg.append(type_replacement(recv_row[0]) + ' ' + recv_row[1])

        write_header_file(class_name, send_arg, recv_arg)
        write_cpp_file(class_name, send_arg, recv_arg)
        write_define(class_name)

        print('make [%s]'% class_name)

class HelloWorldForm(Form):

    def __init__(self):
        self.Text = 'Hello World'
        self.Name = 'Hello World'

form = HelloWorldForm()
Application.Run(form)
# Start packet generator
if __name__ == "__main__" :
    
## Setup output folder
    output_path = '../protocol/'
    if not os.path.isdir(output_path):
        os.mkdir(output_path)

## generator header file that have include all of protocols
    define_file_name = output_path + 'protocol_define.h'
    f = open(define_file_name, 'w')
    f.close()

## generator protocol sources and headers
if len(sys.argv) < 2:
    write_generator('./packet_define_db/packet_define.sqlite')
else :
    print('sqlite path = [%s]'% sys.argv[1])
    write_generator(sys.argv[1])

    print("Press any key to terminate.")
    input()