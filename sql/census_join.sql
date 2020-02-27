use census;
select t1.code, t2.population, t2.reps from state_names as t1
inner join apportion as t2 on t1.state = t2.state order by population desc;
select TotRes from over18_2018 where Area = 'United States';
select t1.code, t2.state, t2.population, t2.reps, t3.TotRes from state_names as t1
inner join apportion as t2 on t1.state = t2.state
inner join over18_2018 as t3 on t2.state = t3.area order by population desc;
select t1.code, t2.state, t2.population, t3.AprCensus as pop_est from state_names as t1
inner join apportion as t2 on t1.state = t2.state
inner join pop_est as t3 on t2.state = t3.area order by population desc;
