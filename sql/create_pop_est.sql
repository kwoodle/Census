-- Area,ApCensus,ApEstBase,2010,2011,2012,2013,2014,2015,2016,2017,2018
-- UnitedStates,"308,745,538","308,758,105","309,326,085","311,580,009","313,874,218","316,057,727","318,386,421","320,742,673","323,071,342","325,147,121","327,167,434"
use census;
drop table if exists `pop_est`;
create table `pop_est`
(
    `AREA`       VARCHAR(50) NOT NULL,
    `AprCENSUS`  INT         NOT NULL,
    `AprEstBASE` INT         NOT NULL,
    `2010`       INT         NOT NULL,
    `2011`       INT         NOT NULL,
    `2012`       INT         NOT NULL,
    `2013`       INT         NOT NULL,
    `2014`       INT         NOT NULL,
    `2015`       INT         NOT NULL,
    `2016`       INT         NOT NULL,
    `2017`       INT         NOT NULL,
    `2018`       INT         NOT NULL
) ENGINE = InnoDB
  DEFAULT CHARSET = latin1;