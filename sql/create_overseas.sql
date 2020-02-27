use census;
drop table if exists `overseas_2010`;
create table `overseas_2010`
(
    `STATE`          VARCHAR(50) NOT NULL,
    `TOTAL`          INT         NOT NULL,
    `EMPLOYESS`      INT         NOT NULL,
    `DEPENDENTS`     INT         NOT NULL,
    `TOTAL_AF`       INT         NOT NULL,
    `EMPLOYESS_AF`   INT         NOT NULL,
    `DEPENDENTS_AF`  INT         NOT NULL,
    `TOTAL_Civ`      INT         NOT NULL,
    `EMPLOYESS_Civ`  INT         NOT NULL,
    `DEPENDENTS_Civ` INT         NOT NULL
) ENGINE = InnoDB
  DEFAULT CHARSET = latin1;
