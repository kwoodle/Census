#"year","state","state_po","state_fips","state_cen","state_ic","office","candidate","party_detailed","writein","candidatevotes","totalvotes","version","notes","party_simplified"
#1976,"ALABAMA","AL",1,63,41,"US PRESIDENT","CARTER, JIMMY","DEMOCRAT",FALSE,659170,1182850,"20210113",NA,"DEMOCRAT"
use census;
drop table if exists presidential_hist;
CREATE TABLE if not exists `presidential_hist` (
    `year` year NOT NULL `date_format`='YYYY',
    `state` varchar(50) NOT NULL,
    `state_po` varchar(4) NOT NULL,
    `candidate` varchar(100) flag=8,
    `party_detail` varchar(50),
    `writein` varchar(20),
    `votes` int flag=11,
    `total_votes` int flag=12,
    `party` varchar(50) flag=15
    ) ENGINE=CONNECT DEFAULT CHARSET=latin1 `table_type`=CSV `file_name`='/var/mariadb/data/Census/1976-2020-president.csv' `header`=1 `sep_char`=',' `quoted`=1

