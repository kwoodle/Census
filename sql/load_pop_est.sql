load data local infile '/home/kwoodle/Documents/Census/data/est2018fixed.csv'
    replace into table census.pop_est
    fields terminated by '|';

