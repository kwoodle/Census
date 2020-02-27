load data local infile '/home/kwoodle/Documents/Census/data/Apportionment2010/2010CensusOverseasCounts_fixed.csv'
    replace into table census.overseas_2010
    fields terminated by ';' ignore 1 lines;
