load data local infile '/home/kwoodle/Documents/Census/data/SCPRC-EST2018-18+POP-RESfixed.csv'
    replace into table census.over18_2018
    fields terminated by '|' ignore 1 lines;
