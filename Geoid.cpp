//
// Created by kwoodle on 4/24/18.
//
// Get data for a geoID from Census Reporter
// !! Electoral Districts, i.e. Census Voting Districts, are only? available in
// decenial, 2000, 2010, Not in American Community Survey.

#include "Geoid.h"
#include <iosfwd>

using std::cout;

int main(int argc, char *argv[]) {
//    URLVec urls{"http://example.com", "http://examplex@.com", "https://www.iana.org"};
//                    150 00 US 36 1031593001	Block Group 1, Suffolk, NY
//                    15000US361031593001	Block Group 1, Suffolk, NY
//                    16000US3605771	Bellport, NY
    string base{"https://censusreporter.org/profiles/"};
    URLVec urls{base + "16000US1714000/",
                base + "62000US36001/",
                base + "16000US3605771/",
                base + "15000US361031593001/"};
    // Get name of this application
    // to be used to create somewhat unique filename
    // in which to store data
    //
    boofs::path app_path{argv[0]};
    string app_name = app_path.filename().string();
    Multi curlm(app_name);
    // Initialize multi_curl with vector of urls to connect to
    // and a name which acts as a base for files to contain received data
    //
    // urls = test_urls;
    while (!urls.empty()) {
        curlm.load(urls);
        ResVec results = curlm.do_perform(urls);

        //?? using CURLMap = std::map<CURL*, tuple<URL, FILE*, DocFile>>;
        // using Resp = pair<URL, DocFile>;
        // using ResVec = vector<Resp>;
        for (auto &e : results) {
            auto u = e.first;
            auto d = e.second;
            HtmlDoc doc{d};

            Xpath xpath{"//h1"}; // xpath for first HTML header 1  <h1> in document
            doc.GetNodeset(xpath);
            string para;
            if (doc.result()) {
                // get content string for first header 1
                para = doc.GetNodeString(doc.nodeset()->nodeTab[0]->xmlChildrenNode, 1);
                cout << "From first HTML h1 in " << u << "\n";
                cout << para << "\n\n";
            } else {
                cout << "No match for xpath " << xpath << " in doc from " << u << "\n\n";
            }
        }
    }

    return 0;
}