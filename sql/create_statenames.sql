use census;
drop table if exists `state_names`;
create table `state_names`
(
    `State`  VARCHAR(50) NOT NULL,
    `Abbrev` VARCHAR(10) NOT NULL,
    `Code`   VARCHAR(2)  NOT NULL
) ENGINE = InnoDB
  DEFAULT CHARSET = latin1;
