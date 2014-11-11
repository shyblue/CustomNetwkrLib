
-- [TransferInfo] Query ¸ðÀ½
-- select * from TransferInfo
-- insert into TransferInfo (unique_no, description) values(2, '¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ ÀûÀçÇÑ´Ù.');
-- insert into TransferInfo (unique_no, description) values(3, '¼±¼öÀÇ »ó¼¼Á¤º¸¸¦ Àü´ÞÇÑ´Ù.');
-- insert into TransferInfo (unique_no, description) values(4, '¾Ö´Ï¸ÞÀÌ¼Ç Á¤º¸¸¦ Àü´ÞÇÑ´Ù.');

-- select * from Header order by unique_no, order_no

-- [Send] Query ¸ðÀ½

-- select * from Send where unique_no = 1 order by order_no;
-- Update Send Set name = 'leaguetime' where unique_no = 1 and order_no = 1;
-- insert into Send(unique_no, order_no, type, name) values(1, 2, 'uint32','pitcher_path_length');
-- insert into Send(unique_no, order_no, type, name) values(1, 3, 'string','pitcher_path');
-- insert into Send(unique_no, order_no, type, name) values(1, 4, 'uint32','fielder_path_length');
-- insert into Send(unique_no, order_no, type, name) values(1, 5, 'string','fielder_path');

-- insert into Send(unique_no, order_no, type, name) values(2, 1, 'uint64','leaguetime');
-- insert into Send(unique_no, order_no, type, name) values(2, 2, 'uint32','animation_path_length');
-- insert into Send(unique_no, order_no, type, name) values(2, 3, 'string','animation_path');

-- delete FROM Send where unique_no  = 3;
-- insert into Send(unique_no, order_no, type, name) values(3, 1, 'uint64','user_uid');
-- insert into Send(unique_no, order_no, type, name) values(3, 2, 'uchar', 'player_type');

-- insert into Send(unique_no, order_no, type, name) values(4, 1, 'uint64','user_uid');
-- insert into Send(unique_no, order_no, type, name) values(4, 2, 'uint64','leaguetime');
-- insert into Send(unique_no, order_no, type, name) values(4, 3, 'uint32','league_no');

-- [Recv] Query ¸ðÀ½

-- select * from recv
-- update recv set type = 'uint16', name = 'result_value'
-- insert into recv(unique_no, order_no, type, name) values(2, 1, 'uint16', 'result_value');

-- delete from Recv where unique_no  = 3; 
-- insert into Recv(unique_no, order_no, type, name) values(3, 1, 'uint16', 'result_value');
-- insert into Recv(unique_no, order_no, type, name) values(3, 2, 'uint32', 'detailinfo_length');
-- insert into Recv(unique_no, order_no, type, name) values(3, 3, 'char[]', 'detailinfo');


-- insert into recv(unique_no, order_no, type, name) values(4, 1, 'uint16', 'result_value');
-- insert into recv(unique_no, order_no, type, name) values(4, 2, 'uint32', 'animation_info_length');
-- insert into recv(unique_no, order_no, type, name) values(4, 3, 'char[]', 'animation_info');

-- update Protocol set sendvalue = '155,101,0,65535,1,0,0,0,59,E:/svnclient/Wserver/trunk/proj_vc11/record/PitcherInfo.etz,59,E:/svnclient/Wserver/trunk/proj_vc11/record/FielderInfo.etz' where protocol_no = 101;
-- update Protocol set sendvalue = '99,102,0,65535,1,0,0,0,0,62,E:/svnclient/Wserver/trunk/proj_vc11/record/animation_info.etz' where protocol_no = 102;
-- update Protocol set sendvalue = '177,101,0,65535,1,0,0,0,70,C:/Users/Administrator/Desktop/Simulation_Cache/record/PitcherInfo.etz,70,C:/Users/Administrator/Desktop/Simulation_Cache/record/FielderInfo.etz' where protocol_no = 101;
-- update Protocol set sendvalue = '110,102,0,65535,1,0,0,0,0,73,C:/Users/Administrator/Desktop/Simulation_Cache/record/animation_info.etz' where protocol_no = 102;
-- update Protocol set sendvalue = '121,101,0,65535,1,0,0,0,42,E:/PBBM_DAUM/result/record/PitcherInfo.etz,42,E:/PBBM_DAUM/result/record/FielderInfo.etz' where protocol_no = 101;
-- update Protocol set sendvalue = '105,102,0,65535,1,0,0,0,0,68,E:/PBBM_DAUM/result/animation_new/second_division/animation_info.etz' where protocol_no = 102;

-- update Protocol set sendvalue = '121,101,0,65535,1,0,0,0,42,/record_HOMERUN/2013101610/PitcherInfo.etz,42,/record_HOMERUN/2013101610/FielderInfo.etz' where protocol_no = 101;
-- update Protocol set sendvalue = '101,102,0,65535,1,0,0,0,0,64,/animation_HOMERUN/second_division/2013101609/animation_info.etz' where protocol_no = 102;
-- update Protocol set sendvalue = '173,101,0,65535,1,0,0,0,68,/home/entaz/simulator_cache/result/record/2013083012/PitcherInfo.etz,68,/home/entaz/simulator_cache/result/record/2013083012/FielderInfo.etz' where protocol_no = 101;
-- update Protocol set sendvalue = '120,102,0,65535,1,0,0,0,0,83,/home/entaz/simulator_cache/result/animation/2013083012/beginner/animation_info.etz' where protocol_no = 102;

-- P = 80 F = 70
-- update Protocol set sendvalue = '30,103,0,65535,1,0,0,201301110000000025,80' where protocol_no = 103;
-- update Protocol set sendvalue = '30,103,0,65535,1,0,0,200000000000228999,70' where protocol_no = 103;
-- update Protocol set sendvalue = '41,104,0,65535,1,0,0,200000000000140773,0,0' where protocol_no = 104;

--update Protocol set sendvalue = '29,103,0,65535,1,0,0,201209080000006097' where protocol_no = 103;
--update Protocol set sendvalue = '41,104,0,65535,1,0,0,201210050000000466,0,0' where protocol_no = 104;
-- SELECT * FROM Protocol


------------- make 1000 StatusInfomation

-- INSERT INTO PROTOCOL (protocol_no, classname, header_no, transfer_no, sendvalue, description) values(1000, 'StatusInfomation', 1, 1000, '25, 900,0,65535,1,0,0,1', '¿¿¿ ¿¿¿¿¿ ¿¿¿¿¿.');

-- INSERT INTO SEND (unique_no, order_no, type, name) values(1000, 1, 'int32', 'infomation_no');

-- INSERT INTO RECV (unique_no, order_no, type, name) values(1000, 1, 'uint16', 'result_value');
-- INSERT INTO RECV (unique_no, order_no, type, name) values(1000, 2, 'uint32', 'use_memory')

update  Protocol set sendvalue = '25,1000,0,65535,1,0,0,1' where transfer_no = 1000;

------------- make 1000 StatusInformation 















