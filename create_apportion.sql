use census;
drop table if exists `apportion`;
create table `apportion`
(
    `STATE`      VARCHAR(50) NOT NULL,
    `POPULATION` INT         NOT NULL,
    `REPS`       SMALLINT    NOT NULL,
    `DELTA`      VARCHAR(5) DEFAULT NULL
) ENGINE = InnoDB
  DEFAULT CHARSET = latin1;
