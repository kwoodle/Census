//
// Created by kwoodle on 4/24/18.
//
// Get data for a geoID form Census Reporter

/* If you know a geoID for a place, you can automatically go to the Census Reporter profile for that place.
 * The URL format is censusreporter.com/profile/{geoid}. You may notice that in your browser, the profile URLs
 * are longer, including the name of the place on the page. Don't worry about that; Census Reporter recognizes
 * "geoID only" URLs and handles them. So, this URL will take you to the
 * profile for Chicago: censusreporter.org/profiles/16000US1714000/
 * */

#ifndef CENSUS_GEOID_H
#define CENSUS_GEOID_H

#include <ktrade/ktcurl.h>
#include <ktrade/HtmlDoc.h>
#include <vector>
#include <string>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp> //include all types plus i/o
#include <boost/filesystem.hpp>

using std::vector;
using std::string;
using std::pair;
namespace boopt = boost::posix_time;
namespace boofs = boost::filesystem;        // to get name of executable

using namespace drk;


#endif //CENSUS_GEOID_H

/* ==> NAMES_ST36_NY_SLDL.txt <==
STATEFP|DISTRICT|NAME|NAMELSAD
36|001|1|Assembly District 1            62000US36001

==> NAMES_ST36_NY_INCPLACE.txt <==
STATEFP|PLACEFP|NAME|NAMELSAD
36|05771|Bellport|Bellport village      16000US3605771

==> NAMES_ST36_NY_VTD.txt <==
STATEFP|COUNTYFP|DISTRICT|NAME|NAMELSAD
36|001|1|010101|Voting District 010101  70000US360011
 */
