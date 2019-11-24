-- Geographic Area|Total Resident Population|Resident Population Age 18 Years and Older|Total Percent
use census;
drop table if exists `over18_2018`;
create table `over18_2018`
(
    `AREA`    VARCHAR(50) NOT NULL,
    `TotRes`  INT         NOT NULL,
    `OVER18`  INT         NOT NULL,
    `PERCENT` FLOAT       NOT NULL
) ENGINE = InnoDB
  DEFAULT CHARSET = latin1;