load data local infile '/home/kwoodle/Documents/Census/data/state_names.csv'
    replace into table census.state_names
    fields terminated by ',' ignore 1 lines;

